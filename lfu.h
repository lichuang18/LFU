#ifndef LRUCACHE_H
#define LRUCACHE_H


typedef unsigned long long u64;

/*创建LRU缓存*/
int LfulistCreate(void **lfulist);
/*销毁LRU缓存*/
int LfulistDestory(void *lfulist);
/*将数据放入LRU缓存中*/
int Put(void *lfulist, u64 key, u64 data);
/*从缓存中获取数据*/
u64 Get(void *lfulist, u64 key);
/*打印缓存中的数据， 按访问时间从新到旧的顺序输出*/
void lfulistPrint();


#endif 
