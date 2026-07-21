#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXLINE 1024
#define MAXARGS 128

void die(const char *msg) { perror(msg); exit(1); }

void trim_newline(char *s) {
    int len = strlen(s);
    if (len > 0 && s[len-1] == '\n') s[len-1] = '\0';
}

int parse_args(char *line, char *argv[]) {
    int argc = 0;
    char *tok = strtok(line, " \t");
    while (tok && argc < MAXARGS-1) {
        argv[argc++] = tok;
        tok = strtok(NULL, " \t");
    }
    argv[argc] = NULL;
    return argc;
}

void setup_C_mode(int cmd_write_fd, int saved_stdin, int saved_stdout) {
    if (dup2(saved_stdin,  STDIN_FILENO)  < 0) die("C: restore stdin");
    if (dup2(saved_stdout, STDOUT_FILENO) < 0) die("C: restore stdout");
    if (dup2(cmd_write_fd, STDOUT_FILENO) < 0) die("C: redirect stdout→pipe");
}

void setup_E_mode(int cmd_read_fd, int saved_stdin, int saved_stdout) {
    if (dup2(saved_stdin,  STDIN_FILENO)  < 0) die("E: restore stdin");
    if (dup2(saved_stdout, STDOUT_FILENO) < 0) die("E: restore stdout");
    if (dup2(cmd_read_fd,  STDIN_FILENO)  < 0) die("E: redirect stdin←pipe");
}

void run_command(char *cmdline, int saved_stdin, int saved_stdout) {
    char *args[MAXARGS];
    char  copy[MAXLINE];
    strncpy(copy, cmdline, MAXLINE-1);
    copy[MAXLINE-1] = '\0';

    if (parse_args(copy, args) == 0) return;

    pid_t pid = fork();
    if (pid < 0) die("run_command: fork");

    if (pid == 0) {
        dup2(saved_stdin,  STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        execvp(args[0], args);
        fprintf(stderr, "*** Unable to execute command\n");
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

void child_loop(char role,
                int cmd_read_fd, int cmd_write_fd,
                int sync_read_fd, int sync_write_fd)
{
    int saved_stdin  = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdin < 0 || saved_stdout < 0) die("dup saved fds");

    if (role == 'C')
        setup_C_mode(cmd_write_fd, saved_stdin, saved_stdout);
    else
        setup_E_mode(cmd_read_fd,  saved_stdin, saved_stdout);

    char line[MAXLINE];

    while (1) {

        if (role == 'C') {
            fprintf(stderr, "Enter command> ");
            fflush(stderr);

            if (fgets(line, MAXLINE, stdin) == NULL) {
                fprintf(stdout, "exit\n");
                fflush(stdout);
                break;
            }
            trim_newline(line);
            if (strlen(line) == 0) continue;

            fprintf(stdout, "%s\n", line);
            fflush(stdout);

            if (strcmp(line, "exit") == 0) break;

            if (strcmp(line, "swaprole") == 0) {
                char dummy;
                read(sync_read_fd, &dummy, 1);

                role = 'E';
                setup_E_mode(cmd_read_fd, saved_stdin, saved_stdout);
                fprintf(stderr, "Waiting for command> ");
                fflush(stderr);
            }

        } else {
            fprintf(stderr, "Waiting for command> ");
            fflush(stderr);

            if (fgets(line, MAXLINE, stdin) == NULL) break;
            trim_newline(line);
            if (strlen(line) == 0) continue;

            if (strcmp(line, "exit") == 0) break;

            if (strcmp(line, "swaprole") == 0) {
                char dummy = 1;
                write(sync_write_fd, &dummy, 1);

                role = 'C';
                setup_C_mode(cmd_write_fd, saved_stdin, saved_stdout);
            } else {
                run_command(line, saved_stdin, saved_stdout);
            }
        }
    }

    close(saved_stdin);
    close(saved_stdout);
}

int main(int argc, char *argv[]) {

    if (argc == 6) {
        char role         = argv[1][0];
        int cmd_read_fd   = atoi(argv[2]);
        int cmd_write_fd  = atoi(argv[3]);
        int sync_read_fd  = atoi(argv[4]);
        int sync_write_fd = atoi(argv[5]);

        child_loop(role, cmd_read_fd, cmd_write_fd,
                   sync_read_fd, sync_write_fd);
        exit(0);
    }

    printf("+++ CSE in supervisor mode: Started\n"); fflush(stdout);

    int cmd_pipe[2];
    if (pipe(cmd_pipe) < 0) die("pipe cmd");

    int sync_pipe[2];
    if (pipe(sync_pipe) < 0) die("pipe sync");

    printf("+++ CSE in supervisor mode: pfd = [%d %d]\n",
           cmd_pipe[0], cmd_pipe[1]);
    fflush(stdout);

    char rfd_s[8], wfd_s[8], srd_s[8], swd_s[8];
    snprintf(rfd_s, sizeof rfd_s, "%d", cmd_pipe[0]);
    snprintf(wfd_s, sizeof wfd_s, "%d", cmd_pipe[1]);
    snprintf(srd_s, sizeof srd_s, "%d", sync_pipe[0]);
    snprintf(swd_s, sizeof swd_s, "%d", sync_pipe[1]);

    printf("+++ CSE in supervisor mode: Forking first child in command-input mode\n");
    fflush(stdout);

    pid_t pid1 = fork();
    if (pid1 < 0) die("fork child1");
    if (pid1 == 0) {
        execlp("xterm", "xterm",
               "-T", "First Child",
               "-e", "./CSE", "C",
               rfd_s, wfd_s, srd_s, swd_s,
               (char *)NULL);
        die("execlp xterm C");
    }

    printf("+++ CSE in supervisor mode: Forking second child in execute mode\n");
    fflush(stdout);

    pid_t pid2 = fork();
    if (pid2 < 0) die("fork child2");
    if (pid2 == 0) {
        execlp("xterm", "xterm",
               "-T", "Second Child",
               "-e", "./CSE", "E",
               rfd_s, wfd_s, srd_s, swd_s,
               (char *)NULL);
        die("execlp xterm E");
    }

    close(cmd_pipe[0]);
    close(cmd_pipe[1]);
    close(sync_pipe[0]);
    close(sync_pipe[1]);

    int status;
    waitpid(pid1, &status, 0);
    printf("+++ CSE in supervisor mode: First child terminated\n");
    fflush(stdout);

    waitpid(pid2, &status, 0);
    printf("+++ CSE in supervisor mode: Second child terminated\n");
    fflush(stdout);

    return 0;
}
