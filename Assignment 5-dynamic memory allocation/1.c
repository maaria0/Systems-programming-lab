#include<stdio.h>
#include<unistd.h>
typedef struct meta_block{
  size_t size;
  int free;
  struct meta_block *next;
}meta_block;
meta_block *global_base=NULL;
#define META_SIZE sizeof(meta_block)
meta_block*find_first_free(meta_block **last,size_t size){
  meta_block *current=global_base;
  while(current &&!(current->free && current->size>=size)){
    *last=current;
    current=current->next;
  }
  return current;
}
meta_block *request_space(meta_block *last,size_t size){
  meta_block *block=sbrk(0);
  void *request=sbrk(size+META_SIZE);
  if(request==(void*)-1){
    return NULL;
  }
  if(last){
    last->next=block;
  }
  block->free=0;
  block->next=NULL;
  block->size=size;
  return block;
}
meta_block*my_malloc(size_t size){
  meta_block *block;
  if(size<=0){
    return NULL;
  }
  if(!global_base){
    block=request_space(NULL,size);
    if(!block)return NULL;
    global_base=block;
  }else{
    meta_block *last=global_base;
    block=find_first_free(&last,size);
    if(!block){
      block=request_space(last,size);
      if(!block) return NULL;
    }else{
      block->free=0;
    }
  }
  return (block+1);
}
void *get_block_ptr(void *ptr){
  return (meta_block*)ptr-1;
}
void my_free(void *ptr){
  if(!ptr) return;
  meta_block *block=get_block_ptr(ptr);
  block->free=1;
  return;
}
typedef struct node{
  int data;
  struct node*next;
}node;
node* enqueue(node*front,node*rear,int x){
  node*newnode=(node*)my_malloc(sizeof(node));
  newnode->data=x;
  newnode->next=NULL;
  //queue empty means : rear=NULL. 
  if(!front){
    front=newnode;
    rear=newnode;
  }else{
    rear->next=newnode;
    rear=newnode;
  }  
 printf("rear is now %d\n",rear->data);
 //   printf("front is now %d\n",front->data);
  return front;
}
node* dequeue(node*front,node*rear){
  if(!front){
    printf("empty\n");
    return NULL;
  }
  node*temp=front;
  if(temp)printf("%d is deleted\n",temp->data);
  front=front->next;
  if(!front){
  front=NULL;
  rear=NULL;
  }
  my_free(temp);
  return front;
}
void checkfront(node*front,node*rear){
  if(!front &&!rear) printf("empty\n");
  node*temp=front;
  if(temp)printf("%d is front ",temp->data);
}
void isempty(node*front,node*rear){
  if(!front && !rear) printf("empty\n");
  else printf("not empty\n");
}
int main(){
  int ch,x;
  node*front=NULL;
node*rear=NULL;
  while(1){
     printf("1.enqueue 2.dequeue 3. front 4.is empty 5.print 6.exit\n");
    scanf("%d",&ch);
    if(ch==1){
      scanf("%d",&x);
      front=enqueue(front,rear,x);
    }else if(ch==2){
      front=dequeue(front,rear);
    }else if(ch==3){
      checkfront(front,rear);
    }else if(ch==4){
      isempty(front,rear);
    }else if(ch==5){
      if(!front){
      printf("empty\n");
      continue;
     }
      node*curr=front;
      while(curr){
        printf("%d ",curr->data);
        curr=curr->next;
      }
      printf("\n");
    }else{
      break;
    }
  }
}
