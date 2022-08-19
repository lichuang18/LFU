#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#include "lfu.h"
#include "lfu_impl.h"
#include <map>
#define DEBUG 0
#define Cachesize 5
std::map<u64,Lfulist *> freqMap;
// 创建一个缓存list 
u64 minfreq; 

int LfulistCreate(void **lfulist)
{
    Lfulist* cache = NULL;
    if (NULL == (cache=(Lfulist*)malloc(sizeof(*cache)))) {
        perror("molloc");
        return -1;
    }
    cache->lfuListSize=0;
    //cache->cachesize = capacity;
    cache->hit=0;
    *lfulist = cache;
    return 0;
}

static Node* newNode(u64 key, u64 data)//创建一个Node
{  
    Node* entry = NULL; 
    if (NULL == (entry=(Node*)malloc(sizeof(*entry)))) {
        perror("malloc");
        return NULL;
    }
    entry->data = data;
    entry->key = key;
    entry->freq = 1;
    return entry;
}
u64 sum_capa()
{
    u64 sum=0;
    auto it = freqMap.begin();
    for(it; it != freqMap.end(); it++)
    {
        sum = sum + it->second->lfuListSize;
    }
    return sum;
}

// 从双向链表中删除指定节点 
static void removeFromList(Lfulist *cache, Node *entry)
{
    if(entry->freq == 1)
    {   //链表为空//
        if(DEBUG) printf("freq 1 level remove!\n");
        if (cache->lfuListSize==0) {
            return;
        }
        if (entry==cache->Head && entry==cache->Tail) {
            //链表中仅剩当前一个节点 //  
            if(DEBUG) printf("list only have 1 node!\n");   
            cache->Head = cache->Tail = NULL;
            //printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&%p",cache->Head);
            //printf("%p",freqMap.find(1)->second->Tail);
            //printf("%p",freqMap.find(1)->second->Head);
            minfreq = entry -> freq + 1;
        } else if (entry == cache->Head) {
            //欲删除节点位于表头//
            //if(DEBUG) printf("list head remove\n");
            cache->Head = entry->Next;
            cache->Head->Prev = NULL;
        } else if (entry == cache->Tail) {
            //欲删除节点位于表尾//
            cache->Tail = entry->Prev;
            cache->Tail->Next = NULL;
        } else {
            //其他非表头表尾情况，直接摘抄节点
            entry->Prev->Next = entry->Next;
            entry->Next->Prev = entry->Prev;
        }
        //删除成功， 链表节点数减1
        cache->lfuListSize--;
    } else
    { 
        if(DEBUG) printf("freq %lld remove!\n",entry->freq);
        cache = freqMap.find(entry->freq)->second;

        if(DEBUG) printf("当前的entry:%lld\n",cache->lfuListSize);
        if (cache->lfuListSize==0) {
            return;
        }
        if (entry==cache->Head && entry==cache->Tail) {
            // 链表中仅剩当前一个节点 //  
            if(DEBUG) printf("只有一个node!\n");    
            cache->Head = cache->Tail = NULL;
            minfreq = entry -> freq + 1;
        } else if (entry == cache->Head) {
            //欲删除节点位于表头//
            cache->Head = entry->Next;
            cache->Head->Prev = NULL;
        } else if (entry == cache->Tail) {
            //欲删除节点位于表尾//
            cache->Tail = entry->Prev;
            cache->Tail->Next = NULL;
        } else {
            //其他非表头表尾情况，直接摘抄节点
            entry->Prev->Next = entry->Next;
            entry->Next->Prev = entry->Prev;
        }
        //删除成功， 链表节点数减1
        cache->lfuListSize--;
    }
    //if(DEBUG) printf("remove over!\n");
    //if(DEBUG) printf("remove某些节点后最小的minfreq:%d\n",minfreq);
}

