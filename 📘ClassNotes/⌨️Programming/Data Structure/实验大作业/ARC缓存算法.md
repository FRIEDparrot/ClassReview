[[📘ClassNotes/⌨️Programming/Data Structure/实验大作业/自适应缓存替换ARC算法.pdf|自适应缓存替换ARC算法.pdf]]

`````ad-info
title: ARC算法的代码实现
collapse: close

main.cpp :
```cpp 
#include "ARCcacheHeader.h"

// 使用要求: 可以一次输入多个词组，并将每一个词组放置在缓存中，
int main()
{
    ARCcache container(5, 3);
    DataType input; 
    while(true) {
        getline(cin, input);
        container.put(input);
        if (input == "exit") break;
        container.Show(true);
    }
    return 0;
}
```

ARCcacheheader.h
```cpp 
#pragma once 
#include <iostream>
#include <string>
#include <unordered_map>  // 使用c++自带的哈希表
#ifndef ARCCACHEHEADER
#define ARCCACHEHEADER   // 避免头文件重复定义
#define Init_Capacity 30
#define Init_Transfrom_Time 3

using namespace std;
typedef string DataType;
/*简介:
* 使用LFU和LRU结合的方法(也即A)，实现对词汇进行自动补全功能中的缓存列表的存储操作
* 在本程序中，使用哈希表和双向链表的存储结构，结合了链表的有序特性和哈希表的查找特性
* 双向循环链表是便于移除尾部的元素
* 基于LFU(Least Recently User,最近不常用算法)和LRU(最近最少使用算法)实现一固定长度链表的插入
* 使用函数get和put来进行词组的读入操作, 并且要求get和put需要以O(1)的时间复杂度运行
*/

// 缓存节点列表
typedef struct CacheList {
    DataType key;  
    int time = 0;  // 定义使用计数器，计算使用次数
    CacheList* prev = nullptr;
    CacheList* next = nullptr;
};

// LRU类部分
class LRUcache {
public:
    LRUcache() {
        this->capacity = Init_Capacity;
        this->ghost_capacity = Init_Capacity;
    }
    LRUcache(int capacity) {
        // 注意这里不够健壮,这是由于初始化的capacity可能小于0，但暂且不管健壮性
        this->capacity = capacity;
        this->ghost_capacity = capacity; // 对ghost分配同样的存储空间
        // 分配同样大小的空间
    }
    LRUcache(int capacity, int transform_time) {
        // 注意这里不够健壮,这是由于初始化的capacity可能小于0，但暂且不管健壮性
        this->capacity = capacity;
        this->ghost_capacity = capacity; // 对ghost分配同样的存储空间
        this->transform_time = transform_time;
        // 分配同样大小的空间
    }
    bool put(DataType data); // 输入元素
    void Add(DataType data); // 输入并且扩充一格
    bool Subtract();         // 删除末尾元素且空间缩减，返回是否成功
    // 其中，由于只是插入元素，则对于ghost的处理放在插入元素函数中即可
    bool check_ghost(DataType data); // 检查ghost中是否有对应元素,有则返回true
    void Show(bool show_ghost); // 显示所有元素

private:
    CacheList* cache = nullptr;
    unordered_map<DataType, CacheList*> map;
    int size = 0;
    int capacity = Init_Capacity;

    CacheList* ghost = nullptr;
    unordered_map<DataType, CacheList*> ghost_map;
    int ghost_size = 0;
    int ghost_capacity = Init_Capacity; // ghost链表的存储空间(固定不变的)

    void Insert(CacheList* L, bool is_ghost); // 直接插入节点
    void DeleteTail(bool is_ghost); // 删除末尾节点
    void DetachNode(CacheList* L, bool is_ghost); // 脱离节点,即从表中删除但是不释放空间
    int transform_time = Init_Transfrom_Time; // 默认从LRU到LFU转移阈值是3

