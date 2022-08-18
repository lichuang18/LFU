

#ifndef LRUCACHEIMPL_H   //先测试是否定义过，如果没有 接下来定义这个
#define LRUCACHEIMPL_H

#include <semaphore.h>
#include <map>

typedef unsigned long long u64; 
/*定义LFU缓存的缓存单元*/
typedef struct Node{
    u64 key;   /* 数据的key,缓存容量的大小*/
    u64 data;  /* 数据的data*/
    u64 freq;  /* 访问数据的频率*/
    struct Node *Prev;    /* 缓存双向链表指针， 指向链表的前一个元素 */
    struct Node *Next;    /* 缓存双向链表指针， 指向链表后一个元素 */
}Node;

/* 定义LU缓存 */
typedef struct Lfulist{
    //u64 cachesize;  /*缓存的容量*/
    u64 lfuListSize;    /*缓存的双向链表节点个数*/
    int hit;
    Node *Head;   /*缓存的双向链表表头*/
    Node *Tail;   /*缓存的双向链表表尾*/
}Lfulist;
#endif 
