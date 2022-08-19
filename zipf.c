#include <stdio.h>

//P(r) = C / r^α  单词频率分布中 C约等于0.1, α约等于1   p(r)是频率   

#define C 0.1
#define a 1
#define N 50 //key数量
#define D 10    //分布的范围

int key[N];     //键值
int value[N];
int freq[N]={0};//频次统计
int order_k[2][N]={0};//频次及数值  [0][]频次   [1]数值
int freq_bak[N]={0};

//生成随机数
void gene_rand()
{
    for(int i = 0 ; i<N ; i++)
    {
        value[i]=key[i]=rand()%D;
    }
}

//zipf分布实现
void zipf(int k[N], int fr[N],int ord[2][N])//输入是一堆数组  输出是分布好的数组，频率越高，排名越靠前
{
    int i,j; 
    int max = 0, max_index = 0; 
    /*
    for(i = 0; i<N ; i++)
        k[i]=rand()%D;
        */
    //统计最高频率，然后放在首位
    for(i = 0; i<N ; i++)
    {
        for(j = 0; j < D ; j++)
        {
            if(k[i] == j) 
            {
                ++fr[j];//统计得到的频率数组，数组内的数据就是频次，  数组索引 就是频率对应的值
                //++freq[j]; //可以打印出来的统计频率的信息
            }
        }
        
    }
    //maxsort
    for(i = 0, j = 0; i < N-1; i ++)
    {
        max = -1;
        for(j = 0; j < N; j++)
        {
            if(fr[j] > max)
            {
                max = freq[j];
                //printf("max :%d ,max_index =%d, J :%d\n",max,max_index,j);
                max_index = j;
            }      
        }
        ord[0][i] = max;
        ord[1][i] = max_index;
        fr[max_index] = -1;
    }
}

int main()
{
    gene_rand();
    for(int i = 0; i<N ; i++ )
    {
        printf("%d,",key[i]);
    }
    printf("\nxunsishane?\n");
    int fr[N]={0};
    int ord[2][N]={0};
    zipf(key,fr,ord);
    //maxsort();
     for(int i = 0; i<D ; i++ )
    {
        printf("%d,",freq[i]);
    }
    printf("\nfreq\n");
    for(int i = 0; i<D ; i++ )
    {
        printf("%d,",ord[0][i]);
    }
    printf("\nfreq  value\n");
    for(int i = 0; i<D ; i++ )
    {
        printf("%d,",ord[1][i]);
    }
    printf("\nxunsishane?\n");
    return 0;
}