    /*
    * 插入函数的要求: 如果size < capacity,直接插入
    * 如果capacity已满，则先删除末尾的元素并将其加入ghost（单独定义加ghost函数）
    * 然后再将元素插入本身中
    *
    * 此时调用加入ghost的函数,这个函数的结构和Insert相同(直接插头结点)
    * 然后ghost如果满了则删除最后一个元素
    */
};

// LFU类部分
class LFUcache {
public:
    LFUcache() {
        this->capacity = Init_Capacity;
        this->ghost_capacity = Init_Capacity;
    }
    LFUcache(int capacity) {
        // 注意这里不够健壮,这是由于初始化的capacity可能小于0，但暂且不管健壮性s
        this->capacity = capacity;
        this->ghost_capacity = capacity;
        // 分配同样大小的空间
    }
    LFUcache(int capacity, int transform_time) {
        // 注意这里不够健壮,这是由于初始化的capacity可能小于0，但暂且不管健壮性s
        this->capacity = capacity;
        this->ghost_capacity = capacity;
        this->transform_time = transform_time;
        // 分配同样大小的空间
    }
    void put(DataType data); // 向LFU内增加缓存元素
    void Add(DataType data); 
    bool Subtract();
    bool check_ghost(DataType data); // 检查ghost中是否有对应元素,有则返回true
    void Show(bool show_ghost);
private:
    CacheList* cache = nullptr;
    unordered_map<DataType, CacheList*> map;
    int size = 0;
    int capacity = Init_Capacity;
    
    int transform_time = Init_Transfrom_Time; // 需要和LRU内一致
    
    CacheList* ghost = nullptr;
    unordered_map<DataType, CacheList*> ghost_map;
    int ghost_size = 0;
    int ghost_capacity = Init_Capacity;
    
    void DetachNode(CacheList* L, bool is_ghost);
    void DeleteTail(bool is_ghost); // 删除尾部元素
    void Insert(CacheList* L, bool is_ghost);     // 有序插入元素
};

class ARCcache {
public:
    ARCcache() {
        this->capacity = Init_Capacity;
        this->Rcache = new LRUcache(Init_Capacity);
        this->Fcache = new LFUcache(Init_Capacity);
    }
    ARCcache(int capacity) {
        this->Rcache = new LRUcache(capacity);
        this->Fcache = new LFUcache(capacity);
        this->capacity = capacity;
    }
    ARCcache(int capacity, int transform_time) {
        this->Rcache = new LRUcache(capacity,transform_time);
        this->Fcache = new LFUcache(capacity,transform_time);
        this->capacity = capacity;
        this->transform_time = transform_time;
    }
    void put(DataType data);
    void Show(bool show_ghost);
private: 
    // 一共有两个缓存指针,  分别存储Rcache和Fcache
    LRUcache* Rcache = nullptr;
    LFUcache* Fcache = nullptr;
    int size = 0;
    int capacity = Init_Capacity;
    int transform_time = Init_Transfrom_Time;  // 初始化为初始转变次数
    // 初始转变次数是触发消息频次达到几次时， 就加入LFU的cache
    // 默认加入的是LRU的cache,但是当触发达到一定次数，就会加入LFU的cache
};


#endif // !ARCCACHEHEADER
```

LRUfunc.cpp 

```cpp 
#include "ARCcacheHeader.h"

// put 是直接向缓存空间输入的函数: 如果次数大于3返回true, 否则返回false
bool LRUcache::put(DataType data) {
    // 对于另一个把这个空间全部占用掉的情况，此时capacity=0,不插入
    if (this->capacity == 0) return false;

    if (map.find(data)!= map.end()) { // 如果找到, 则将节点移到头部, 并将
        CacheList* L = map[data]; // 注意: 如果map[data]->time > 3, 则需要添加到LFUcache中
        L->time += 1;
        // 删除之后，在头部添加相应的节点
        DetachNode(L, false); // 从节点中移除
        Insert(L, false);     // 把这个节点放在头结点(第一个访问的节点)
        if (L->time >= transform_time) return true;  // 在LFU中加入一次
    }
    else { // map中没有找到
        CacheList* L = new CacheList(); // 新建缓存节点
        map[data] = L;  // 将节点存储在哈希表中
        if (this->size == this->capacity) {
            DeleteTail(false);
            // 删除节点，探后放进ghost中
        }
        L->key = data;
        L->time = 1;    // 使用次数为1
        Insert(L, false); // 在缓存中插入节点
    }
    return false;
}