//插入下一freq层
static Node* insertToListHead(Lfulist *cache, Node *entry)
{
    Node *removedEntry = NULL;       
    //printf("总的cache size: %lld, 当前层cache size: %lld\n",zongs,cache->lfuListSize);
    
    if(entry->freq == 1)
    {
        ++cache->lfuListSize;
        u64 zongs = sum_capa();
        if(DEBUG) printf("freq = %lld ,insert\n",entry->freq);
        if (zongs > Cachesize) {
            //如果缓存满了， 即链表当前节点数已等于缓存容量， 
            //那么需要先删除链表表尾节点， 即淘汰最久没有被访问到的缓存数据单元
            if(DEBUG) printf("min freq:%lld\n",minfreq);
            removedEntry = freqMap.find(minfreq)->second->Tail;
            removeFromList(freqMap.find(minfreq)->second, freqMap.find(minfreq)->second->Tail);
        }
            //如果当前链表为空链表
        if (cache->Head == NULL && cache->Tail == NULL) {
            if(DEBUG) printf("第一次插入必定执行!\n");
            cache->Head = cache->Tail = entry;
            minfreq = entry->freq;//如果往之前清空过的list添加东西，需要让minfreq置为当前freq
        } else { //当前链表非空， 插入表头
            //entry 是新node  iru***head是旧node
            entry->Next = cache->Head;//new的next指向old
            entry->Prev = NULL;              //new的prev指向null
            cache->Head->Prev = entry;//old的prev指向new
            cache->Head = entry;             //更新list的表头为new
        }
    } else//新的freq level
    {
        
        if(DEBUG) printf("第%lld freq层的insert!!!\n",entry->freq);
        
        if(freqMap.find(entry->freq) == freqMap.end())//如果不存在映射，那就创建
        {
            if(DEBUG) printf("开始创建freq %lld 的cache映射!!!\n",entry->freq);
            void *lfulist;
            if (0 == LfulistCreate(&lfulist))
            {
                if(DEBUG) printf("新缓存器创建成功\n");
            }  
            Lfulist *newcache = (Lfulist *)lfulist;
            freqMap.insert({entry->freq,newcache});//创建freq 和 list的映射  
            cache = newcache;
        } else//如果存在，就用之前的
        {
            cache = freqMap.find(entry->freq)->second;
        }
        ++cache->lfuListSize;
        u64 zongs = sum_capa();
        //printf("!!!!!!!!!!  freq :%lld, listsize:%lld, sum_capa:%lld \n",entry->freq,cache->lfuListSize,zongs);
        if (zongs > Cachesize) {
            //如果缓存满了， 即链表当前节点数已等于缓存容量， 
            //那么需要先删除链表表尾节点， 即淘汰最久没有被访问到的缓存数据单元
            removedEntry = freqMap.find(minfreq)->second->Tail;
            removeFromList(freqMap.find(minfreq)->second, freqMap.find(minfreq)->second->Tail);
        }
            //如果当前链表为空链表
        if (cache->Head == NULL && cache->Tail == NULL) {
            if(DEBUG) printf("第%lld freq层的插入,空链情况！\n",entry->freq);
            cache->Head = cache->Tail = entry;
            //printf("新链表的地址:%p,head:%p,tail:%p\n",freqMap.find(2)->second,freqMap.find(2)->second->Head,freqMap.find(2)->second->Tail);
        } else 
        {   //当前链表非空， 插入表头
            //entry 是新node  iru***head是旧node
            entry->Next = cache->Head;//new的next指向old
            entry->Prev = NULL;              //new的prev指向null
            cache->Head->Prev = entry;//old的prev指向new
            cache->Head = entry;             //更新list的表头为new
        }
    } 
    return removedEntry;
}


std::map<u64,Node *> searMap;//<key, Node>
//put操作// 将数据放入缓存中
int Put(void *lfulist, u64 key, u64 data)
{
    Lfulist *cache = (Lfulist *)lfulist;
    //从哈希表中查找数据是否已经存在缓存中//
    Node *entry= NULL;
    
    if(freqMap.empty())
    {
        printf("空的freq cache\n");
        //第一次把数据put到缓存，需要建立频率为1的缓存hash表映射
        freqMap.insert({1,cache});
        minfreq = 1;
        //if(DEBUG) printf("成功创建频率为1的list映射!\n");
    } 
    auto it = searMap.find(key);
    if(it == searMap.end())//缓存miss
    {
        ////freqMap.find(i) == freqMap.end(); 判断频次i所对应list是否存在
        entry = newNode(key, data);//创建新的缓存单元，也就是创建节点
        //将新建缓存单元插入链表表头，这时候需要考虑缓存是否满了
        Node *removedEntry = insertToListHead(cache, entry);
        if (NULL != removedEntry) {
            //将新建缓存单元插入链表表头过程中， 发生缓存满了的情况， 需要淘汰最久没有被访问到的缓存数据单元//         
            auto it = searMap.find(removedEntry->key);
            delete it->second;
            it->second = NULL;
            searMap.erase(it);
        }
        if(DEBUG){
            printf("***************MISS!!!****************");
            printf("insert key:%lld, value:%lld,\n",key,entry->data);
            // printf("node addr:%p,node->next%p,full is empty:%p\n",entry,entry->Next,removedEntry);
            // printf("cache size:%d  cache head:%p,",cache->lfuListSize,cache->Head);
            // printf("cache tail:%p\n",cache->Tail);
        }

        //将新建缓存单元插入哈希表//
        searMap.insert({key,entry});
    } else//缓存hit
    {
        cache->hit++;
        //更新数据， 将该数据更新到链表表头//
        entry = it->second;//节点信息
        entry->data = data;
        if(DEBUG){
            printf("****************hit!!!******************");
            printf("insert key:%lld, value:%lld,\n",key,entry->data);
            // printf("node addr:%p,node->next%p",entry,entry->Next);
            // printf("cache size:%d  cache head:%p,",cache->lfuListSize,cache->Head);
            // printf("cache tail:%p\n",cache->Tail);
        }
        removeFromList(cache, entry);
        ++entry->freq;//接下来需要将元素从访问频率i层，放到i+1
        if(DEBUG) printf("After hit, this data_freq = %lld\n",entry->freq);
        Node *removedEntry = insertToListHead(cache,entry);
        //insertToListHead(cache, entry);
        //updateLRUList(cache, entry);
        //printf("insert key:%lld, value:%lld, node addr:%p,node->next%p\n",key,entry->data,entry,entry->lruListNext);
        //printf("cache size:%d  cache head:%p , cache tail:%p\n",cache->lfuListSize,cache->lruListHead,cache->lruListTail);
    }
    return 0;
}

