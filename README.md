## [LFU_Cache](https://github.com/lichuang18/LFU)
参考<基于双向链表和哈希表实现的LRU缓存>，C语言混合C++实现。参考改编自[https://github.com/lichuang18/LFU](https://github.com/Stand1210/c-LRU-)

### 描述
+ LFU缓存置换算法
+ 基于双向链表和哈希表的存储模型
+ 基于键值对，键和值的数据类型为u64和指针
+ 没有全局存储结构的写操作上锁，并发安全（参考实现有多线程，可自行借鉴）

#####说明：
这里描述用的node就是缓存里的一项存储单元，list就是整个缓存器

```
>>参考LRU，用map函数构建哈希表，建立key到node映射关系 map<key,node>
>>其次再用一层哈希表，建立freq到list的关系 map<freq,list>
>>list是双向链表
>>用minfreq标记访问频率最小的level
>>访问频次增加，就从当前频次level删除，然后在更高的频次level插入该缓存node
>>当缓存满了，删除minfreq标记的访频最低的level的表尾的缓存node。
>>插入时，插入到对应访频level的list的表头。
```
### usage
> 基本执行
```
make
./main
```
