#include <stdio.h>
#include <string.h>

#include "lfu.h"
#include "lfu_impl.h"
#include <map>
#include <iostream>

#define N 3//8*1024//创建N个缓存
#define NUM_KEY 10//1024*1024

#define N2 2
#define N3 2
//随机1000万个key
//第一层8K个
//算一下miss和hit
int hit = 0;
using namespace std;
int main(int argc, char **argv){
    void *lfulist;
    //创建缓存器
    if (0 == LfulistCreate(&lfulist))
        printf("缓存器创建成功\n");

    //向缓存器中添加数据
    u64 a_key = 123;
    u64 a_value = 1234;

    if (0 != Put(lfulist, 1, 1234))
        printf("put (1, 1234) failed!\n");
    lfulistPrint(); 
    if (0 != Put(lfulist, 1, 38))
        printf("put (1, 1234) failed!\n");
    lfulistPrint(); 
    if (0 != Put(lfulist, 1, 52))
        printf("put (1, 1234) failed!\n");
    lfulistPrint(); 
    
    if (0 != Put(lfulist, 1, 62))
        printf("put (1, 1234) failed!\n");
    lfulistPrint(); 
    if (0 != Put(lfulist, 2, 1234))
        printf("put (1, 1234) failed!\n");
    lfulistPrint(); 
    if (0 != Put(lfulist, 2, 1234))
        printf("put (1, 1234) failed!\n");
    lfulistPrint(); 
    if (0 != Put(lfulist, 2, 1234))
        printf("put (1, 1234) failed!\n");
    lfulistPrint();
    if (0 != Put(lfulist, 5, 1234))
        printf("put (1, 1234) failed!\n");
    lfulistPrint();
    if (0 != Put(lfulist, 5, 1234))
        printf("put (1, 1234) failed!\n");
    lfulistPrint();
    if (0 != Put(lfulist, 3, 1234))
        printf("put (1, 1234) failed!\n");
    lfulistPrint();
    if (0 != Put(lfulist, 4, 1234))
        printf("put (1, 1234) failed!\n");
    lfulistPrint();
    if (0 != Put(lfulist, 6, 1234))
        printf("put (1, 1234) failed!\n");
    lfulistPrint();
    /*
    if (0 != Put(lfulist, 3, 1234))
        printf("put (1, 1234) failed!\n");
    lfulistPrint();  
    if (0 != Put(lfulist, 4, 1234))
        printf("put (1, 1234) failed!\n");
    lfulistPrint(); 
    if (0 != Put(lfulist, 5, 1234))
        printf("put (1, 1234) failed!\n");
    lfulistPrint();
    if (0 != Put(lfulist, 6, 1234))
        printf("put (1, 1234) failed!\n");
    lfulistPrint();
    */
    /*
    if (0 != lfulistSet(lfulist, 2, 1234))
        printf("put (2, 1234) failed!\n");
    lfulistPrint(lfulist);
    if (0 != lfulistSet(lfulist, 1, 55))
        printf("put (2, 1234) failed!\n");
    if (0 != lfulistSet(lfulist, 1, 33))
        printf("put (2, 1234) failed!\n");
    lfulistPrint(lfulist);
    if (0 != lfulistSet(lfulist, 3, 25))
        printf("put (2, 1234) failed!\n");
    lfulistPrint(lfulist);
    if (0 != lfulistSet(lfulist, 4, 86))
        printf("put (2, 1234) failed!\n");
    lfulistPrint(lfulist);
    */
}