// 使用头插方法，仅插入节点, 不考虑删除
void LRUcache::Insert(CacheList *L, bool is_ghost) {
    if (!L) return;
    if (is_ghost) {
        if (!this->ghost) {
            ghost = L;
            ghost->next = ghost;
            ghost->prev = ghost;
        }
        else {
            CacheList* q = ghost->prev;
            L->next = ghost;
            L->prev = q;
            q->next = L; 
            ghost->prev = L;
            this->ghost = L;
        }
        this->ghost_size += 1;
    }
    else { // 向主表内插入
        if (!cache) { // 对于头结点为空的情况
            cache = L;
            cache->next = cache;
            cache->prev = cache;
        }
        else {// 由于是头结点访问时间最近，所以使用头插方法, 插入完之后重新给head节点赋值
            CacheList* q = cache->prev;
            L->next = cache;
            L->prev = q;
            cache->prev = L;
            q->next = L;
            this->cache = L;
        }
        this->size += 1;
        // 插入完毕之后，头结点是最近访问的元素
    }
};

// 对于主List是让L暂时取出链表不释放指针(已经置空前后指针)，之后调用插入函数进行头插
// 而对于ghost_list是删除哈希表元素，不释放指针便于后续往主List中插入
void LRUcache::DetachNode(CacheList* L, bool is_ghost) {
    // 注意需要判断是否为ghost，虽然代码相似，但是一个是size-1, 另一个ghost_size-1
    if (!is_ghost) { // 暂时移除指针，不抹除哈希表中的元素
        if (this->size == 0) {
            throw runtime_error("nothing to detach!");
        }
        if (this->size == 1) {
            this -> cache = nullptr;
        }
        else if (L == cache) { // 暂时清除头结点
            this -> cache = L->next;
            cache->prev = L->prev;
            L->prev->next = cache;
        }
        else { // 取出中间节点
            CacheList* q = L->prev;
            q->next = L->next;
            L->next->prev = q;
        }
        L->prev = nullptr; L->next = nullptr;
        size -= 1;
    }
    else {
        this->ghost_map.erase(L->key);
        // 注意: 由于之后指针会往主list中放, 不释放指针
        if (this->ghost_size == 1) {
            this -> ghost = nullptr;
        }
        else if (L == ghost) { 
            // 删除头结点
            CacheList* p = this->ghost;
            this->ghost = L->next; // 重置
            ghost->prev = L->prev;
            L->prev->next = ghost;
        }
        else { // 连接前后节点
            CacheList* q = L->prev;
            q->next = L->next;
            L->next->prev = q;
        }
        // 重置L指针以便后续插入
        L->prev = nullptr;
        L->next = nullptr;
        L->time = 1;            // 将time重置为1
        ghost_size -= 1;
    }
}

