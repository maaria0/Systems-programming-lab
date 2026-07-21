//basically in this ques->array resizing
#include<stdio.h>
#include<unistd.h>
#include<string.h>
//everything is same-> from my_malloc u have to write my_realloc , then some more functions.
typedef struct meta_block {
    int free;
    size_t size;
    meta_block * next;
}meta_block;
#define META_SIZE sizeof(meta_block);
meta_block *global_base=NULL;
meta_block *find_first_free_block(meta_block**last,size_t size){
    meta_block *current=global_base;
    while(current && !(current->free && current->size>=size)){
        *last=current;
        current=current->next;
    }
    return current;
}
meta_block *request_space(meta_block *last,size_t size){
    meta_block *block=sbrk(0);
    void*request=sbrk(size+META_SIZE);
    if(request==(void*)-1){
        return NULL;
    }
    if(last){
        last->next=block;
    }
    block->free=0;
    block->size=size;
    block->next=NULL;
    return block;
}
meta_block *my_malloc(size_t size){
    meta_block*block;
    if(size<=0)return NULL ; //invalid request
    if(!global_base){
        block=request_space(NULL,size);
        if(!block)return NULL;
        else block->free=0;
    }else{
        meta_block *last=global_base;
        block=find_first_free_block(last,size);
        if(!block){
            block=request_space(&last,size);
            if(!block)return NULL;
        }else{
            block->free=0;
        }
    }
    return (block+1);
}
void *get_ptr(meta_block *ptr){
    return (meta_block *)ptr-1;
}
void my_free(void *ptr){
    if(!ptr)return;
    meta_block*block=get_ptr(ptr);
    block->free=1;
}
meta_block *my_realloc(void *ptr,size_t new_size){
    if(!ptr) return my_malloc(new_size); //no block assigned yet.
    meta_block *block=get_ptr(ptr);
    if(block->size>=new_size) return block;
    void *new_ptr=my_malloc(new_size);
    if(!new_ptr) return NULL;
    memcpy(new_ptr,ptr,block->size);
    my_free(ptr);
    return new_ptr;
}
typedef struct DynamicArray{
    int *data;
    int size;
    int capacity;
}DynamicArray;
void init_array(DynamicArray *arr, int intial_size){
    arr->data=(int*)my_realloc(intial_size *sizeof(int));
    arr->size=0;
    arr->capacity=intial_size;
}
void resize_array(DynamicArray *arr,int new_size){
    arr->data=(int*)my_realloc(arr->data,new_size*sizeof(int));
    arr->capacity=new_size;
    if(arr->size>new_size) arr->size=new->size;
}
int main(){

}
