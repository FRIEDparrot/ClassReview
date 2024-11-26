## 一、分支限界法综述
### (1) 分支限界法的概念
分支限界法是一种类似于回溯法的， 在问题的解空间树上搜索问题解的算法。但是一般<mark style="background: transparent; color: red">分支限界法的目标与回溯法的求解目标不同</mark>, 即回溯法是求出解空间树中<mark style="background: transparent; color: red">满足约束条件的所有解</mark>, 分支限界法是找出满足约束条件的一个解; 或者是在满足约束条件的解中<mark style="background: transparent; color: red">找出使某一目标函数值达到极大或者极小的解</mark>。

需要注意的是, 由于求解目标不同, 分支限界法对于解空间树的搜索方式不同, 即采用<b><mark style="background: transparent; color: blue">广度优先搜索的方法或者最小耗费优先的方式, 搜索解空间树; 在扩展节点处先生成所有的子节点, 再从当前的活节点表中选择下一个扩展节点</mark></b>。同时, <mark style="background: transparent; color: red">为了有效地选择下一扩展节点, 分支限界法会从活节点中选择一个最有利的节点作为扩展节点。从而是搜索朝解空间树上具有最优解的分支进行推进, 以便尽快找出最优解。</mark>分支限界法**在离散最优化问题求解上往往具有较好的表现**。

分支限界法中，**每一个活节点仅有一次机会成为扩展节点, 而扩展时会进行出队并将其子节点中有可能产生最优解的部分入队**。之后从取出和出队即可。

> [!HINT] 优先队列的概念
> 优先队列中的元素按照从大到小的方式进行排序, 每次插入元素时, 都会根据优先级自动排序。即都会将新的元素放在正确的位置上。
> 每一次出队时, 采用 top 获取优先级最高的元素。
> 优先队列实际上是利用树实现的, 根节点的值最大。而每次取出时，从树中将根节点和某个叶子节点交换, 再取出叶子节点并且重新排序 