// 删除尾部元素并删除哈希表对应元素, 如果是删除主表的元素，则加入ghost中
void LRUcache::DeleteTail(bool is_ghost) {
    if (!is_ghost) {
        CacheList* remove;
        if (!this->cache) throw("no element to delete");
        else if (this->size == 1) {
            remove = cache;
            this -> cache = nullptr; // 更新头结点
        }
        else {
            // 移除 cache的prev节点即尾结点
            remove = cache->prev;
            CacheList* q = remove->prev;
            q->next = cache;
            cache->prev = q;
        }
        // 重整remove节点
        remove->prev = nullptr; remove->next = nullptr; remove->time = 0;
        // 清除哈希表中的元素
        map.erase(remove->key);

        if (this -> ghost_size == this -> ghost_capacity) DeleteTail(true);
        // 对于ghost缓存满，则先移除元素, 再插入到ghost数组中, 不满则不移除元素
        // 然后将元素加入ghost中,显然ghost中是没有对应的项的
        this -> ghost_map[remove->key] = remove;
        Insert(remove, true);
        this->size -= 1;
    }
    else { // 删除ghost数组中的尾部并移除
        if (!this->ghost) throw("no element to delete");
        CacheList* remove;
        if (this->ghost_size == 1) { 
            // 对于ghost中有一个元素并被取出时，删除头结点
            remove = this -> ghost;
            this->ghost = nullptr;
        }
        else {
            remove = ghost->prev;
            CacheList* q = remove->prev;
            q->next = ghost;
            ghost->prev = q;
        }
        ghost_map.erase(remove->key); // 删除哈希表元素
        delete remove; // 释放存储空间
        this -> ghost_size -= 1;
    }
}

// 插入元素，并且扩容一格(注意需要判断最大空间)
void LRUcache::Add(DataType data) {
    // 这个对应的是在ghost中找到的情况，需要删除ghost中对应的节点
    this->capacity += 1;
    CacheList* L = new CacheList();
    L->key = data;
    L->time = 1;
    this->map[data] = L; // 直接塞入哈希表
    Insert(L, false);    // 插入节点 
}

// 删除最后一个节点，并且容量缩减一格, 返回是否成功缩减subtract
bool LRUcache::Subtract() {
    if (this->capacity == 0) return false;
    // 如果表满,则删除节点并放入ghost数组, 否则不用删除节点
    if (this -> size == this ->capacity) DeleteTail(false);  
    this->capacity -= 1;
    return true;
}

// 在ghost中进行查找，如果有则删除对应ghost中的元素并返回true(接下来总函数会调用put)
bool LRUcache::check_ghost(DataType data) {
    // 注意使用find而不使用ghost_map[data]; 因为这样会新增元素
    if (this->ghost_map.find(data)!= this ->ghost_map.end()) {
        DetachNode(ghost_map[data], true);
        return true;
    }
    return false;
}

void LRUcache::Show(bool show_ghost) {
    CacheList* p = this->cache;
    cout << "=============== Last recently used =========" << endl;
    if (!p) {
        cout << "----- empty list : nothing to show -----" << endl;
        return;
    }
    for (; p->next!= cache;p = p->next) {
        cout << "key : " << p->key << " , time : " << p->time << endl;
    }
    cout << "key : " << p->key << " , time : " << p->time << endl;
    if (show_ghost) {
        cout << "------------ghost is ----------------" << endl;
        CacheList* p = ghost; 
        if (!p) return;
        for (; p->next != ghost; p = p->next) {
            cout << "key : " << p->key << " , time : " << p->time << endl;
        }
        cout << "key : " << p->key << " , time : " << p->time << endl;
    }
    cout << "=============== end ====================" << endl;
}
```

LFUfunc.cpp

```cpp 
#include "ARCcacheHeader.h"

// 插入元素，其中插入的次数直接设为transform_time
void LFUcache::put(DataType data) {
    if (this->capacity == 0) return; // 不能加入元素
    if (this -> map.find(data)!= this->map.end()) {
        // 列表中有该元素
        CacheList* L = map[data];
        this -> DetachNode(L, false);
        L->time += 1;   // 增加使用次数标记
        Insert(L, false);
        cout << "after insert:" << L->time << endl;
    }
    else { // 没有找到, 考虑删除或者插入元素
        CacheList* L = new CacheList();
        if (this->size == this->capacity) {
            DeleteTail(false); // 删除一个元素并加入ghost中
        }
        L->key = data;
        L->time = this -> transform_time;
        
        cout << "putting : " << L->key << "the time is " <<  this -> transform_time << endl;
        this->map[data] = L;
        Insert(L, false);
    }
}

