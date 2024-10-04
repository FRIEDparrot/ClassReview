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

