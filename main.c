#include <stdio.h>
#include <string.h>

#include "lfu.h"
#include "lfu_impl.h"
#include <map>
#include <iostream>

#define N 3//8*1024//创建N个缓存
#define NUM_KEY 1024
#define D 100000 //生成的随机数范围
#define N2 2
#define N3 2
//随机1000万个key
//第一层8K个
//算一下miss和hit
u64 freq[D] = {0};

void zipf(u64* k, u64* fr,u64** ord)//输入是一堆数组  输出是分布好的数组，频率越高，排名越靠前
{
    int i,j; 
    u64 max = 0, max_index = 0;   
    /*for(i = 0; i<N ; i++)
        k[i]=rand()%D;*/
    //统计最高频率，然后放在首位
     
    for(i = 0; i<NUM_KEY ; i++)
    {
        for(j = 0; j < D ; j++)
        {
            if(k[i] == j) 
            {
                ++fr[j];//统计得到的频率数组，数组内的数据就是频次，  数组索引 就是频率对应的值
                ++freq[j];
            }
        }  
    }
    //maxsort
    for(i = 0; i < D; i++)
    {
        max = 0;
        
        for(j = 0; j < D; j++)
        {
            //printf("!!!fr[%d] = %lld   %lld\n",j,fr[j],max);
            if(fr[j]>max)
            {
                max = fr[j];
                //printf("max :%lld ,max_index =%lld, J :%d\n",max,max_index,j);
                max_index = j;
            }      
        }
        ord[0][i] = max;
        ord[1][i] = max_index;
        fr[max_index] = NULL;
    }
}

int hit = 0;
using namespace std;
int main(int argc, char **argv){
    //准备数据
    u64 *key = (u64 *)malloc(NUM_KEY * sizeof(u64));
    u64 *fr = (u64 *)malloc(D * sizeof(u64));
    u64 **ord = (u64 **)malloc(2 * sizeof(u64*));

    ord[0] = (u64 *)malloc(D * sizeof(u64));
    ord[1] = (u64 *)malloc(D * sizeof(u64));

    for(int i = 0; i<NUM_KEY ; i++)
        key[i]=rand()%D;
    /*
    for(int i = 0; i<NUM_KEY ; i++ )
    {
        printf("%lld,",key[i]);
    }
    printf("\n");
    */
    zipf(key,fr,ord);
    printf("\nout of ord freq\n");
    //打印freq信息
    for(int i = 0; i<D ; i++ )
    {
        printf("%lld,",freq[i]);
    }
    /*
        printf("\nfreq\n");
        //打印freq信息
        for(int i = 0; i<D ; i++ )
        {
            printf("%lld,",ord[0][i]);
        }
        printf("\nfreq  value\n");
        //打印freq对应的值信息
        for(int i = 0; i<D ; i++ )
        {
            printf("%lld,",ord[1][i]);
        }
        printf("\nxunsishane?\n");
    */
    //创建缓存器
    void *lfulist;
    if (0 == LfulistCreate(&lfulist))
        printf("缓存器创建成功\n");
    //向缓存器中添加数据  
    for(int i=0; i<D; i++)
    {
        for(int j=0; j<ord[0][i]; j++)
        {
            if (0 != Put(lfulist, ord[1][i], ord[1][i]))//ord[1][i]    
                printf("put (%lld, %lld) failed!\n",ord[1][i], ord[1][i]);
        }
    }
    lfulistPrint();
    printf("insert次数:%d\n",NUM_KEY);
/* 
    //向缓存器中添加数据   手动单次添加，方便调试观测
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
*/
}