// 插入函数, 将节点插入表中,插入方法是有序插入
void LFUcache::Insert(CacheList* L, bool is_ghost) {
    if (!is_ghost) {
        // 有序插入算法
        if (!cache) { // 空则建立结点
            this -> cache = L;
            cache->next = cache;
            cache->prev = cache;
        }
        else if (cache->time < L->time) { // 插入到头结点上
            CacheList* p = cache->prev;
            L->next = cache;
            L->prev = p;
            p->next = L;
            cache->prev = L;
            this->cache = L; // 更新头结点
        }
        else {
            CacheList* pre = cache;
            // 在循环链表中有序插入元素 -> 此处有不足，如果使用双哈希表可以得到O(1)的复杂度
            for (; pre->next != cache && pre ->next-> time > L->time; pre = pre->next);
            // 最后停在最末端元素上,将L插入到pre的尾部即可
            CacheList* q = pre->next;
            L->prev = pre;
            L->next = q;
            pre->next = L;
            q->prev = L;
        }
        this->size += 1;
    }
    else{ // 将节点插入ghost
        if (!ghost) {
            this->ghost = L;
            ghost->next = ghost;
            ghost->prev = ghost;
        }
        else if (ghost->time < L->time) {
            CacheList *p = ghost->prev;
            L->next = ghost;
            L->prev = p;
            p->next = L;
            ghost->prev = L;
            this->ghost = L;  // 更新头结点
        }
        else {
            CacheList* pre = ghost;
            // 在循环链表中有序插入元素
            for (; pre->next != ghost && pre->next->time > L->time; pre = pre->next);
            // 将L插入到pre的尾部即可
            CacheList* q = pre->next;
            L->prev = pre;
            L->next = q;
            pre->next = L;
            q->prev = L;
        }
        this->ghost_size += 1;
    }
}

// 添加元素并且增加空间
void LFUcache::Add(DataType data) {
    this->capacity += 1;
    CacheList* L = new CacheList();
    L->key = data;
    L->time = 1;  // 此时调用的次数设为1
    this->map[data] = L;
    Insert(L, false);
}

// 删除尾元素
void LFUcache::DeleteTail(bool is_ghost) {
    if (!is_ghost) {
        CacheList* remove;
        if (!this->cache) throw("no element to delete");
        // 删除节点，哈希值并将其移动到ghost表中
        else if (this->size == 1) { //删除头结点
            remove = cache;
            this->cache = nullptr;
        }
        else { // 移除尾结点
            remove = cache->prev;
            CacheList* q = remove->prev;
            q->next = cache;
            cache->prev = q;
        }
        remove->prev = nullptr; remove->next = nullptr; remove->time = 0;
        map.erase(remove->key);
        if (this->ghost_size == this->ghost_capacity) DeleteTail(true);
        
        this -> ghost_map[remove->key] = remove;   // 加入到ghost_map 的哈希表中
        Insert(remove, true);
        this->size -= 1;
    }
    else { // 删除ghost中的结尾元素
        if (!this->ghost) {
            throw("no element to delete");
        }
        CacheList* remove;
        if (this->ghost_size == 1) {
            remove = ghost;
            this->ghost = nullptr;
        }
        else {
            remove = ghost->prev;
            CacheList* q = remove->prev;
            q->next = ghost;
            ghost->prev = q;
        }
        ghost_map.erase(remove->key);
        delete remove;  // 释放指针空间和哈希表空间
        this->ghost_size -= 1;
    }

}