最常见的分支限界法有以下两种方式: 
1. 队列式 (FIFO) 分支限界法: 即将活节点表组织成队列, 并按照队列规则选取节点
2. 优先队列式分支限界法: (优先队列参考[C++优先队列的使用](https://blog.csdn.net/m0_75215937/article/details/132418111)) 将活节点组织成一个优先队列，并从优先队列中**选取优先级最高的下一个节点称为扩展节点**。<mark style="background: transparent; color: red">其余的变为死节点以节省时间空间</mark>

> [!NOTE] 说明
> 实际上应当根据问题情况,选用最大优先队列或者最小优先队列。由于我们往往求解的是可扩展的问题（类似于最大值），因此如果可扩展, 则先扩展完所有的子节点, 然后去除头结点。而当**没有可以扩展的节点时**, **则将其记为一个可行的叶节点。计算其结果并与最优的结果比较**, <b><mark style="background: transparent; color: blue">然后删除这个节点</mark></b>

我们以一个背包问题说明分支限界法的思想: 设背包容量为 25, 则如下图所示:
![[Excalidraw/第六章 分支限界法(BFS) 2024-09-28 10.39.36|600]]
需要说明的是，分支限界法和广度优先搜索极其类似, **唯一的不同是不搜索以不可行节点为根节点的子树**。
一般地，例如旅行商等费用最小问题可以采用最小堆的分支限界法进行解决。

### (2) 关键思想
1. <b><mark style="background: transparent; color: blue">问题基本入手的方向: 子集树建模和排列树建模方法</mark></b>
2. 根据实际问题**确定采用最大堆或者最小堆的结构**。
3. 采用<mark style="background: transparent; color: red">每次记录上一次的最佳搜索节点</mark>, 用于回溯最终获得最优解。
4. 专门针对集装箱类问题的<mark style="background: transparent; color: red">子集树问题</mark>处理方案，以**最可能获得最大**进行优先，此时一旦找到根, 则必定为最优解。
5. 在遍历子集树时，通过树结构记录最优的步骤, 从而最小化遍历解空间所需记录的路径数量。

> [!caution] 关键:优先队列的搜索思想
> 在许多情况下, 使用优先队列进行分支限界方法进行搜索, **一旦搜索找到合适的路径等等，则必定得到的结果就是最优解**，此时可以较好地减少搜索的复杂度。

> [!caution] 关键:限界函数的选取
> 限界函数的选取也是分支限界法中的核心思想之一, 除了能够以当前找到的最优值进行限界以外, 也可以采用其他的限界函数构造方式

## 二、相关问题
### (1) 单源最短路径问题
Dijkstra 算法和 Floyd 算法参考[[📘ClassNotes/⌨️Programming/🌳Data Structure & Algorithms/🛰️Algorithms/第四章 贪心算法#(4) 图的最小生成树和最短路径问题|第四章 贪心算法]]
求解某个顶点 S 到顶点 t 之间的最短路径:
![[attachments/Pasted image 20240928150821.png]]
具体节点编号如下， 求解从 s 到 t 之间的最短路径:
![[Excalidraw/第六章 分支限界法(BFS) 2024-09-28 11.43.11]]
 搜索过程中, 首先**创建一个最小堆(优先队列)**，用于表示活节点优先队列, 其中length越小,优先级越高. 
  5
 如图的单源最短路径问题中，剪枝方法主要有两个:
 1. 根据最短的路长进行剪枝 : 即如果当前的路长小于已经搜索到的最长路长, 则将当前路径剪去。
 2. 根据节点的控制关系进行剪枝 : 每次到过一个节点, 则可以利用节点本身的空间, 计算出到达该节点的最小路长, 如果另外一条分支到达该节点的路长大于这个值, 则剪去该路长。
 3. 对于某些情况, 存在**某个节点先搜索到, 但是后搜索出比其原先路径更短的路径的情况**, 此时<mark style="background: transparent; color: red">如果继续按照这个广度优先搜索, 则必将出现重复节点的情况</mark>。此时需要做的，方法一是停止以前这个树的节点的广度优先搜索, 然后重新搜索一次。 方法二是按照搜索出的路径重新更新每一个节点的距离。但是**这会导致原先已经搜索过变为死节点的部分,都重新变成活节点, 重新更新**
 4. <b><mark style="background: transparent; color: blue">解决问题3的方法是利用一个数组 p (pre_nodes)记录从源到每个顶点的前驱节点，注意: 虽然记录了前驱节点， 但是注意这是用于构造最短路径的, 而不能够解决死结点重新更新的问题。实际上还是需要继续搜索的</mark></b>。
 5. 实际上只需一个数组记录原点到每个节点的最短距离和前驱节点即可。
 6. 对于实际的求解中, 实际上**完全可以重复加节点， 这是由于最小优先队列的性质得到的**。由于优先队列性质，必将先搜索完距离小的更短的节点以更新完毕子节点的距离，而前面即使添加过重复节点，在此之后也就会立刻被自动剪掉了。

`````ad-note
title: C++中最大堆和最小堆的创建
collapse: open
- C++中使用include<queue>中的 `prior_queue` 类。
- `prior_queue`在创建初始时默认是最大堆, 即有: `prior_queue<int> a` 默认创建最大堆。
- 可以通过指定比较函数，创建最小堆(即重设比较`less`函数为`std::greater`): `std::priority_queue<int, std::vector<int>, std::greater<int>> nodes;`
`````

下面代码求解了0到达每个点的最短距离:
```cpp title:分支限界法求解单源最短距离
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

#define MAX_DISTANCE  100000

using namespace std;

const int n = 11;

int neibhor_matrix[n][n] = {
    {0, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 7, 2, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 9, 2, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 3, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 5, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

int dist[n];   /* 记录到每个节点的最短距离 */
int pre[n];    /* 记录每个节点的前驱顶点 (用于构造最短路径) */
int best_dis;        /* record the best distance choice */

/* 默认采用最大堆进行存储, 而我们采用最小堆进行存储, 所以需要重载比较函数 */
std::priority_queue<int, std::vector<int>, std::greater<int>> nodes;    

/* 采用分支限界法求解顶点 0 到每个节点的最短路径 */
void branch_boundary() {
    dist[0] = 0;
    for (int i = 1; i < n; i++) dist[i] = MAX_DISTANCE;
    memset(pre, 0, sizeof(pre));
    while (!nodes.empty()) nodes.pop();
    nodes.push(0);    /* 取源节点为初始扩展节点 */
    while (!nodes.empty()) {
        int idx = nodes.top();
        nodes.pop();
        for (int i = 0; i < n; i++) {
            if (neibhor_matrix[idx][i] != 0 && dist[idx] + neibhor_matrix[idx][i] < dist[i]){
                dist[i] = dist[idx] + neibhor_matrix[idx][i];
                pre[i] = idx;
                nodes.push(i);   /*  直接push 和记录前驱节点即可 */
            }
        }
    }
}

int main()
{
    branch_boundary();
    for (int i = 0; i < n; i++) {
        cout << dist[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < n; i++) {
        int p = i;
        do {
            cout << p << "<-";
            p = pre[p];
        } while (p != 0);
        cout << "0" << endl;
    }
    return 0;
}
```

输出结果如下:
```cpp
0 2 3 4 9 4 5 12 7 6 8
0<-0
1<-0
2<-0
3<-0
4<-1<-0
5<-1<-0
6<-2<-0
7<-4<-1<-0
8<-5<-1<-0
9<-6<-2<-0
10<-9<-6<-2<-0
```

### (2) 装载问题
装载问题参考[[📘ClassNotes/⌨️Programming/🌳Data Structure & Algorithms/🛰️Algorithms/第五章 回溯法(DFS)#(1) 装载问题|第五章 回溯法(DFS)]], 实际上是一个子集树。
装载问题的分支限界法关键是剪枝机制 :
1. 当前所装载的部分大于 capacity 时, 进行剪枝
2. 采用的优先队列用于存储最大载重量, 每次仅需记录一个 layer, 然后每次把 layer 递增, 并入队所有的活节点即可。
3. 由于只有装和不装之分, 所以装的选择一定会达到最后, 因此可以直接剪去 

注意：如果采用优先队列进行, 会由于小端插入, 导致小端的 0 在前， push 完毕之后, 由于是小端存储, 则较小的总是在最前端。从而每次 top 取出结果都是0, 因此可以直接采取队列的方式。具体代码如下:
```cpp 
#include <iostream>
#include <vector>
#include <queue>
using namespace std;
int times = 0;
int max_loading(int n, int* w, int capacity) {
    int max_load = 0;
    int layer = 0;
    /* 建立 pq 是用来存储载重量的优先队列(小端) */
    queue<int> q;
    q.push(0);   /* 当前的选择是 capacity = 0 */
    for (layer = 0; layer < n; layer++) {
        int s = q.size();
        if (layer == n - 1) {
            for (int i = 0; i < s; i++) {
                times++;
                int cap = q.front();
                q.pop();
                int c = cap + w[layer] <= capacity ? cap + w[layer] : cap;
                max_load = max(c, max_load);
            }
        }
        else {
            for (int i = 0; i < s; i++) {
                int cap = q.front();
                q.pop();
                times++;
                if (cap + w[layer] <= capacity) {
                    q.push(cap + w[layer]);  /* load w[layer] situation */
                    q.push(cap);             /* not load w[layer] situation */
                }
                else {
                    q.push(cap);   /* not load w[layer] situation */
                }
            }
        }        
    }
    return max_load;
}

int main() {
    int n = 10;
    int w[10] = { 2, 5, 9, 4, 2, 4, 3, 1, 2, 3 };
    int capacity = 20;

    int max_load = max_loading(n, w, capacity);
    cout << "The maximum loading is: " << max_load << endl;
    cout << "total searched times : " << times << endl;
    return 0;
}
```

可以看出，实际上上述算法没有减少多少计算过程的复杂度(尝试次数高达800)，仅仅是对多出的部分进行了剪枝。
除了采用**剩余集装箱重量**剪枝(加上剩余也不够)以外, 也可以在搜索子树的过程中保存当前构造的子集树的路径。

`````ad-note
title: 基于优先队列的优化
collapse: open
在优先队列中, 队列的优先级可以定义为**从根节点到节点  $x$ 的路径相应的载重量加上剩余集装箱的重量**, 然后从优先级最大的活节点中，选择下一个扩展节点。
注意：以节点 $x$ 为根节点的 子树中，所有节点路径载重量不超过 `x.totalweight` 注意: 此时在优先队列式分支限界法中， **一旦有一个叶节点成为当前扩展节点(n+1层节点)，则可以断言该叶节点相应的解即为最优解**， 否则没有对应的最优解。 

同时, 我们将最优解的可行路径由子集树中从该节点开始指向父节点的指针逐步进行构造。
`````

首先，实际装载时, 最优解总是从"装入"的分支产生的，因而我们检查最优解<mark style="background: transparent; color: red">仅需要比较"装入"后的重量大小和最优解即可。</mark>

> [!caution] 构造方式
> 在所有的选择中, 可以以<b><mark style="background: transparent; color: red">该情况的载重量 + 剩余重量</mark></b>作为权重，这是由于, 我们希望先尝试"有可能达到最大载重量"的部分进行优先尝试。<b><mark style="background: transparent; color: blue">由于该情况载重 + 剩余重量 小于 最大重量时</mark></b>, 显然是得不到最优解的，而一旦所有的都搜索完毕，显然得到的就是最优解。而装不满的情况必然出现另外能装满情况载重 + 剩余重量 > 该点载重 + 剩余重量。 由于这样搜索必将得到最优解的性质，我们甚至不需要记录和比较相应节点的重量和最大重量比较;
> 
> 需要说明的是，得到最优值时，我们不知道最优的重量是哪几件货组成的，因此我们在搜索过程中**方法一是每个解保留一个数组，即保留根节到每个活节点的路径**, 但是这样内存开销比较大；方法二是保留构造的解空间树。即<mark style="background: transparent; color: red">只记录父节点的方法, 然后向根节点回溯得到解的组成。</mark>

根据上述思路，优化之后的装载问题算法代码如下:  
```cpp title:装载问题算法代码
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int times = 0;

typedef struct cargo{
    char id;
    int weight;
}cargo;

typedef struct node {
    node* parent;
    cargo* cur;       /* 当前是否选择该货物, 不选则为 nullptr */
    int layer;        /* 当前层数 */ 
    int weight;       /* 当前选择的重量值 */
    int weight_left;  /* 剩余重量值 */
    node(node* p, cargo* c, int layer, int w, int w_left){
        this->parent = p;
        this->cur = c;
        this->layer  = layer;
        this->weight = w;
        this->weight_left = w_left;
    }
}node;

/* 比较相应重量值 + 剩余重量值, 大的优先, 所以采用 < */
struct cmp_node{
    bool operator()(const node *a, const node *b) const {
        return a->weight + a->weight_left < b->weight + b->weight_left;
    }
};

node* max_loading(int n, cargo cargo_arr[], int capacity) {
    /* 按照重量递减排序货物便于快速搜索出需要的解 */
    cargo* cargos_new = new cargo[n];
    memcpy(cargos_new, cargo_arr, sizeof(cargo) * n);
    std::sort(cargos_new, cargos_new + n, [](cargo a, cargo b) {return a.weight > b.weight; }); 
    
    int max_load = 0;

    vector<int> left_weight_arr(n);  /* 按照层计算选择每个 cargo 后的剩余质量 */
    int tmp = 0;
    for (int i = n - 1; i >= 0; i--) {
        tmp += cargos_new[i].weight;
        left_weight_arr[i] = tmp;
    }

    priority_queue<node*, std::vector<node*>, cmp_node> nodes;
    node* root = new node(nullptr, nullptr, -1, 0, left_weight_arr[0]);    /* 创建根节点, 不加入任何节点 (layer为-1) */
    nodes.push(root);

    node *best_node = nullptr;

    while (!nodes.empty()) {
        node* cur_node = nodes.top();
        nodes.pop();
        times++;
        int layer = cur_node->layer + 1;
        if (layer == n - 1) { /* 出现 n 级节点, 即找到最优解 */ 
            best_node = cur_node;
            break;
        }
        if (cur_node->weight + left_weight_arr[layer] < max_load) continue;    /* 用于剪枝, 去掉不可能节点 */
        if (cur_node->weight + cargos_new[layer].weight <= capacity) {
            node* new_node1 = new node(cur_node, &cargos_new[layer], layer , cur_node->weight + cargos_new[layer].weight, left_weight_arr[layer + 1]);
            nodes.push(new_node1);
            max_load = max(max_load, new_node1->weight);
        }
        node* new_node2 = new node(cur_node, nullptr, layer, cur_node->weight, left_weight_arr[layer + 1]);
        nodes.push(new_node2);
    }
    return best_node;
}

void show_cargo_node(node* root) {
    if (!root) {
        cout << "no solution node to show" << endl;
        return;
    }
    cout << "--------- show cargo node of layer : " << root->layer << " ----------------" << endl;
    cout << "weight of this layer: " << root->weight << endl;
    node* cur = root;
    while (cur && cur ->layer!= -1) {
        if (cur->cur != nullptr) {
            cout << "id " << cur->cur->id << ", weight : " << cur->cur->weight << endl;
        }
        cur = cur->parent;
    }
}

int main() {
    int n = 10;
    int w[10] = { 2, 5, 9, 4, 2, 4, 3, 1, 2, 3 };
    // int w[10] = { 14, 10, 11, 15, 7, 4, 7, 4, 7, 8 };
    int capacity = 20;
    /* construct the cargo array */
    cargo cargos[10]; 
    for (int i = 0; i < n; i++) {
        cargos[i].id = 'A' + i;
        cargos[i].weight = w[i]; 
    }

    node* best_load = max_loading(n, cargos, capacity);
    show_cargo_node(best_load);
    cout << "total searched times : " << times << endl;
    return 0;
}
```

上述代码执行结果是:
```cpp
--------- show cargo node of layer : 8 ----------------
weight of this layer: 20
id E, weight : 2
id A, weight : 2
id J, weight : 3
id F, weight : 4
id D, weight : 4
id B, weight : 5
total searched times : 109
```
仅用了 109 次解空间搜索, 可以看出大大提高了解空间的搜索效率。

### (3) 布线问题
布线问题实际上是按照步数进行广度优先搜索的问题。
首先在一个 $n \times n$ 的方格上, 其中不可布线区域采用 1 标记, 而确定从点  $A(x_1, y_1)$ 到  $B(x_2, y_2)$ 的最短布线。

取如下的矩阵, 其中红色部分为不可布线区域; 
![[Excalidraw/第六章 分支限界法(BFS) 2024-10-01 21.32.10|350]]


> [!caution] 思想
>  需要说明的是, 在带有重复搜索问题的情况下,一个重要方法是采用一个 steps 数组, 记录每个节点的最短搜索到的步长, 同时会根据路径短进选择权重(例如最短路径则建立最小优先队列), 这样就可以避免重复搜索（减少搜索次数）, 同时按照短的方式进行搜索
>  

问题求解代码较为简单，具体如下:
```cpp
#include<iostream> 
#include<algorithm>
#include<queue> 
#include<vector>

using namespace std;
const int n = 7;

int mat[n][n] = {
        {0, 0, 1, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 1, 0, 0},
        {1, 0, 0, 0, 1, 0, 0},
        {1, 1, 1, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 0}
};
int steps[n][n];
int best_option[n][n];
const int fst[4] = { -1, 1, 0, 0 };
const int scd[4] = { 0, 0, -1, 1 };

/* 将各个节点中, 距离小的进行前置 */
struct steps_cmp {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) {
        return mat[a.first][a.second] > mat[b.first][b.second];
    }
};

/*  */
void bfs(int start_x, int start_y, int end_x, int end_y) { 

    // if (steps[i][j] > -1 && step > steps[i][j]) return;
    
    std::priority_queue<std::pair<int,int>, vector<std::pair<int, int>>, steps_cmp> q;
    q.push(std::make_pair(start_x, start_y));
    steps[start_x][start_y] = 0;

    /* boarden search */
    while (!q.empty()) {
        std::pair<int, int> node = q.top();
        q.pop();
        int xo = node.first;
        int yo = node.second;
        if (xo == end_x && yo == end_y) return;
        for (int i = 0; i < 4; i++) {
            int x = xo + fst[i];
            int y = yo + scd[i];
            if (x >= 0 && x < n && y >= 0 && y < n && mat[x][y] == 0) {
                if (steps[x][y] > -1 && steps[xo][yo] + 1 >= steps[x][y]) {
                    continue;
                }
                else {
                    steps[x][y] = steps[xo][yo] + 1;
                    best_option[x][y] = i;
                    q.push(std::make_pair(x, y));
                }
            }
        }
    }
}

/* 电路布线问题 */
int main() {
    memset(steps, 0, sizeof(steps));
    memset(best_option, 0, sizeof(best_option));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            steps[i][j] = -1;
            best_option[i][j] = -1;
        }
    }
    int start_x = 2, start_y = 1;
    int end_x = 3, end_y = 5;

    bfs(start_x, start_y, end_x, end_y );
    cout << "minimum steps : " << steps[end_x][end_y] << endl;
    int x = end_x, y = end_y;
    if (steps[end_x][end_y] != -1) {
        while (x != start_x || y != start_y) {
            cout << "(" << x << "," << y << ")<-";
            int idx = best_option[x][y];
            if (idx < 0) break;
            x = x - fst[idx];
            y = y - scd[idx];
        }
        cout << x << "," << y << endl;
    }
    return 0;
}
```

### (4) 最大团问题
问题参考[[📘ClassNotes/⌨️Programming/🌳Data Structure & Algorithms/🛰️Algorithms/第五章 回溯法(DFS)#(5) 最大团问题|第五章 回溯法(DFS)]] 的相同问题, 采用分支限界方法求解最大团问题。<mark style="background: transparent; color: red">采用子集树的方法</mark>对最大团问题进行遍历和求解。因此实际上是类似于集装箱问题, 可按照该问题的思路进行求解。

![[Excalidraw/第五章 回溯法(DFS) 2024-09-23 19.37.58|500]]

```cpp
int adjacency_matrix[7][7] = {
    {0, 1, 1, 1, 0, 0, 1}, // connection for node 1
    {1, 0, 0, 0, 1, 0, 1}, // connection for node 2
    {1, 0, 0, 1, 0, 0, 1}, // connection for node 3
    {1, 0, 1, 0, 1, 0, 1}, // connection for node 4
    {0, 1, 0, 1, 0, 1, 1}, // connection for node 5
    {0, 0, 0, 0, 1, 0, 1}, // connection for node 6
    {1, 1, 1, 1, 1, 1, 0}  // connection for node 7
};
```

<b><mark style="background: transparent; color: blue">需要注意</mark></b>: 我们不采用每一个向量存储子集树遍历过程中的选择数组, 而是直接采用树结构进行存储, 这样能够显著减少实际的存储空间;

具体代码如下所示:
```cpp
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std; 

const int n = 7;

/* 求解最大团问题的分支限界方法 */
int adjacency_matrix[n][n] = {
    {0, 1, 1, 1, 0, 0, 1}, // connection for node 1
    {1, 0, 0, 0, 1, 0, 1}, // connection for node 2
    {1, 0, 0, 1, 0, 0, 1}, // connection for node 3
    {1, 0, 1, 0, 1, 0, 1}, // connection for node 4
    {0, 1, 0, 1, 0, 1, 1}, // connection for node 5
    {0, 0, 0, 0, 1, 0, 1}, // connection for node 6
    {1, 1, 1, 1, 1, 1, 0}  // connection for node 7
};

/* 采用树结构进行存储, 这个是为了避免每个解存储一个向量而增加复杂度 */
typedef struct cnode {
    cnode* parent;
    int cur_idx;   /* 当前搜索到的层 */
    int elem;           /* 当前搜索到的为 0 或者 1 (其中1为存在集合中) */
    int size;           /* 当前得到的最大团大小 */
    cnode(cnode* parent, int cur_idx, int elem, int size)  {
        this->parent = parent;
        this->cur_idx = cur_idx;
        this->elem = elem;
        this->size = size;
    }
}cnode;

struct cmp {
    bool operator()(const cnode *a, const cnode *b){
        /* int idx = *max_element(vec.begin(), vec.end()) + 1; */
        return a->size + (n-1) - a->cur_idx < b->size + (n-1) - b->cur_idx;     /* 按照当前团大小 + 剩余元素个数排序, 建立团 */
    }
};

/* 求解最大团问题的分支限界方法 */
cnode* get_max_clique() {
    cnode* root = new cnode(NULL, -1, 0, 0);
    priority_queue<cnode*, vector<cnode*>, cmp> pq;
    pq.push(root);
    
    while (true) {
        cnode* node = pq.top();
        pq.pop();

        /* check if the node can be added into the max clique */
        int new_idx = node->cur_idx + 1;
        if (new_idx == n) return node;   /*  reach the last layer */

        bool flag = true;
        for (cnode* c = node; c->parent != NULL; c = c->parent) {
            /* 检查团兼容性 */
            if (c->elem == 1 && !adjacency_matrix[c->cur_idx][new_idx]) {
                flag = false;
            }
        }
        cnode *new_node1 = new cnode(node, new_idx, 0, node->size);
        pq.push(new_node1);

        if (flag) {
            cnode *new_node2 = new cnode(node, new_idx, 1, node->size + 1);
            pq.push(new_node2);
        }
    }
    return root;
}

int main() {
    cnode* node = get_max_clique();
    while (node->parent != NULL) {
        if (node->elem) {
            cout << node->cur_idx << " ";
        }
        node = node->parent;
    }
    return 0;
}
```

### (5) 旅行商问题
对于旅行商问题的分支限界方法, 实际上是排列树的解空间搜索问题。一般会以最短距离为权重建立优先队列。

对于排列树, 一般也有两种不的实现方式, 一种是**仅采用优先队列存储活节点**, 另一种是采用优先队列存储活节点, 同时存储当前构造出的部分排列树，此时容易通过回溯获取到解的形成方式。

首先, 我们借助 python 产生范围内的需要的城市的点: 生成 20 个城市, 位置如下: 
![[attachments/Pasted image 20241006000007.png|400]]

```python
import numpy as np  
import matplotlib.pyplot as plt  
  
x_range = np.array([0, 100], dtype=np.float32)  
y_range = np.array([0, 100], dtype=np.float32)  
  
city_num = 20  
citys = np.array([[np.random.rand() * (x_range[1] - x_range[0]) + x_range[0]  for i in range(city_num)],  
                 [np.random.rand() * (y_range[1] - y_range[0]) + y_range[0]  for i in range(city_num)]], dtype = np.float32)  
dist_mat = np.zeros((city_num, city_num), dtype=np.float32)  
for i in range(city_num):  
    for j in range(city_num):  
        dist = np.sqrt((citys[0][i] - citys[0][j])**2 + (citys[1][i] - citys[1][j])**2)  
        dist_mat[i][j] = dist  
for i in range(city_num):  
    print("{", end='')  
    for j in range(city_num):  
        print(f"{dist_mat[i][j]:.2f},",end='')  
    print("}")  
  
fig = plt.figure()  
ax = fig.add_subplot(111)  
ax.scatter(citys[0], citys[1], color='red')  
for i in range(citys.shape[1]):  
    city = citys[:, i]  
    ax.text(city[0], city[1]-0.01, f'{i+1}', fontsize=12, ha='center', va='bottom')  
plt.show()
```

此时, 形成排列树, 并采用分支限界方法进行搜索, 而<mark style="background: transparent; color: red">对于排列树的搜索的分支限界算法</mark>, 关键是采用何种方法进行优先搜索。
如果我们以当前的距离为权重建立小根堆, <mark style="background: transparent; color: red">当有满节点的情况被弹出时, 则这种情况一定是最优的。</mark>因为**假设有其他解比当前解的距离小, 则加成满节点的情况后, 距离一定更小且是排在前面的**, 显然会比当前距离更长的先弹出。

此外, 还有更多基于此的优化方法 : 
1. 首先, 针对每个顶点找出每个顶点的最小距离出边，并采用 $ldist$ 进行记录, 而 rdist 记录 $x[s:n-1]$  中的<mark style="background: transparent; color: red">顶点最小出边距离和</mark>。
2. 如果出现某个顶点没有出边, 则<mark style="background: transparent; color: red">算法即刻结束, 显然找不到回路</mark>

```cpp
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

const int n = 20;

/* 定义 A 的邻接矩阵 */
float Dist[n][n] = {
{0.00,71.65,62.79,43.82,30.01,46.88,39.27,73.31,32.98,30.35,84.60,40.74,66.95,38.10,44.93,69.53,50.89,34.97,29.19,52.60,},
{71.65,0.00,35.36,34.45,41.77,79.43,76.31,81.59,53.78,49.49,14.49,32.70,87.82,79.78,80.09,11.70,25.59,74.53,97.95,35.68,},
{62.79,35.36,0.00,48.56,41.32,49.90,49.28,46.41,32.28,56.28,48.25,41.66,105.08,87.10,51.31,24.16,19.22,81.82,91.98,56.67,},
{43.82,34.45,48.56,0.00,17.39,70.57,64.70,84.99,42.85,15.84,44.21,7.67,57.15,45.36,70.07,38.59,29.34,40.08,66.12,10.83,},
{30.01,41.77,41.32,17.39,0.00,53.58,47.47,70.42,26.42,15.57,54.60,11.60,65.62,45.79,52.94,40.94,24.31,40.50,56.54,28.12,},
{46.88,79.43,49.90,70.57,53.58,0.00,7.61,29.46,28.02,65.82,93.84,63.47,112.12,84.60,2.61,70.76,54.08,80.87,69.13,81.40,},
{39.27,76.31,49.28,64.70,47.47,7.61,0.00,36.07,23.15,59.00,90.79,57.86,104.69,77.02,5.87,68.43,50.73,73.34,61.98,75.52,},
{73.31,81.59,46.41,84.99,70.42,29.46,36.07,0.00,44.94,85.19,94.66,77.32,134.51,109.26,32.05,70.57,60.05,104.90,97.95,95.25,},
{32.98,53.78,32.28,42.85,26.42,28.02,23.15,44.94,0.00,40.42,68.25,35.58,89.58,65.20,27.88,46.96,28.28,60.55,61.65,53.65,},
{30.35,49.49,56.28,15.84,15.57,65.82,59.00,85.19,40.42,0.00,59.98,17.30,50.08,31.68,64.77,51.83,38.12,26.28,50.38,22.58,},
{84.60,14.49,48.25,44.21,54.60,93.84,90.79,94.66,68.25,59.98,0.00,44.38,92.07,88.40,94.53,24.14,40.08,83.37,109.76,42.12,},
{40.74,32.70,41.66,7.67,11.60,63.47,57.86,77.32,35.58,17.30,44.38,0.00,63.49,48.78,63.10,34.55,22.55,43.39,65.47,18.18,},
{66.95,87.82,105.08,57.15,65.62,112.12,104.69,134.51,89.58,50.08,92.07,63.49,0.00,29.95,110.55,94.74,86.04,32.03,63.75,52.27,},
{38.10,79.78,87.10,45.36,45.79,84.60,77.02,109.26,65.20,31.68,88.40,48.78,29.95,0.00,82.79,83.28,69.59,5.40,35.45,46.57,},
{44.93,80.09,51.31,70.07,52.94,2.61,5.87,32.05,27.88,64.77,94.53,63.10,110.55,82.79,0.00,71.68,54.62,79.16,66.70,80.90,},
{69.53,11.70,24.16,38.59,40.94,70.76,68.43,70.57,46.96,51.83,24.14,34.55,94.74,83.28,71.68,0.00,19.32,77.90,97.34,42.68,},
{50.89,25.59,19.22,29.34,24.31,54.08,50.73,60.05,28.28,38.12,40.08,22.55,86.04,69.59,54.62,19.32,0.00,64.22,79.34,37.69,},
{34.97,74.53,81.82,40.08,40.50,80.87,73.34,104.90,60.55,26.28,83.37,43.39,32.03,5.40,79.16,77.90,64.22,0.00,36.57,41.73,},
{29.19,97.95,91.98,66.12,56.54,69.13,61.98,97.95,61.65,50.38,109.76,65.47,63.75,35.45,66.70,97.34,79.34,36.57,0.00,72.16,},
{52.60,35.68,56.67,10.83,28.12,81.40,75.52,95.25,53.65,22.58,42.12,18.18,52.27,46.57,80.90,42.68,37.69,41.73,72.16,0.00,},
};

typedef struct tree_node {
    float cur_dist;
    int cur_idx;    /* stop when cur_idx = 0 */
    tree_node* parent;
    //vector<int> node_ava_nxt;    vector<int> node_ava_nxt

    tree_node(float cur_dist, int cur_idx, tree_node* parent) {
        this->cur_dist = cur_dist;
        this->cur_idx = cur_idx;
        this->parent = parent;
        // this->node_ava_nxt = node_ava_nxt;
    } 
}tree_node;


/* sort each dist in node and return the index needed */
void get_sorted_idx(int n, int** sorted_idx) {
    for (int i = 0; i < n; i++) {
        std::vector<std::pair<float, int>>  distances;
        for (int j = 0; j < n; j++) {
            distances.push_back(std::make_pair(Dist[i][j], j));
        }
        std::sort(distances.begin(), distances.end());
        for (int j = 0; j < n; j++) {
            sorted_idx[i][j] = distances[j].second;
        }
    }
}

vector<int> get_available_nodes(tree_node* cur_node, int** sorted_idx, int n) {
    vector<int> ava_nodes;
    for (int i = 0; i < n; i++) {
        ava_nodes.push_back(sorted_idx[cur_node->cur_idx][i]);
    }
    tree_node* t = cur_node;
    while (t!= NULL) {
        auto it = std::find(ava_nodes.begin(), ava_nodes.end(), t->cur_idx);
        if (it != ava_nodes.end()) {
            ava_nodes.erase(it);
        }
        t = t->parent;
    }
    return ava_nodes;
}

struct cmp {
    // sort from small to large 
    bool operator()(tree_node* a, tree_node* b) {
        return a->cur_dist > b->cur_dist;
    }
};

tree_node* traveling_problem(int n) {
    int** sorted_idx = new int*[n];
    for (int i = 0; i < n; i++) 
        sorted_idx[i] = new int[n];
    get_sorted_idx(n, sorted_idx);

    tree_node* root = new tree_node(0, 0, NULL);   /* start from node 0 */
    priority_queue<tree_node*, vector<tree_node*>, cmp> pq;
    pq.push(root);
    
    while (!pq.empty()) {
        tree_node* cur_node = pq.top();
        pq.pop();
        if (cur_node->cur_idx == 0 && cur_node->parent != NULL) {
            return cur_node;
        }
        /* note : if we need to search all the path, must add "else" after remove "return" above */
        vector<int> ava_nodes = get_available_nodes(cur_node, sorted_idx, n);
        if (ava_nodes.size() == 0) {
            float new_dist = cur_node->cur_dist + Dist[cur_node->cur_idx][0];   /* return to 0 to form a loop */
            tree_node* new_node = new tree_node(new_dist, 0, cur_node);
            pq.push(new_node);
        }
        else {
            for (int idx : ava_nodes) {
                float new_dist = cur_node->cur_dist + Dist[cur_node->cur_idx][idx];
                tree_node* new_node = new tree_node(new_dist, idx, cur_node);
                pq.push(new_node);
            }
        }
    }
    return NULL;
}

const int n_srh = 10; /* only calculate the search range */

int main() {
    tree_node *cur_node =  traveling_problem(n_srh);
    if (cur_node != NULL) {
        cout << "find available distance loop : " << cur_node->cur_dist << endl;
        while (cur_node != NULL) {
            cout << cur_node->cur_idx ;
            if (cur_node->parent != NULL) 
                cout << "->";
            cur_node = cur_node->parent;
        } 
    }
    else {
        cout << "no available loop" << endl;
    }
    return 0;
}
```

在设定规模为10时,找到的回路为 0->8->6->5->7->2->1->3->9->4->0 (270.84)
![[attachments/Pasted image 20241006234801.png|400]]


### (6) 电路板排列问题
电路板排列问题参考[[📘ClassNotes/⌨️Programming/🌳Data Structure & Algorithms/🛰️Algorithms/第五章 回溯法(DFS)#(8) 电路板排列问题|回溯法]], 即列举全部电路板排列的问题。
显然, 当前面的排列已经确定时, 最大值就已经确定了, 因此我们从当前排列中, 按照密度最小进行从小到大建立最小堆, 而节点记录当前的最小密度即可。

回溯法中, 我们采用递归实现了排列树的遍历, 而需要注意的是 bestd(当前最优解参数)用于进行剪枝。如果当前扩展节点的 cur_dens 不小于 bestd (因为<mark style="background: transparent; color: red">我们需要考虑最后一块电路板的密度</mark>, 所以实际上和旅行商问题类似的, 我们可以采用最大的带有全部节点的排列出队时，则直接找到最优的解，而密度在最后一个节点产生时就已经确定了,可以用于剪枝)

需要说明的是, 在排列树进遍历时, <b><mark style="background: transparent; color: blue">相对于回溯法, 每个节点都要记录当前的各个板剩余的线头数量</mark></b>， 因此<mark style="background: transparent; color: red">对于电路板排列问题, 分支限界法需要更多的空间消耗</mark>. 

下面给出了一个简单示例, 没有加末尾的记录和剪枝:
```cpp title:电路板排列问题,分支限界方法
#include <iostream>
#include <array>
#include <vector>
#include <queue> 
#include <algorithm>
using namespace std;

const int n = 8;
const int m = 5;
int circuit_boards[n];       /* 电路板的排列数组 */
int best_circuit_boards[n];  /* 电路板排列数组的最佳结果 */
int wire_nums[n - 1];        /* 每个区间之间的连线数量 */
int best_wire_nums[n - 1];   /* 电路板区间之间的连线数量的最佳结果 */

// int conn_tot[m];             /* 记录每个方法总数量的全局变量 */

const std::array<std::vector<int>, m> circuit_conn = { {
    {3,4,5},
    {0, 1},
    {0, 2},
    {2, 5},
    {6, 7},
} };

int** init_conn_matrix(array<vector<int>, m> conn) {
    /* 新建一个矩阵, 存储每个连接块是否有节点, 即连接块 */
    int** conn_mat = new int* [m];   /* block m */
    for (int i = 0; i < m; i++) {
        conn_mat[i] = new int[n];    /* index n */
        /* 将所有元素设置为0 */
        for (int j = 0; j < n; j++) {
            conn_mat[i][j] = 0;
        }
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < circuit_conn[i].size(); j++) {
            conn_mat[i][circuit_conn[i][j]] = 1;
        }
    }
    return conn_mat;
}
int conn_tot[m];

typedef struct node {
    int density;
    int idx;
    node* parent;
    int size; /* 当前排列的长度 */
    int conn_left[m];
    node(int density, int idx, node* parent, int size) {
        this->density = density;
        this->idx = idx;
        this->parent = parent;
        this->size = size;
        for (int i = 0; i < m; i++) {
            conn_left[i] = circuit_conn[i].size();
        }
    }
}node;

struct cmp {
    bool operator()(const node* a,const node* b) {
        return a->density > b->density; /* 小顶堆 */
    }
};

node* circuit_board_problem(int n) {
    int** conn_matrix = init_conn_matrix(circuit_conn);
    priority_queue<node*, vector<node*>, cmp> pq;

    int min_res_find = m; /* 初始化最大结果 */ 
    /* 初始化 conn_tot 数组 */
    for (int i = 0; i < m; i++) {
        conn_tot[i] = circuit_conn[i].size();
    }

    for (int i = 0; i < n; i++) {
        node* cur_node = new node(0, i, nullptr, 1);
        for (int j = 0; j < m; j++) {
            cur_node->conn_left[j] -= conn_matrix[j][i];
        }
        pq.push(cur_node);
    }

    while (!pq.empty()) {
        node* cur_node = pq.top();
        pq.pop();
        /* 排列树遍历 -> 找到结果则最优 */
        if (cur_node->size == n) {
            return cur_node;
        }
        /* 枚举下一块板 */
        
        for (int i = 0; i < n; i++) {
            /* 从树中搜索之前的节点 */
            node* t = cur_node;
            while (t != nullptr) {
                if (t->idx == i) {
                    break;
                }
                t = t->parent;
            }
            if (t!=nullptr) continue;  /* 如果已经存在, 则跳过 */ 
            
            node* new_node = new node(0, i, cur_node, 0);  /* 复制当前节点 */
            /* 复制 parent 的 conn_left */
            for (int k = 0; k < m; k++) {
                new_node->conn_left[k] = cur_node->conn_left[k];
            }
            // memcpy(new_node->conn_left, cur_node->conn_left, m *  sizeof(int));
            
            /* 计算当前节点部分的连线数量 */
            int sum = 0;
            for (int j = 0; j < m; j++) {
                if (new_node->conn_left[j] < conn_tot[j] && new_node->conn_left[j] > 0) {
                    sum += 1;
                }
                new_node->conn_left[j] -= conn_matrix[j][i];    /* 更新当前节点 */
            }
            new_node->density = max(sum, cur_node->density);  /* 注意这里要和 cur_node-> density 进行比较, 以继承前面的 */
            new_node->size = cur_node->size + 1;
            pq.push(new_node);
        }
    }
    return nullptr;
}

int main() {
    node* node = circuit_board_problem(n);
    if (node == nullptr) {
        cout << "No solution" << endl;
    }
    else {
        cout << "Solution: " << node->density << endl;
    }
    while (node != nullptr) {
        cout << node->idx << " ";
        node = node->parent;
    }
    return 0;
}
```
结果为:
```c
6 7 4 3 5 2 0 1 
```
仍然是最优的结果(显然就此题效率不如回溯法)

### (7) 批处理作业调度问题
问题参考[[📘ClassNotes/⌨️Programming/🌳Data Structure & Algorithms/🛰️Algorithms/第五章 回溯法(DFS)#(2) 批处理作业调度问题|批处理作业调度问题]], 实际上也是排列树的遍历问题。
采用分支限界法要求得到的结果需要具有局部最优性质，而关键是<b><mark style="background: transparent; color: blue">如何确保只要搜索到叶节点, 则所得到的该结构就是最优的</mark></b>。

> [!caution] 核心
> 批处理作业调度问题的核心是<b><mark style="background: transparent; color: red">选取限界函数</mark></b>, 在本方法中, **取了每枝上的两种特殊情况, 得到该枝上的子树的完成时间和的下界**。而剪枝时, <mark style="background: transparent; color: red">只要这个下界 >= 当前找到的最优解，则进行剪枝</mark>