/*
static void freeList(Lfulist *cache);
// 创建一个缓存单元    创建一个节点

// 释放一个缓存单元 //
static void freeNode(Node* entry)
{
    if (NULL == entry) return;
    free(entry);
}
// 双向链表相关接口及实现

//辅助性接口， 用于保证最近访问的节点总是位于链表表头
static void updateLRUList(Lfulist *cache, Node *entry)
{
    //将节点从链表中摘抄
    removeFromList(cache, entry);  //remove(   list  ,  node  )
    //将节点插入链表表头
    insertToListHead(cache, entry);
}

*/

/*
// 释放一个LRU缓存 //
int lfulistDestory(void *lfulist)
{
    Lfulist* cache = (Lfulist*)lfulist;
    if (NULL == cache) return 0;
    //free searMap
    auto iter = searMap.begin();
    while (iter != searMap.end())
    {
        delete iter->second;
        iter->second = NULL;
        iter = searMap.erase(iter);
    }
    //free linklist
    freeList(cache);
    free(cache);
    return 0;
}


//释放整个链表
static void freeList(Lfulist* cache)
{
    //链表为空
    if (0 == cache->lfuListSize) {
        return;
    }
    
    Node *entry = cache->lruListHead;
    //遍历删除链表上所有节点
    while(entry) {
        Node *temp = entry->lruListNext;
        freeCacheEntry(entry);
        entry = temp;
    }
    cache->lfuListSize = 0;
}

//从缓存中获取数据//
u64 lfulistGet(void *lfulist, u64 key)
{
    Lfulist *cache = (Lfulist *)lfulist;
    //从哈希表查找数据是否已经在缓存中//
    auto iter = searMap.find(key);
    Node* entry = NULL;
    if (iter != searMap.end()) {
        //缓存中存在该数据， 更新至链表表头//
        entry = iter->second;
        updateLRUList(cache, entry);
        //返回数据//
        return entry->data;
    } else {
        //缓存中不存在相关数据//
        printf("cache miss\n");
        return 0;
    }
}

//调试接口
*/

u64 sum_hit()
{
    u64 sum=0;
    auto it = freqMap.begin();
    for(it; it != freqMap.end(); it++)
    {
        sum = sum + it->second->hit;
    }
    return sum;
}
//遍历缓存列表， 打印缓存中的数据， 按访问时间从新到旧的顺序输出//
void lfulistPrint()
{
    Lfulist *cache = NULL;
    auto it = freqMap.begin();
    u64 zongs = sum_capa();
    for(it ;it != freqMap.end();it++)
    {
        if(DEBUG) printf("第%lld频次",it->first);
        cache=it->second;
        if(cache->Head == NULL)
        {
            if(DEBUG) printf("this freq level is NULL!\n");
        } else
        {   
            if (NULL == cache || 0 == zongs) return;
            fprintf(stdout, "\nfre=%lld>>>>>>>>>>>>\n",it->first);
            fprintf(stdout, "cache (key data):\n");
            Node* entry = cache->Head;
            while (entry) 
            {
                fprintf(stdout, "(%lld, %lld)", entry->key, entry->data);
                entry = entry->Next;
            }  
            fprintf(stdout, "\n<<<<<<<<<<<<<<<\n");
        }
    }  
    if(DEBUG) printf("当前缓存容量为:%lld,最小freq层的频率为%lld\n",zongs,minfreq);
    printf("hit次数为:%lld\n",sum_hit());
}