// 主表时(put调用)往其他位置有序insert, ghost表时(仅有check_ghost调用)，向主表中insert
void LFUcache::DetachNode(CacheList*L, bool is_ghost) {
    // Detach操作相同,直接复制粘贴FRUfunc的代码
    if (!is_ghost) {
        if (this->size == 0) {
            throw runtime_error("nothing to detach!");
        }
        if (this->size == 1) {
            this->cache = nullptr;
        }
        else if (L == cache) { // 暂时清除头结点
            this->cache = L->next;
            cache->prev = L->prev;
            L->prev->next = cache;
        }
        else { // 取出中间节点
            CacheList* q = L->prev;
            q->next = L->next;
            L->next->prev = q;
        }
        L->prev = nullptr; L->next = nullptr;
        size -= 1;
    }
    else {
        ghost_map.erase(L->key);
        // 注意: 由于之后指针会往主list中放, 不释放指针
        if (this->ghost_size == 1) {
            this->ghost = nullptr;
        }
        else if (L == ghost) {
            // 删除头结点
            CacheList* p = this->ghost;
            this->ghost = L->next; // 重置
            ghost->prev = L->prev;
            L->prev->next = ghost;
        }
        else { // 连接前后节点
            CacheList* q = L->prev;
            q->next = L->next;
            L->next->prev = q;
        }
        // 重置L指针以便后续插入
        L->prev = nullptr;
        L->next = nullptr;
        L->time = 1;            // 将time重置为1
        ghost_size -= 1;
    }
}

// 删除结尾元素并减少容量
bool LFUcache::Subtract() {
    if (this ->capacity == 0) return false;
    // 如果表满,则删除节点并放入ghost数组, 否则不用删除节点
    if (this ->size == this ->capacity) DeleteTail(false);
    this->capacity -= 1;
    return true;
};

// 在ghost中进行查找，如果有则删除对应ghost中的元素并返回true(接下来总函数会调用put)
bool LFUcache::check_ghost(DataType data) {
    if (ghost_map.find(data)!= ghost_map.end()) {
        this ->DetachNode(ghost_map[data], true);
        return true;
    }
    return false;
}

// 显示缓存内容
void LFUcache::Show(bool show_ghost) {
    CacheList* p = this->cache;
    cout << "=============== Last frequently used =========" << endl;
    if (!p) {
        cout << "----- empty list : nothing to show -----" << endl;
        return;
    }
    for (; p->next != cache; p = p->next) {
        cout << "key : " << p->key << " , time : " << p->time << endl;
    }
    cout << "key : " << p->key << " , time : " << p->time << endl;
    if (show_ghost) {
        cout << "------------ghost is ----------------" << endl;
        CacheList* p = ghost;
        if (!p) return;
        for (; p->next != ghost; p = p->next) {
            cout << "key : " << p->key << " , time : " << p->time << endl;
        }
        cout << "key : " << p->key << " , time : " << p->time << endl;
    }
    cout << "=============== end ====================" << endl;
}
```

ARCfunc.cpp

```cpp
#include "ARCcacheHeader.h"

// 将元素输入ARC缓存表中
void ARCcache::put(DataType data) {
    bool find = false;
    // 先在各个表的ghost中搜索
    if (this->Rcache->check_ghost(data)) { // 这个删除
        // 缩减Fcache, 如果不成功,则调用put函数
        if (this->Fcache->Subtract()) {
            this->Rcache->Add(data);  // 扩容Rcache
        }
        else { // Fcache缩容失败,此时直接加入Rcache中
            this->Rcache->put(data);
            // 进行put一次，由于在ghost列表中找到,
            // 原先数组中应该没有这个数，理论上不用if
        }
        find = true;
    }
    if (this->Fcache->check_ghost(data)) {
        if (this->Rcache->Subtract()) {
            this->Fcache->Add(data); // 扩容
        }
        else {
            this->Fcache->put(data); // 不扩容直接放
        }
        find = true;
    }

    // 如果两个的ghost中都没有对应的数, 则不改变partition(两个容量不改变)
    if (!find) {
        // 优先加入LRUcache中, 作为第一个元素
        if (this->Rcache->put(data)) this->Fcache->put(data);
        // 使用put函数向Fcache中也添加对应元素
    }
}

void ARCcache::Show(bool show_ghost) {
    cout << "%%%%%%%%%%%%%%%%%%% Begin %%%%%%%%%%%%%%%%%%%%%%" << endl;
    this->Rcache->Show(show_ghost);
    this->Fcache->Show(show_ghost);
    cout << "%%%%%%%%%%%%%%%%%%%% end %%%%%%%%%%%%%%%%%%%%%%%" << endl;
}
```

`````

