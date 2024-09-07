# 算法的概念
1. 输入: 有零个或者多个外部量的输入作用  
2. 输出: 有至少一个量作为输出 
3. 确定性: 算法是无歧义性的
4. 有限性: 算法的执行次数必定是有限的

常见的算法包括:
Greedy (贪心法)：最短路经
Divide & Conquer (分治法)：如二分法检索、Hanoi塔
Backtracking (回溯法)：迷宫、皇后问题
Dynamic programming (动态规划法)：优化问题
Local searching (局部搜索法)：最优化问题
Branch & limitation (分支限界法)：检索

算法的复杂理论:
1. 货郎问题 , 背包问题, 双机调度问题 
2. 问题复杂度与排序问题 
3. 调度问题和投资问题

- 调度问题和投资问题  
- 问题复杂度 `->` 排序问题  `->` 算法的伪码表示(其中时间复杂度表示函数渐进的界来反映函数之间的关系) `->` 函数渐进的界的定理 

## 一、贪心算法与调度投资问题
### (1) 调度问题
调度问题的概念: 
1. 有n项任务, 每一项任务的加工时间已知, 并且从0时刻开始陆续安排到一台机器进行工作， 并定义<mark style="background: transparent; color: red">任务的完成时间是从0到每个任务加工截止的时间</mark>。求所有任务完成时间最短的安排方案

由于应当尽可能减小每一个任务的等待时间: 
可以使用贪心方法计算: 按照**从小到大的方法进行安排** 

| 1 | 2 | 3 | 4 | 5 |
| ---- | ---- | ---- | ---- | ---- |
| 3 | 10 | 8 | 5 | 15 |

在实际安排时, 第一个做的任务的时间出现5次, 第二个
$$t(I) = \sum^{n}_{k=1} (n - k + 1) t_{ik}$$
使得此值达到最小的排列即为问题的解; 
例如, 使用贪心算法, 可以得到如下安排顺序

| 1 | 2 | 3 | 4 | 5 |
| ---- | ---- | ---- | ---- | ---- |
| 3 | 5 | 8 | 10 | 15 |

对应总时间为: (3 x  5) + (5 x 4) + (8 x 3) + (10 x 2) + 15
<mark style="background: transparent; color: red">算法的设计策略</mark> : 加工短的先做。
<mark style="background: transparent; color: red">算法的正确性</mark> : 要求算法**对于所有的实例均得到的是最优解**。

如何对上述算法的正确性证明: 只需设上面有一个相邻的逆序 $t_{i} > t_{j}, i< j$, 此时可以交换任务i,j得到调度g, 而第二种将其交换过来， 则两种方法的时间差为:
$$[t_{i} \times (n- i + 1) + t_{j}\times (n- j + 1)] - [t_{j}(n-i + 1) + t_{i}(n- j + 1)]$$
因此得到$(t_{i} - t_{j})(n - i + 1) + (t_{j} - t_{i}) (n-j + 1) = (t_{i} - t_{j})(j-i) > 0$, 即存在逆序即加工时间增加。 

需要说明的是, 对于背包问题等， 如果对于重量和价值等， 往往贪心方法无法得到最优的解。
### (2) 投资问题
共有m元钱, 共需要投资n个项目, 其效益函数为$f_{i}(x)$, 表示第i个项目投资$x$元的收益, 且$i = 1,2,3\dots n$
如何分配每个项目浅使得总的收益最大。 
![[attachments/Pasted image 20240123155028.png|500]]
<mark style="background: transparent; color: red">建模方法:</mark> 输入n个项目,m为总的钱数, 并构造n维向量 $<x_1, x_2, \dots x_n>$是第$i$个项目的钱数, 使得
$$\max \sum^{n}_{i=1} f_{i}(x_{i}) \qquad  \sum^{n}_{i=1} x_{i} = m$$
最大。 
1. 穷举算法以及动态规划思想, 每一个项目投入n资金的已知, 则总的分配方法个数可以使用插板法获取, 但是插板必须是有序插入, 因此6个槽插入3个板, 总的情况个数为:
$$C_{6}^{1} + C_{6}^{2} \times 2  + C_{6}^{3} = 56种$$
也可以考虑每一个板都不为0的情况(至少为1), 则在原先基础5万元上加上4万元, 每个至少为1万元, 此时是8个槽, 而分配4个项目是 3个板， 因此是$C_{8}^{3}=56$种

