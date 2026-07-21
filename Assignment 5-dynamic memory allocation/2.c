#include<stdio.h>
#include<unistd.h>
#include<string.h>
typedef struct meta_block{
    int free;
    size_t size;
    struct meta_block *next;
}meta_block;
//3 functions
meta_block *global_base=NULL;
#define META_SIZE sizeof(meta_block)
meta_block * find_first_free_block(meta_block **last,size_t size){
    meta_block*current=global_base;
    while(current && !(current->free && current->size>=size )){
        *last=current;
        current=current->next;
    }
    return current;
}
meta_block *request_space(meta_block *last,size_t size){
    meta_block *block=sbrk(0);
    void *request=sbrk(size+META_SIZE);
    if((void*)request==-1){
        return NULL;
    }
    if(last){
        last->next=block;
    }
    block->free=0; //block isnt free now 
    block->next=NULL;
    block->size=size;
    return block;
}
meta_block *my_malloc(size_t size){
    meta_block*block;
    if(size<=0){
        return NULL; //invalid request
    }
    //now check if it starts from global base or not
    if(!global_base){
        block=request_space(NULL,size);
        if(!block)return NULL;
    }else{
        meta_block *last=global_base;
        block=find_first_free_block(&last,size);
        if(!block){
            block=request_space(last,size);
            if(!block) return NULL;
        }else{
            block->free=0; //block isnt free 
        }
    }
    return block+1;
}
meta_block *get_ptr(void *ptr){
    return (meta_block*)ptr-1;
}
meta_block *my_calloc(size_t num,size_t size){
    size_t total_size=num*size;
    void *ptr=my_malloc(total_size); //request total_size space 
    if(!ptr)return NULL;
    memset(ptr,0,total_size); //set all to 0.
    return ptr;
}
void my_free(void *ptr){
    if(!ptr) return;
    meta_block *block=get_ptr(ptr);
    block->free=1;
}
//matrix functionss
int **create_matrix(int rows,int cols){
    int **matrix=(int **)my_calloc(rows,sizeof(int*));
    for(int i=0;i<rows;i++){
        matrix[i]=(int*)my_calloc(cols,sizeof(int));
    }
    return matrix;
}
void fill_matrix(int **matrix,int rows,int cols){
    int value=1;
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            matrix[i][j]=value++;
        }
    }
}
void free_matrix(int **matrix,int rows){
    for(int i=0;i<rows;i++)my_free(matrix[i]);
    my_free(matrix);
}
int main(){
    int rows=3;int cols=3;
    int **mat=create_matrix(3,3);
    fill_matrix(mat,3,3);
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            printf("%d ",mat[i][j]);
        }
        printf("\n");
    }
    free_matrix(mat,3);
}