最大产生的效益是$f_{1}(1) +f_{3}(3) +f_{4}(1) = 61$为最大的收益
为了保证其具有最优的解, 方程$x_1+ x_2 + \dots  + x_n = m$的非负整数解向量$<x_1, x_2, \dots  x_n>$个数估计如下: 
首先<mark style="background: transparent; color: red">将其中的可行的解表示为0-1序列</mark>, 共有m个1和n-1个0; 其中, 此序列的第一个有$x_{1}$个分量, 第二个$x_{2}$个可行的分量, 以此类推。 例如: 取项目个数n=4,钱数m=7, 则对应<1,2,3,1>可以表示为 1011011101 序列, 即有10个数, 其中7个为1, 则得到$C_{10}^{7}$为序列个数, 而使用n,m表示为:
$$C_{m + n - 1}^{m} = \Omega ((1+ \varepsilon )^{m+n-1})$$
即为指数函数增长的复杂度。

对于排序算法及其难度估计， 参考[[📘ClassNotes/⌨️Programming/🌳C and Data Structure/C++基础知识/复习 七大排序算法|七大排序算法]], 

## 二、货郎问题与计算复杂性理论
有n个城市, 已知任意两个城市， 任意两个城市的距离均给出, 求解最短的路径。

实际算法参考[[📘ClassNotes/⌨️Programming/🌳C and Data Structure/课程笔记/第七章 图#(2) 最短路径问题|Dijkstra算法和Floyd算法]], 以及Prim算法和Kruskal算法获取最小生成树。但是往往我们可以使用bfs来解决(实际上也是比较简单的算法, 但是时间复杂度较高) 


### 补充: 0-1背包问题
有n件物品装入背包, 每一件的重量$w_i$, 价值$v_i$, 且给出背包允许装入的重量限制为 B
即使用0,1向量表示背包问题$<x_1, x_2, \dots  x_n>$, 即有:
$$\max \sum^{n}_{i=1} v_{i} x_{i}\qquad \sum^{n}_{i=1}w_{i} x_{i} \leq B $$
### 双机调度问题: 
给出n项任务, 任务i的时间为$t_i$, 此时用两台相同及其加工, 使得总的完成时间达到最小。
可以考虑，总的时间为D, 并设第一个机器的时间为$t_1$, 有$t_1 < t_2$, 显然有$t_1< D/2$, 即让$t_1$在加工时间不超过D/2的情况下尽可能大 -> 也简化成0-1背包问题, 容量为 D/2, 

上述的问题也属于NP-hard问题

算法 的研究目标:
- 问题: 建模并且寻找算法, 
- 算法: 算法的评价， 时间复杂度分析等等

## 三、算法的时间复杂度
首先: 问题一般含有若干参数, 而问题的描述包括
- 定义问题参数 
- 说明每个参数的取值范围和参数间的关系 
- 说明解满足的条件 

时间复杂度是针对基本运算, **算法所做的基本运算的次数**
其中基本运算有**比较, 加法,  乘法, 置指针, 交换**等等 
输入规模: 一般是输入串的编码长度。

求解规模为$n$的问题需要的最短时间。
常见的时间复杂度有$O(n), O(n^2), O(n!), O(x^n), O(n\log n)$等等

## 四、算法的伪码表示

即使用箭头和循环等等表示算法
赋值语句:  $\leftarrow$ 
分支: if then else 
循环 while for , repeat until 
输出 return 等等
![[attachments/Pasted image 20240130101246.png]] 

# C++ 基本语法


下面的代码演示了结构体(java - interface) 参数重载和异常抛出的使用方法: 
```cpp
#include <iostream>
#include <string>
#include <stack>
using namespace std;

// java interface method  
typedef struct Computatable {
public:
    int w;
private:
    int add(int a, int b) {
        return a + b;
    }
};

template <typename ItemType> // 可以用于class或者函数前面
ItemType dec(ItemType a, ItemType b) {
    return a - b;
}
// 类的继承 
class String2: public string{
public :
    int tag;
}

int main()
{
    string s = "Hello World!\n";
    stack<int>* p = new stack<int>(); // basic usage for stack 
    // throw new exception("Exceptions!!!"); // exception throw method 
    std::cout << dec<double>(5, 3) << endl;
    return 0;
}
```
其中类的继承可以使用public, protected 和private形式, 其中protected 成员可以被继承, 而public决定基类的public成员继承之后成为什么成员。 

初始化函数的继承如下: 
```c
#include <iostream>
#include <vector>
#include <random>

using namespace std; 

// 定义点类 
template <typename ItemType>
class point {
public:
    ItemType x;
    ItemType y;
    point(ItemType x, ItemType y){
        this->x = x;
        this->y = y;
    };
};

// 继承点类的初始化
template <typename ItemType>
class pointComp : public point<ItemType>{
public:
    int number;
    // 初始化方式
    pointComp(ItemType x, ItemType y, int _num) : point<ItemType>(x,y), number(_num){ // 直接继承点的构造方法
        
    }
};
```

如果在继承中复用方法 ，则如果方法中以父类作为参数, 则子类也可以传入到函数中作为参数。
```c
// 继承点类的初始化
template <typename ItemType>
class pointComp : public point<ItemType>{
public:
    int IDnum;
    // 初始化方式
    pointComp(ItemType x, ItemType y, int _num) : point<ItemType>(x,y), IDnum(_num){ // 直接继承点的构造方法
    }
    // can compare to Point and pointCmp
    int CmpX(point <ItemType>p) { 
        // 需要说明的是, 由于可以和父类point进行比较, 而子类也是父类
        // 因而这个比较自动包含了pointCmp的比较
        if (this->x < p.x) {
            return -1; // 
        }
        else if (this->x == p.x) {
            return 0;
        }
        return 1;
    }
    // must be the same type
    int CmpY(point <ItemType>p) {
        if (this->y < p.y) {
            return -1;
        }
        else if (this->y == p.y) {
            return 0;
        }
        return 1;
    }

};

int main() {
    int n;
    vector<point<float>> vec;
    cin >> n;
    // -10 - 10 之间生成 100 个点
    for (int i = 0; i < n; i++) {
        float x, y;
        cin >> x >> y;
        vec.push_back(point<float>(x, y));
    }
    pointComp<float> p1(0.1, 0.1, 1);
    pointComp<float> p2(0.1, 0.1, 2);

    cout << p1.CmpX(p2) << p1.CmpY(p2) << endl;

    return 0;
}
```


```cpp
// 对于相同参数的向量复制
vector<pointCmp<float>> vec2(vec.begin(),vec.end());
// 另外需要说明: 哈希表的第一个参数不能是结构体, 否则会报错, 只能第二个参数是结构体, 例如: 
unordered_map<int, pointCmp<float>> Point_Map; 
```

for 循环的特殊用法
```cpp
for (const point<float> p2 : rightps)

for each (const a in b){
}
```

set的使用: 
set是集合的简称, 即内部元素必须是互异的; 且是默认按照升序排好的。
```cpp
#include <set>

set<int> s;
s.insert(10); 
s.insert(20);
s.insert(13);
set<int>:: iterator it;  // 定义集合迭代器 
for (it = s.begin(); it ! = s.end(); it++){
	cout << *it << endl;
}
s.count() // 元素个数 
s.erase() // 删除元素
s.find(); // 寻找元素  (返回迭代器)
s.empty(); 
s.clear();
```


函数内部定义lambda表达式: 
```cpp 
float Calcu_weight(point A, point B, point C){
    // 函数内部使用lambda表达式定义函数内部函数
    auto distance2 = [](point m, point n) {
        return pow(m.x-n.x, 2) + pow(m.y - n.y,2);
    };
    return 0;
}
```