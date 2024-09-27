## 一、回溯法的算法框架
### (1) 回溯法概念
回溯法一般可以系统搜索问题的所有解, 基本方法是在问题的解空间树中,  按深度优先策略, 从根节点触发搜索解空间树。

算法搜索到解空间的任一节点时, 首先判断节点是否包含问题的解。**如果必定不包含, 则跳过对该子树的搜索。并逐层向祖先节点进行回溯; 否则进入子树, 继续深度优先搜索并搜索到根节点， 直到根节点的所有子树都搜索完毕**。另外，**只需要搜索到问题的一个解，问题便可终止**。这种深度优先的方法称为回溯法。

回溯法解决问题时, 应当**明确定义问题的解空间**, 并**确定易于搜索的解空间结构**。从根节点出发， 深搜遍历整个空间。 开始时, 节点为活节点, 即当前扩展节点。如果不能往纵深移动, 则当前扩展节点成为死节点。同时<mark style="background: transparent; color: red">回溯到上一个活节点处</mark>。

虽然回溯法一般编程较为容易, 但是复杂度一般是指数型的或者阶乘型的, 因此不适用于大规模的问题求解。

例如背包问题的暴力算法一般都采用回溯的方法进行。<mark style="background: transparent; color: red">旅行商问题在规模不是很大时, 也可以采用回溯方法</mark>。
一般常用两种方法避免无效搜索， 提高回溯法的搜索效率。 最容易想到的方法是通过回溯 + 剪枝的方法, 即搜索过程中，采用<b><mark style="background: transparent; color: blue">剪枝函数</mark></b>。具体有如下两类: 
1. <mark style="background: transparent; color: red">使用约束函数，在扩展节点剪去不满足约束的子树</mark>.
2. <mark style="background: transparent; color: red">使用限界函数, 剪去得不到最优解的子树</mark> 
![[Excalidraw/第五章 回溯法 2024-09-18 23.43.10|350]]
### (2) 递归回溯和迭代回溯
回溯法一般分为**迭代回溯**和**递归回溯**两种。
递归法求解回溯问题的一般结构化代码如下:

> [!hint] 说明
> 下面的两个代码均是搜索子集树而言的, 而**搜索排列树**的代码需要在搜索前后进行两次swap, 这在会放在下一节进行讲述。

```c
#include <iostream>

const int max_depth = 5;
int x[max_depth];

void back_track(int depth) {
    if (depth > max_depth) output(x);
    else {
        for (int i = f(n, depth); i < g(n, depth); i++) {
            x[depth] = h(i);
            if (constraint(depth) && bound(depth)) back_track(depth + 1);
        }
    }
}
```

而对于迭代回溯方法, 一般采用的是<b><mark style="background: transparent; color: blue">树的非递归深度优先遍历算法</mark></b>, 核心思想是检测当前是否遍历完, 遍历完则采用 depth-- 进行回退。具体如下:
```c
void back_track_iter() {
    int depth = 1;
    while (depth > 0) {
        if (f(n, depth) < g(n, depth)) {
            for (int i = f(n, depth); i <= g(n, depth); i++) {
                x[depth] = h(i);
                if (constraint(depth) && bound(depth)) {
                    if (is_solution(depth)) output(x);   // 是否已经得到问题的可行解 
                }
                else {
                    depth++;
                }
            }
        }
        else {
            depth--;
        }
    }
}
```

### (3) 子集树与排列树
**子集树和排列树**是回溯法中最常见的两种解空间结构。
<b><mark style="background: transparent; color: blue">子集树</mark></b>定义为从 n 个元素的集合$S$中, 找出$S$满足某种性质的子集, 则称相应的解空间为子集树。一般子集树的遍历需要 $\Omega( 2^n)$ 的空间; 一般一个节点往往仅有2个子节点(选或者不选)

<b><mark style="background: transparent; color: blue">排列树</mark></b>定义为确定 n 个元素的最佳排列问题。即满足某种性质的排列。此时排列树一般有 $O(n!)$ 个节点, 此时遍历需要$\Omega(n!)$计算时间。一般一个节点往往有较多的子节点， 且在求解过程中变化。
![[Excalidraw/第五章 回溯法 2024-09-20 09.16.35|750]]
回溯法搜索子集树的一般方法已经在上面说过, 此处给出排列树的搜索递归算法:
可以参考 [[📘ClassNotes/⌨️Programming/🌳Data Structure & Algorithms/〽️ Data Structure/第三章 栈和队列的链式存储#(5) 穷举n个元素的排列|穷举n个元素的排列]] 问题 :
```cpp
#include <iostream>
#include <algorithm>
using namespace std;
const int n = 5; /* max_depth */
int x[n];

void output(int x[], int n) {
    for (int i = 0; i < n; i++) {
        cout << x[i] << " ";
    }
    cout << endl;
}

/* exhaust the permutation of n elements */
void backtrack_seq(int depth) {
    if (depth == n - 1) {
        output(x, n);
        return;
    }
    /* note: considering the case of j = depth, in this case we still swap */
    for (int j = depth; j  < n; j++) {
        swap(x[j], x[depth]);
        backtrack_seq(depth + 1);
        swap(x[depth], x[j]);
    }
}

int main() {
    for (int i = 0; i < n; i++)  x[i] = i;
    backtrack_seq(0);
    return 0;
}
```

需要说明的是, 无论是递归方法还是迭代方法, 都需要考虑本身该位不交换的情况。
对于迭代办法的排列树回溯法穷举问题, <b><mark style="background: transparent; color: blue">迭代方法遍历排列树需要使用栈来模拟递归调用栈</mark></b>, 首先使用
```cpp
#include  <stack>
stack<pair<vector<int>, int>> stk;
```
先将初始情况加入一个以 pair 为栈的 stack: 每次出栈一个元素并且对相应的深度depth项进行加一操作。<mark style="background: transparent; color: red">最后直到深度大于相应遍历深度时,则进行遍历的实际操作</mark> (此处使用打印输出)
```cpp
/* iterative method -> for the permutation */
vector<int> seq;
for (int i = 0; i < n; i++) seq.push_back(i);

stack<pair<vector<int>, int>> stk;
stk.push(make_pair(seq, 0));

while (!stk.empty()) {
	pair<vector<int>, int> p = stk.top();
	stk.pop();
	
	vector<int> v = p.first;
	int depth = p.second;
	if (depth == n) {
		/* reach the last element */
		for (int i = 0; i < v.size(); i++) {
			cout << v[i] << " ";   /* 在此加入排列树的遍历逻辑 */ 
		}
		cout << endl;
	}
	else {
		for (int i = p.second; i < n; i++) {
			swap(v[depth], v[i]);
			stk.push(make_pair(v, depth + 1));
			swap(v[depth], v[i]);
		}
	}
}
```
示例题目可见[[#(2) 批处理作业调度问题]] 

### (4) 回溯法的关键与复杂度优化方案
回溯法虽然也是一种穷举的方案, 但是<b><mark style="background: transparent; color: blue">关键是深度搜索和剪枝，即在搜索过程中判断方案的可行性, 并且剪去不需要的部分</mark></b>。而不是<mark style="background: transparent; color: red">先穷举出所有的方案，然后判断哪个方案的结果是最佳的</mark>; 因此这样的方法计算复杂度要优于穷举方法。

核心是利用当前的状态计算出一个<mark style="background: transparent; color: red">穷举中的当前值并与最优值比较</mark>, 并剪去不需要的部分。


## 二、相关问题
### (1) 装载问题
对于 n 个集装箱装入 2 个船, 载重量分别为$c_{1}, c_{2}$, 其中集装箱  $i$ 的重量为 $w_i$  确定是否有合理方案将 $n$ 个集装箱装入船。显然类似于背包问题, 我们只需将第一个尽可能装满即可。因此只要递归穷举出所有结果即可。

> [!HINT] O(n) 和 $\Omega (n)$ 的区别
> $O(n)$ 表示算法最坏情况的复杂度, 即运行复杂度的上界; 而 $\Omega(n)$表示算法在最好情况下的复杂度, 即运行的复杂度下界; 有时也使用 $\Theta$ 表示算法的平均复杂度。

动态规划方法求解该问题的复杂度是 $O(\min\{c_1,2^n \})$, 因此该求解办法的复杂度为 $O(2^n)$, 此外还需要额外的 $O(n)$ 栈空间。

以下算法体现的几种典型的剪枝方式:
1. 可行性约束函数: 遍历中发现某种解方案不可行即直接剪去。即剪去不满足条件 $\sum^{m}_{i=1} w_{i} x_{i} \leq  c_{1}$ 的子树。
2. <mark style="background: transparent; color: red">上界函数: 可以通过引入上界函数剪去不含最优解的子树</mark>。引入**上界函数**之后, 可以在到达叶节点之后, 不必检查该叶节点是否由于当前的最优解。例如: cw 是当前重量, 而 $r$ 是 <mark style="background: transparent; color: red">剩余的集装箱重量</mark>，显然当 $cw + r < \text{max\_capacity}$ 时, 可以剪去该分支的右子树。(体现在 current_capacity + accumulate(w + depth, w + num, 0) > max_capacity 一句, 如果小于等于, 则找不到更大, 即可删除对应的分支)
3. 当重量到达 c1 之后, 自动退出返回的机制。

```cpp title:装载问题求解代码
#include <iostream>
#include <algorithm>
#include <numeric>
#include <string.h>

using namespace std;

const int c1 = 20, c2 = 15;

const int num = 10;
const int w[num] = { 2, 5, 9, 4, 2, 4, 3, 1, 2, 3 };
int best_x[num], x[num];

int max_capacity = 0;

/* recursive method for problems */
int max_loading(int depth, int current_capacity) {
    if (current_capacity > c1) return 0;
    if (depth == num) {
        if (current_capacity > max_capacity) {
            memcpy(best_x, x, num * sizeof(int));
            max_capacity = current_capacity;
        }
        return 0;
    }
    if (current_capacity == c1) {
        max_capacity = c1;
        memcpy(best_x, x, num * sizeof(int));
        return 1;
    }
    else if (current_capacity + accumulate(w + depth, w + num, 0) > max_capacity)
    {   /* Note: when accumulate(w + depth, w + num, 0) <= max_capacity, it can't reach best solution any more */
        x[depth] = 0;
        if (max_loading(depth + 1, current_capacity)) return 1;
        x[depth] = 1;
        if (max_loading(depth + 1, current_capacity + w[depth])) return 1;
    }
    return 0;
}

int main() {
    memset(x, 0, num * sizeof(int));
    max_loading(0, 0);
    cout << "max capacity: " << max_capacity << "\n"<<"best choice is : ";
    for (int i = 0; i < num; i++) {
        cout << i << ":" << best_x[i] << ",";
    }
    cout << endl;
    
    return 0;
}
```

### (2) 批处理作业调度问题
参考[[📘ClassNotes/⌨️Programming/🌳Data Structure & Algorithms/🛰️Algorithms/第三章 动态规划算法#(7) 流水线作业调度问题|流水线作业调度问题]](实际上是直接根据推导出Johnson不等式排序解决)
首先, 给定 n 个作业的集合 $J = \{ J_1, J_2, \dots J_n \}$, 每个作业 $J_i$ 均有 2 项任务分别在两台机器上完成, 且每个任务均需要由机器 1 处理, 再由机器 2 处理。其中作业 $J_i$ 需要机器 $j$ 处理的时间为 $t_{ji}$. 其中 $i = 1,2,\dots n, j = 1,2$

<mark style="background: transparent; color: red">我们希望每个产品产出的时刻和最短， 即在机器2上完成处理的时刻和</mark> (时刻的和) 为:
$$f = \sum^{n}_{i=1}  F_{2i}$$
称为该作业调度的**完成时间和**。<mark style="background: transparent; color: red">制定最佳方案, 使得完成时间和最小。</mark>(时刻之和最小)
总结起来实际上是一个比较简单的排列树搜索问题。
对于一个任务列, 可以采用如下的简单代码获取总的完成时间和(由于A的加工时间是可以密堆的, 而下方部分只需考虑 t_last 即可):
![[Excalidraw/第五章 回溯法 2024-09-21 07.47.35]]
```cpp
int get_work_time_sum(vector<task*> tasks) {
    int t_pre = 0, t_last = 0, res = 0;   /* t_pre : record the pre elapsed time before task T_2i */
    if (tasks.size() == 0) return 0;
    for (int i = 0; i < tasks.size(); i++) {
        t_pre += tasks[i]->time1;
        t_last = max(t_last + tasks[i]->time2, t_pre + tasks[i]->time2);
        res += t_last;
    }
    return res;
}
```

完整代码示例如下所示: 
```cpp
#include <iostream> 
#include <algorithm>
#include <vector>
#include <stack>

using namespace std;

typedef struct task{
    char id;
    int time1;
    int time2;
    task(int id, int time1, int time2) {
        this->id = id;
        this->time1 = time1;
        this->time2 = time2;
    };
}task;

int get_work_time_sum(vector<task*> tasks) {
    int t_pre = 0, t_last = 0, res = 0;   /* t_pre : record the pre elapsed time before task T_2i */
    if (tasks.size() == 0) return 0;
    for (int i = 0; i < tasks.size(); i++) {
        t_pre += tasks[i]->time1;
        t_last = max(t_last + tasks[i]->time2, t_pre + tasks[i]->time2);
        res += t_last;
    }
    return res;
}

/* dispatch the best sequence for minimize the sum of the finish time  */
int batch_dispatch(vector<task*> tasks,vector<task*> &tasks_res) {
    tasks_res.clear();
    int n = tasks.size();
    int min_time = 10000;  /* record the minimum time spend */

    /* iterative method -> for the permutation */
    vector<int> seq;
    for (int i = 0; i < n; i++) seq.push_back(i);

    stack<pair<vector<int>, int>> stk;
    stk.push(make_pair(seq, 0));

    while (!stk.empty()) {
        pair<vector<int>, int> p = stk.top();
        stk.pop();
        
        vector<int> idx = p.first;
        int depth = p.second;
        if (depth == n) {
            /* reach the last element */
            vector<task*> tasks_cur;
            for (int i = 0; i < n; i++) {
                cout << idx[i] << " ";
                tasks_cur.push_back(tasks[idx[i]]);
            }
            /* calculate the work time sum */
            int work_time_sum = get_work_time_sum(tasks_cur);
            cout << "  --> work_time_sum: " << work_time_sum << endl;
            if (work_time_sum < min_time) {
                min_time = work_time_sum;
                tasks_res = tasks_cur;
            }
        }
        else {
            for (int i = p.second; i < n; i++) {
                swap(idx[depth], idx[i]);
                stk.push(make_pair(idx, depth + 1));
                swap(idx[depth], idx[i]);
            }
        }
    }
    return min_time;
}

int main() {
    task task1 = task('A', 2, 1);
    task task2 = task('B', 3, 1);
    task task3 = task('C', 2, 3);

    vector<task*> tasks = {&task1, &task2, &task3};
    vector<task*> tasks_res;

    int min_time = batch_dispatch(tasks, tasks_res);

    cout << "min_time: " << min_time << endl;
    cout << "best sequence : ";
    for (int i = 0; i < tasks_res.size(); i++) {
        cout << tasks_res[i]->id << " ";
    }
    cout << endl;
    return 0;
}
```

输出 ACB 为最佳的选择;

### (3) 符号三角形问题
例如对于下图是有 14 个正号和负号的三角形:
对于一个边长为$n$的由+-组成的三角形, 构成符号三角形要求:
1. **每两个同号下面均为 +, 异号下面均为 -**
![[Excalidraw/第五章 回溯法 2024-09-21 07.54.06]]
一般情况下,  **符号三角形的一条边有 n 个符号**, <mark style="background: transparent; color: red">符号三角形问题要求对于给定的 n 值, 计算有多少不同的符号三角形, 使其 +, - 个数相同</mark>。实际上创建一个符号三角形矩阵, 并利用暴力穷举的回溯法问题, 实际上可以从最下面一行搜索, 然后穷举出所有情况(实际上我们只需穷举每行第一个元素, 其他的由下一行确定就行了)。


首先我们给出<b><mark style="background: transparent; color: blue">直接采用二进制值特性, 生成 01的全排列用于遍历集合取值, 并且不使用递归的代码</mark></b>: (实际上排列种类为$2^n$, 只需按二进制输出每一个数即可) 
```cpp
#include <iostream>
void generatePermutations(int n) {
    int total = 1 << n; // 2^n
    for (int i = 0; i < total; ++i) {
        for (int j = n - 1; j >= 0; --j) {
            std::cout << ((i >> j) & 1);
        }
        std::cout << std::endl;
    }
}
int main() {
    int n = 2;
    generatePermutations(n);
    return 0;
}
```

需要说明的是, <b><mark style="background: transparent; color: red">上面采用二进制特性循环遍历集合取值</mark></b>的代码可以用于遍历 0 和 depth 两个位置处的所有选择可能, 实际上从0开始可能的选择是 2 << num, 则只需 i < (1<< num) 即可遍历完毕, 这个是一种方法，但是过于暴力:
```cpp
int idx[2] = { 0, depth };
int num = depth == 0 ? 1 : 2;
/* travel through every possible options of the layer */
for (int i = 0; i < 1 << num; i++) {
	for (int j = 0; j < num ; j++) {
		if (((i >> j) & 0x01)) cout << 1 << " ";
		else cout << 0 << " ";
	}
	cout << endl;
}
```


为了方便, 可以单独设置一个变量 plus_num 记录当前正号个数, 每次设置元素时, 只需 `plus_num += (elem - tri_mat[i][j]);` 就可以了。

`````ad-note
title: 全局变量剪枝条件注意事项
collapse: open
剪枝需要注意 : 由于采用全局变量记录实际上是剪枝减去了 > 4 的情况。
```cpp
+ + -
 + -
  +
```
而实际上由于全局变量存储， <b><mark style="background: transparent; color: blue">这个情况会保留到之后, 会导致后面的部分全部被修剪</mark></b>，而实际减去的只需depth前面部分的 + 个数比这个大才剪枝。因此定义 `int plus_cnt_arr[n];` 记录每一层以下的`+` 数量; 并在 set_elem 中修改对应数量。另外如果先判断也会导致该层无法重新设置而抛弃该层剩下的其他结果。

**因此必须采用设置完这一层后进行判断 continue 条件进行剪枝**。  (51行)
`````

```cpp title:符号三角问题代码
#include <iostream>
using namespace std; 

const int n = 7;
int tri_mat[n][n];

int plus_cnt = 0;
int plus_cnt_arr[n];
int plus_cnt_target = n * (n + 1) / 4;
int result = 0;

void set_elem(int i, int j, int elem) {
    int delta = (elem - tri_mat[i][j]);
    plus_cnt += delta;   /* count in */
    tri_mat[i][j] = elem;

    for (int k = 0; k <= i; k++) {
        plus_cnt_arr[k] += delta;
    }
}

void show_triangle() {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < i; k++)cout << " ";
        for (int j = 0; j < n - i; j++) {
            tri_mat[i][j]? cout << "+ ": cout << "- ";
        }
        cout << endl;
    }
    cout << endl;
}


void tri_recursion(int depth) {
    if (depth == n) {
        if (plus_cnt == plus_cnt_target) {
            show_triangle();
            result++;
        }
        return;
    }
    int layer = n - (depth + 1);
    /* list the first element */
    for (int j = 0; j <= 1; j++) {
        set_elem(layer, 0, j);
        /*  fill other elements */
        for (int i = 1; i < depth + 1; i++) {
            tri_mat[layer + 1][i - 1] ? set_elem(layer, i, tri_mat[layer][i - 1]) : set_elem(layer, i, !tri_mat[layer][i - 1]);
        }
        /* second truncation condition is plus_cnt_target - plus_cnt >  2 * plus_cnt_target - (depth * (depth + 1)/2)  */
        if (plus_cnt_arr[layer] > plus_cnt_target || plus_cnt_target + plus_cnt < (depth * (depth + 1) / 2)) continue; 
        tri_recursion(depth + 1);
    }
}

int main() {
    int tri_mat[n][n];
    memset(tri_mat, 0, sizeof(tri_mat));
    memset(plus_cnt_arr, 0, sizeof(plus_cnt_arr));
    if (n <= 2) cout << "no triangle suitable!!!" << endl;
    else {
        tri_recursion(0);
        cout << "suitable triangle count: " << result << endl;
    }
    return 0;
}

```

### (4) n 后问题 (可用优化和独立构造)
#### 1. 回溯法与可用优化方法的结合
求解 $n\times n$ 棋盘上放置 $n$ 个不互相攻击的皇后问题, 如果是需要求解所有可能的方式, 则穷举基本上是不可避免的。可以参考 [算法典型例题：N皇后问题，五种解法，逐步优化](https://segmentfault.com/a/1190000045080541) 部分, 在实际占用空间中, 左对角线 = 行 + 列; 副对角线 = 行 - 列 则实际上归结为与已有点**行, 列, 行+列, 行-列不同**。 此处<b><mark style="background: transparent; color: blue">详解可用优化方法的思路</mark></b>。

由于可用位置是 行|列|主对角线|次对角线，可以通过 $Y[i], L[i],  R[i]$ <mark style="background: transparent; color: red">记录每列是否被占据。判断某个位置是否可以放置皇后</mark>。如果采用穷举和判断, 则复杂度达到 $O(n^n)$, 基本无法解决。 <mark style="background: transparent; color: blue">其中比较好的办法是采用一个 next 记录每行下一个可用的列</mark>,  

在 N后问题中,  对于检查某个排列是否为有效解, (用于检测是否在斜线上, 显然右斜线 $i-p[i] = j - p[j]$可以检测到, 而当 $i + p[i] = j +  p[j]$ 时, 有 $i - j =  p[j] - p[i]$) , 因此可以采用 `abs(perm[i] - perm[j]) == abs(i - j)` 的方法进行检测。

下面给出可用优化方案的解题思路: 利用 cur 记录当前第 k 行检测的位置, 而$q[k]$记录实际选择。 
令 l, r, 初始时均为0, 则可以放置的条件是 l, r 对应位置均为 0, 首先将 cur 置为 $q[k]$, 然后继续尝试。(cur && cur <= e) 是<mark style="background: transparent; color: red">结束条件</mark>, 
```cpp
// $q[k] > cur$ 是循环条件, 实际上相当于 
for (cur = nex[pre]; cur && cur <=e && cur <= q[k]; pre = cur, cur = nex[pre]);
while (cur && cur <= e && (l[k + cur] || r[k - cur + n]))
            pre = cur, cur = nex[pre];
```

实际上一开始是取行数 k = 1 的, 如果没有则回溯 k--, 回溯到 k<1 时, 退出循环。
初始化时 q 均为0, 标志每个都没有放过。**因此每次回溯释放时, 会释放 l,r 和 q**

寻找时, 仅仅是寻找 l,r 满足条件的位置, 而 $\text{cur} > q[k]$ 是尝试的列; 每次尝试放置下一行时, 会将 $q[++k] = 0$, 即从 0 开始重新寻找。
![[Excalidraw/第五章 回溯法(DFS) 2024-09-23 16.31.20|550]]
每一次, 由于列 cur 处已经被使用, 我们利用 $\text{nxt[cur]}$ 节点<mark style="background: transparent; color: red">存储 pre 节点的值用于恢复. </mark>, 并将 pre 节点置为 $\text{nxt[cur]}$, 即将可用的下一个节点下标前移到原先的位置。而恢复时, 只需重设并按照层换回 nxt 的两个值, **即可恢复 nxt 的状态**。 而如果回溯中验证得到该列不可用, 则首先 k--, 向前一行, 然后重新从 0 开始, 寻找可用的位置。

`next[i]` 表示位置 `i` 的下一个可用位置（可用列) 
```cpp title:可用优化方法求解N皇后问题
#include  <iostream>
#include  <string>

using namespace std;
const int n = 8;

/**
 * N皇后问题：可用优化（所有下标均从1开始）
 * @param n 皇后的数量
 * @return 摆法的数量
 */
int queen() {
    // 特判
    if (n == 1)return 1;
    // 棋盘 当前放置哪个皇后 解的数量
    int q[n + 1], k = 1, res = 0;
    memset(q, 0, sizeof q);

    // 中点位置 e:当前行最多能放到第几列
    int w = (n + 1) >> 1, e;
    // n是否为奇数
    bool odd = n & 1;
    // 标志数组
    int nxt[n + 1], l[2 * n + 1], r[2 * n];
    memset(l, 0, sizeof(l)), memset(r, 0, sizeof(r));

    for (int i = nxt[n] = 0; i < n; ++i) nxt[i] = i + 1; // 建立可用列链表
    
    int cur = 0, pre = 0, temp = 0;  // 当前节点 cur前驱 临时节点 

    while (k > 0) {
        pre = 0, cur = nxt[pre]; // cur指向第一个可用位置
        
        if (k == 1)e = w;  // 第一行放置的皇后不能超过中间
        else if (k == 2 && odd && q[1] == w)e = w - 1;   // N为奇数且第一行放在中间时，第二行不能超过中间
        else e = n;  // 其它情况超过中间

        /**
         * 寻找第k行的下一个可以放置的位置
         * !l[k+cur]&&!r[k-cur+n]&&q[k]<=cur：cur需要满足的条件，q[k]<=cur保证当前行尝试的位置会“一直前进”
         * cur=0: 链表为空或者找到最后未发现满足条件的列
         * cur>e：cur已超过当前行设定的边界，即基础实现中添加的两个限制条件
         * cur&&cur<=E用以限定cur的边界
         */
        //  for (cur = nex[pre]; cur && cur <=e && cur <= q[k]; pre = cur, cur = nex[pre]);
        while (cur && cur <= e && (l[k + cur] || r[k - cur + n] || q[k] > cur))
            pre = cur, cur = nxt[pre];
        // 放置当前行时，把**当前行原先占有的位置释放**
        if (q[k]) {
            // 恢复成放置原先位置前的状态
            temp = nxt[q[k]];
            nxt[q[k]] = nxt[temp];
            nxt[temp] = q[k];
            // 保持pre为cur的前驱
            if (nxt[q[k]] == cur)pre = q[k];
            // 标记所在斜线可放置
            l[k + q[k]] = r[k - q[k] + n] = 0;
        }
        if (!cur || cur > e)k--;  // 未找到合适的列，回溯
        else if (k == n) { // 找到合适的列但当前行是最后一行，放完再回溯
            q[k] = cur;
            for (int i = 1; i <= n; i++) cout << q[i] << " ";
            cout << "nxt array : ";
            for (int i = 0; i < n+1; i++) cout << nxt[i];
            cout << endl;


            res++, k--;
        }
        // 找到合适的列但非最后一行，放完后放置下一行
        else {
            q[k] = cur;
            nxt[pre] = nxt[cur];                // cur已被占用，删除cur
            nxt[cur] = pre;                     // 记录前驱，用以恢复到放置前的状态
            l[k + cur] = r[k - cur + n] = 1;    // 标记所在斜线不可放置
            q[++k] = 0;                         // 放置下一行
        }
    }
    return res << 1;
}

int main() {
    cout << queen() ;
}
```

> [!NOTE] 对称性的应用 
> 另外第一行皇后放置位置不能超过中点, n为奇数且第一行放在中间时，第二行不能超过中间, 否则实际上是对称的;

#### 2. 构造法在线性复杂度求解单个解的问题
此外, 对于 N 后问题, 如果仅需要[线性复杂度构造 N 皇后单个解](https://zhuanlan.zhihu.com/p/612041043)
首先由于我们是求解$k$行的一个列的排列 $p_k$,根据:
$$\sum (k - p_{k}) = 0$$
而同时 $k+p_{k}$ 各不相同, $k-p_{k}$ 各不相同, 此时可以构造前面 $k -p_k$ 均为正数,后面均为负数。
$$p = \begin{cases}
2,4,6, \dots n, 1,3,5, n-1 \qquad   n 偶数\\
2,4,6, \dots n-1, 1,3, \dots n \qquad  n奇数
\end{cases}$$
此时显然$k-p_k$ 是不同的, 而 $k+p_k$ 是3的等差数列,则不同需要: 
$$\left(\frac{n}{2} + 2 \right)\%3  \neq  (2 + 1)\%3 + \rightarrow  \frac{n}{2} \%3 \neq  1 \rightarrow n \%6 \neq  2 $$
上式在n为偶数时, 同理得到n为奇数时:
$$\frac{n - 1}{2} \% 3 \neq  1  \rightarrow n \% 6\neq  3 $$
此时针对 n = 6t+2 和 6t + 3 的情况, 可以采用如下构造 (见[参考资料](https://www.cnblogs.com/qscqesze/p/5795803.html)): 
取 $k = \frac{n}{2}, n$ 为偶数; 或者 $k = \frac{n - 1}{2}, n$ 为奇数
k,k+2,k+4,...,n,2,4,...,k-2,k+3,k+5,...,n-1,1,3,5,...,k+1 (k为偶数,n为偶数)  
k,k+2,k+4,...,n-1,2,4,...,k-2,k+3,k+5,...,n-2,1,3,5,...,k+1,n (k为偶数,n为奇数)  
k,k+2,k+4,...,n-1,1,3,5,...,k-2,k+3,...,n,2,4,...,k+1 (k为奇数,n为偶数)  
k,k+2,k+4,...,n-2,1,3,5,...,k-2,k+3,...,n-1,2,4,...,k+1,n (k为奇数,n为奇数)

### (5) 最大团问题
给定无向图 $G = (V,E)$,(其V为顶点集, E为边) 若有 $U \subseteq  V$, 且对于任意的 $u, v \in  U$ 均有 $(u,v) \in E$, 则称 U 是 G 的<mark style="background: transparent; color: red">完全子图</mark>。即<mark style="background: transparent; color: red">在完全子图中, 顶点之间两两相互连接</mark>。
$G$ 的完全子图 $U$是$G$的<mark style="background: transparent; color: red">团</mark>当且仅当 $U$ **不包含在 $G$ 的**<mark style="background: transparent; color: red">更大的完全子图中</mark>。而最大团是指<mark style="background: transparent; color: red">顶点数目最多的团</mark>，即顶点数目最多的顶点之间相互连接的子图。

<b><mark style="background: transparent; color: blue">补图的定义</mark></b>: 对于任一无向图 $G = (V, E)$, 其补图定义 $\overline{G} = (V_1, E_1)$ 定义为: $V_1 = V$, 且有 $(u,v)\in E_1$ , 当且仅当 $(u,v) \notin E$ 时, $\overline{G}$ 为 G 的补图。
![[Excalidraw/第五章 回溯法(DFS) 2024-09-23 19.37.58]]

<b><mark style="background: transparent; color: blue">独立集的定义</mark></b>: 对于 $U \subseteq  V$,且 $\forall u ,v\in U$, 满足 $(u,v) \notin E$, 则称 $U$ 为 V 的独立集。例如(2,4,6) 是最大独立集。(最大独立集求解也采用回溯法进行)

最大团问题是典型的回溯法求解问题。示例代码如下:
```cpp title:最大团问题示例代码
#include <iostream>

using namespace std;

const int n = 7;   /* node number */

int adjacency_matrix[7][7] = {
    {0, 1, 1, 1, 0, 0, 1}, // connection for node 1
    {1, 0, 0, 0, 1, 0, 1}, // connection for node 2
    {1, 0, 0, 1, 0, 0, 1}, // connection for node 3
    {1, 0, 1, 0, 1, 0, 1}, // connection for node 4
    {0, 1, 0, 1, 0, 1, 1}, // connection for node 5
    {0, 0, 0, 0, 1, 0, 1}, // connection for node 6
    {1, 1, 1, 1, 1, 1, 0}  // connection for node 7
};

int max_size = 0;   /* record the maximum clique size */
int max_clique[n];   /* record the maximum clique */
int clique_tmp[n];

bool check(int mat[n][n], int cur_clique[n], int new_node) {
    for (int i = 0; i < new_node; i++) {
        if (cur_clique[i] && !mat[i][new_node]) {
            return false;
        }
    }
    return true;
}

void trace_back(int mat[n][n], int depth, int clique_size) {
    if (clique_size + n - depth <= max_size) return;
    if (depth == n) {
        if (clique_size > max_size){
            max_size = clique_size;
            memcpy(max_clique, clique_tmp, n * sizeof(int));
        }
        return;
    }
    if (check(mat, clique_tmp, depth)) {
        clique_tmp[depth] = 1;
        trace_back(mat, depth + 1, clique_size + 1);
        clique_tmp[depth] = 0;
        trace_back(mat, depth + 1, clique_size);
    }
    else {
        clique_tmp[depth] = 0;
        trace_back(mat, depth + 1, clique_size);
    }
}

/* use the traceback method for maximum clique algorithm */
void get_max_clique(int mat[n][n], int clique[]) {
    max_size = 0;
    memset(max_clique, 0, n * sizeof(int));
    memset(clique_tmp, 0, n * sizeof(int));
    trace_back(mat, 0, 0);
    memcpy(clique, max_clique, n * sizeof(int));
}

int main() {
    int clique[n];
    get_max_clique(adjacency_matrix, clique);

    cout << "max size : " << max_size << endl;
    cout << "max clique :";
    for (int i = 0; i < n; i++) {
        if (max_clique[i]) {
            cout << " " << i;
        }
    }
    return 0;
}
```


> [!CAUTION] 注意
> 需要注意的是, 双循环迭代会导致某个节点加入之后无法退出, 注意不是一个节点和其他节点有连接， 加入之后就必将成为最大团的一部分。因此需要考虑每个节点加与不加的情况, 复杂度是 $n * 2^n$

### (6) 图的 m 着色问题 
给定一个无向连通图和 m 种不同的颜色, 要求每个顶点着色一种颜色, <mark style="background: transparent; color: red">而每条边两个顶点颜色不同,  而对应的可着色的最少需要的颜色种类数记为</mark>m,  则称 m 为这个图的色数。求一个图的色数 m 的问题称为图的<mark style="background: transparent; color: red"> m 可着色优化问题。</mark>

仍然采用如下的无向图作为着色优化目标:
![[Excalidraw/第五章 回溯法(DFS) 2024-09-25 14.47.42|350]]
此时, 考虑每个顶点穷举 m 种颜色, 如果不行则 m++; 当解空间为 m, 节点数目 n 时, 则总检查个数复杂度是 $\sum_{i = 1}^{n} m^{i}$, 每次添加一个节点需要查重的复杂度是 $n$ , 则总复杂度为
$$\sum_{i = 1}^{n} m^{i} \times  n  = O (nm^{n})$$
一般问题, 我们只需考虑对于给定m, 某个图是否是 m 可着色的, 如果是可着色, 则返回其所有有效解。

<b><mark style="background: transparent; color: blue">回溯过程中, 我们主要每次加入一个节点, 然后判断这个节点和之前的颜色是否冲突即可</mark></b>, 而**不需要全部举出来再判断结果**。
```cpp title:图m着色问题回溯方法代码
#include <iostream>
using namespace std;

const int n = 7;
const int m = 4;
/* color for each node */
int colors[n] = { 0, 0, 0, 0, 0, 0, 0 };  /* 0: not have color */ 
int result_num = 0;
int adjacency_matrix2[n][n] = {
    {0, 1, 1, 1, 0, 0, 1}, // connection for node 1
    {1, 0, 0, 0, 1, 0, 1}, // connection for node 2
    {1, 0, 0, 1, 0, 0, 1}, // connection for node 3
    {1, 0, 1, 0, 1, 0, 1}, // connection for node 4
    {0, 1, 0, 1, 0, 1, 1}, // connection for node 5
    {0, 0, 0, 0, 1, 0, 1}, // connection for node 6
    {1, 1, 1, 1, 1, 1, 0}  // connection for node 7
};

bool is_valid(int map[n][n], int color[n], int idx) {
    for (int i = 0; i < idx; i++) {
        if (map[idx][i] && (color[i] == color[idx])) {
            return false;
        }
    }
    return true;
}

void trace_back(int depth) {
    if (depth == n) {
        result_num++;
        for (int color : colors) {
            cout << color << " ";
        }
        cout << endl;
        return;
    }
    for (int i = 1; i <= m; i++) {
        colors[depth] = i;
        if (is_valid(adjacency_matrix2, colors, depth)) {
            trace_back(depth + 1);
        }
    }
}

int main() {
    trace_back(0);

    cout <<  "available results : " << result_num << endl;
    return 0;
}
```

> [!NOTE] 4 色猜想
> 4 色猜想猜测: 假设每个国家是单连通域, 在一个平面或者球面上的任何地图能够只使用 4 种颜色进行着色

4色猜想的说明: 
![[Excalidraw/第五章 回溯法(DFS) 2024-09-25 15.43.41|750]]
### (7) 圆排列问题
给定 n 个大小不等的圆 $c_1, c_2, \dots c_n$,  此时将 n 个圆排进一个矩形框中, 并要求圆与矩形框底部相切。从 n 个圆的排列中，找出到右侧圆直径长度最小的圆排列。

![[Excalidraw/第五章 回溯法(DFS) 2024-09-25 16.04.44]]
由于距离计算为:
$$d = r1 + \sqrt{(r2 + r1)^{2} -(r2 - r1)^{2}}  + r2 = r1 + r2  + 4 \sqrt{r1 \times r2} $$
回溯极其容易:
```cpp 
#include <iostream>
#include <algorithm>
using namespace std; 

const int n = 10;
int circuit_radius[n] = {1, 2, 10, 7, 4, 5,  3, 8, 9, 6 };
int min_radius[n];

float min_dis = 10000.0;

/* cur_dis 当前圆的圆心位置 */
void circuit_seq_traceback(int depth, float cur_dis) {
    if (cur_dis >= min_dis) return;
    if (depth == n) {
        if (cur_dis < min_dis) {
            min_dis = cur_dis;
            memcpy(min_radius, circuit_radius, sizeof(min_radius));
        }
        return;
    }
    for (int i = depth; i < n; i++) {
        swap(circuit_radius[depth], circuit_radius[i]);
        float res_dis = 0;
        if (depth != 0) {
            res_dis = cur_dis + sqrt(4 * circuit_radius[depth] * circuit_radius[depth - 1]);
        }
        if (depth == 0 || depth == n - 1) {
            res_dis += circuit_radius[depth];
        }
        circuit_seq_traceback(depth + 1, res_dis);
        swap(circuit_radius[depth], circuit_radius[i]);
    }
}

int main() {
    memset(min_radius, 0, sizeof(min_radius));
    circuit_seq_traceback(0, 0.0);

    cout << "min_dis = " << min_dis << endl;
    for (int i = 0; i < n; i++) {
        cout << min_radius[i] << " ";
    }
    return 0;
}
/*
min_dis = 92.7701
5 7 3 9 1 10 2 8 4 6
*/
```

上述算法的时间复杂度是 $O(n!)$, 另外对于位置对称和有部分圆半径相同的情况, 实际上不用继续取相应的排列。可以通过此进一步减小计算量。

### (8) 电路板排列问题
对于电路板排列问题, 该问题实际上是<mark style="background: transparent; color: red">将 n 块电路板以最佳方案插入带有 n 个插槽的机箱中,  而其不同的排列方式对应不同插入方案</mark>。
设 $B = \{1,2, \dots  n\}$ 是 n块电路板集合, 且取每一个连接块 $N_i$ 为 B 的子集, 且 $N_i$ 中的电路板使**用同一根导线相连**; 

取 n = 8, m = 5 时, 给定 n 块电路板及其对应的 m 个连接如下:
$$\begin{cases}
B = \left\{1,2,3,4,5, 6,7,8  \right\}  \\
L =  \left\{N_{1},  N_{2} N_{3}, N_{4}  N_{5} \right\}\\
N_{1} = \left\{ 4,5,6 \right\}  \\
N_{2} = \left\{ 2,3 \right\} \\
N_{3} = \left\{ 1,3 \right\} \\
N_{4} = \left\{ 3, 6 \right\}\\
N_{5}= \left\{ 7, 8 \right\} 
\end{cases}$$
如果电路板是按照 $1,2,3, \dots 8$  插入的, 则按照下面的 "跨线数" 给出每两格之间的连线数量。而<mark style="background: transparent; color: red">电路板排列密度</mark>定义为跨越相邻电路板插槽的最大连线数. 
![[Excalidraw/第五章 回溯法(DFS) 2024-09-25 19.09.32|600]]
求解一种最佳的电路板的排列顺序, 以使电路板具有最小排列密度(例如上图的为2), 而右侧为最佳的排列方案。

为了降低复杂度, 我们不采用封装一个函数判断的方法, 例如如下虽然能够计算, 但是需要列举全部。这失去了回溯法的意义。
```cpp
int circuit_density(int circuit_boards[n], array<vector<int>, m> circuit_conn) {
    int line_dense[n - 1];
    memset(line_dense, 0, sizeof(line_dense));

    for (int i = 0; i < m; i++) {
        vector<int> conn = circuit_conn[i];
        int min_idx = n, max_idx = 0;
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < conn.size(); k++) {
                if (circuit_boards[j] == conn[k]) {
                    if (j < min_idx) min_idx = j;
                    else if (j > max_idx) max_idx = j;
                }
            }
        }
        for (int j = min_idx; j < max_idx; j++) {
            line_dense[j] += 1;
        }
    }
    return *max_element(line_dense, line_dense + n - 1);
}
```

因此，为了 <b><mark style="background: transparent; color: blue">每一步能够确定当前状态的</mark></b> 最小密度, 我们希望能够在交换到电路板 i 时, **查看 $i$ 在几个连接块中**, 同时将其状态存储; 

显然, 当前面的节点已经确定，并且前面的排列也确定时, 在 0~i-1 之间的各个部分的连接块都已经确定了。因此我们只需对于新加的节点 i, 查询: 
1. 节点 i 在几个区间中, 如果在这个区间中且<mark style="background: transparent; color: red">不等于该区间的开始值</mark>, 则可以将相应区间的排列密度值 +1 
2. 由于需要记录是否在某个区间中, 不能直接通过矩阵进行判断。所以需要带一个 **conn_now数组参数**, <mark style="background: transparent; color: red">记录当前已经遍历过第 j 的选择的多少个节点(最多不超过对应选择的数量)</mark>, 而是否在其中可以<mark style="background: transparent; color: red">通过新建一个 conn_mat 矩阵来进行判断</mark>， 只要在递归过程中判断这个小区间的密度即可, 以前的部分均不需要保存。
3. 在某个区间, 要求: 1. `conn_left[j] > 0;`   2. `conn_now[j] < conn_tot[j];`  -> 而每次只需遍历方法让 `conn_left[j] -=  conn_mat[j][idx]` 即可
4. <b><mark style="background: transparent; color: blue">采用回溯法回退到以前的状态方法: </mark></b>只需恢复conn_left 数组, 这个只需重新遍历一遍，即 `conn_left[j] +=  conn_mat[j][idx]` 而 wire_nums 不需要恢复。


> [!caution] 循环中进行剪枝的方法
> 循环中剪枝，一般采用 `continue` 进行, 此时不需要将当前计算出的状态量传给下一个函数。而必须传入当前计算的状态量时, 一般在初始判断并且 `return;` 
> 但是在两个 swap 中采用 continue 会导致最后一个 swap 无法正常进行, 因此最好还是用 return 的方式。

> [!caution] memcpy 的问题
> 在复制数组包括memset内容时, 如果是用 n 指定元素个数, 必须加上 sizeof(int) 和 sizeof(char) 等等;

```cpp title:电路板排列问题(完整代码)
#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
using namespace std; 

const int n = 8;
const int m = 5;
int circuit_boards[n];       /* 电路板的排列数组 */
int best_circuit_boards[n];  /* 电路板排列数组的最佳结果 */
int wire_nums[n - 1];        /* 每个区间之间的连线数量 */
int best_wire_nums[n - 1];   /* 电路板区间之间的连线数量的最佳结果 */

int conn_tot[m];             /* 记录每个方法总数量的全局变量 */
int min_density = 10000;     

const std::array<std::vector<int>, m> circuit_conn = { {
    {3,4,5},
    {0, 2},
    {0, 1},
    {2, 5},
    {6, 7},
}};

int** init_conn_matrix(array<vector<int>,m> conn) {
    /* 新建一个矩阵, 存储每个连接块是否有节点, 即连接块 */
    int** conn_mat = new int*[m];   /* block m */
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

/* 回调函数, 计算每个电路板的连接,找出最小的部分 */
void circuit_traceback(int depth,  int **conn_mat, int *conn_left, int max_wire_num) {
    if (max_wire_num >= min_density) return;  /* trim the branch */
    if (depth == n) {
        /* record minimum density */
        if (max_wire_num < min_density) {
            min_density = max_wire_num;
            memcpy(best_circuit_boards, circuit_boards, n * sizeof(int));
            memcpy(best_wire_nums, wire_nums, (n - 1)* sizeof(int));
        }
        return;
    }
    for (int i = depth; i < n; i++) {
        int wire_num = 0;
        swap(circuit_boards[i], circuit_boards[depth]);
        int idx = circuit_boards[depth];  /* 当前电路板的编号 */ 
        for (int j = 0; j < m; j++) {
            if (conn_left[j] < conn_tot[j] && conn_left[j] > 0) {  /* 该节点在第j个连接块范围内 */
                wire_num++;
            }
            conn_left[j] -= conn_mat[j][idx];
        }
        if (depth > 0) wire_nums[depth - 1] = wire_num; /* 记录区间线的个数(0时不需要) */
        circuit_traceback(depth + 1, conn_mat, conn_left, max(max_wire_num, wire_num));
        swap(circuit_boards[i], circuit_boards[depth]);

        /* 每次退出一个节点搜索时, 需要恢复 conn_left 的状态 */
        for (int j = 0; j < m; j++) {
            conn_left[j] += conn_mat[j][idx];
        }
    }
}

/* 回溯法求解电路板排列问题 */
int circuit_board_problem() {
    memset(wire_nums, 0, sizeof(wire_nums));
    memset(best_wire_nums, 0, sizeof(best_wire_nums));

    int conn_left[m];
    for (int i = 0; i < m; i++) conn_tot[i] = circuit_conn[i].size();
    memcpy(conn_left, conn_tot, sizeof(conn_tot));
    int** conn_mat = init_conn_matrix(circuit_conn); /* 生成记录矩阵 */ 
    
    memset(best_circuit_boards, 0, sizeof(best_circuit_boards));
    for (int i = 0; i < n; i++) circuit_boards[i] = i;

    /* 调用递归函数 */
    circuit_traceback(0, conn_mat, conn_left, 0);

    cout << "minimum density: " << min_density << endl;
    cout << "best circuit boards: ";
    for (int i = 0; i < n; i++) {
        cout << best_circuit_boards[i] << " ";
    }
    cout << endl;
    cout << "best wire nums: ";
    for (int i = 0; i < n-1; i++) {
        cout << best_wire_nums[i] << " ";
    }
    cout << endl;
    return 0;
}

int main() {
    circuit_board_problem();
    return 0;
}
```

得到的 1 0 2 5 4 3 6 7 实际上是密度最小的排列结果。
```cpp
minimum density: 1
best circuit boards: 1 0 2 5 4 3 6 7
best wire nums: 1 1 1 1 1 0 1
```

复杂度分析: 计算**各个节点的连线数量和恢复均需要 m 复杂度**，而根节点个数为 $n!$, 则计算密度耗时为 $O(m n!)$ , 由于每一次更新最优解最多减少1， 最优解的更新次数为 O(m), 因此更新最优解的 memcpy 需要 O(mn) 时间。总复杂度为 $O(mn!)$ 

### (9) 连续邮资问题
首先, **设某国家发行了 n 种不同面值的邮票**, 从小到大排列; 而**每个信封仅允许贴 m 张邮票**; 连续邮资问题要求对于给定的 m,n 值, <mark style="background: transparent; color: red">给出邮票面值的最佳设计</mark>, 得到<mark style="background: transparent; color: red">在 1 个信封上可贴出邮资 1 开始, 增量为 1 的最大连续邮资区间</mark>。

> [!example] 示例
> 取 n = 5, m = 4, 则采用 (1, 3, 11, 15, 32) 作为邮票面值, 可以贴出连续区间为 1-70

设邮票的面值分别为 $d_1, d_2, \dots d_n$, 
为了保证基础的连续, 不考虑 $m$ 大小, 则第二个邮票的面值必须在 $[2 * d_{1}, m + 1]$ 以内; 例如第二个为 6, 则必然不连续。

另外, 考虑贴邮票的过程并不是按照贪心算法去贴的, 而是**采用类似背包问题**(如1,3,6,7 的面值, 贴18邮票应该最少 6,6,6 而非 7, 7, 3, 1);

给出一种思路: <mark style="background: transparent; color: red">逐次尝试, 增加面值(例如第二张从2开始)</mark>, 并将初始面值设置为 $(1,2,\dots n)$, 停止条件是 <mark style="background: transparent; color: red">如果和前面部分面值区间产生不连续的部分， 则不继续增加</mark>;  但是这样比较复杂, 因为最后一个尝试次数可能有 $(m + 1)^{n - 1}$, 取 m = 4, n = 5, 则实际上有 $5^{4} = 625$ 种, 可以接受。

下面说明回溯法的求解办法:
1. 显然 $d[2]$ 的取值范围是 $[2: m+1]$, 而在一般情况下, 当 $x[1, i-1]$ 已经选定之后, 连续区间已经确定为 $[1:r]$ (新加一个则重新计算), 对应的第i张邮票面值**也不应当超过\[r+1\]**, 则<mark style="background: transparent; color: red">第 i 张邮票面值尝试区间</mark>为 $[x[i-1] + 1, r]$ , 然后深度优先遍历搜索即可。
2. <mark style="background: transparent; color: red">利用一个比较大的数组 least_stamps</mark> **记录在 $d[1:r]$ 已经确定的情况下, 能够贴出 $y[j]$ 邮资所需要的最少邮票数**。一般可以将上限取为 2000 左右; 
3. 对于 $y[j]$ 在迭代过程中, 实际上<mark style="background: transparent; color: red">在新加入邮票面值之前的凑出方式均不需要更改</mark>, 因此只需要从 $x[i-1]+1$ 开始进行检查, 然后更新后面部分获得面值得到的对应的最小邮票数即可。 截断条件(循环停止条件) 是有一个值查不到连续。

4. 对于检查某个值最少需要几张邮票的问题, 可以按照背包问题的思路求解某个值最少需要几张邮票凑出: (实际上仅需要三行, 此处直接采用全部矩阵, 不考虑空间复杂度)
![[Excalidraw/第五章 回溯法(DFS) 2024-09-26 15.23.50|700]]


完整代码如下: 
```cpp title:连续邮资问题回溯方法代码
#include <iostream>
#include <vector>
#define MAX_VAL_TEST 2000

using namespace std;

const int m = 4;
const int n = 5;

int maxval = 0;    /* record the maximum continuum stamp value range */
int d[n];    /* denomination of each stamp */
int d_best[n];
int  least_stamp[MAX_VAL_TEST + 1];  /* least stamp_table, each record least stamp number needed for this value */

/* bag problem solution for get the minimum number for correspond value 
* @param d: denomination of each stamp
* @param n: number of stamps
* @param value: the value that we want to get
* @param max_number: the maximum number of stamps we can use 
* @return: the minimum number of stamps (0 if no possible solution)
*/
int get_least_stamp_num(int d[], int n, int value, int max_number) {
    int res = 0;
    int *dp = new int[(value + 1) * max_number];  /* allocate buffer for dynamic programming */
    memset(dp, 0, sizeof(int) * (value + 1) * max_number);
    for (int k = 0; k < n; k++) dp[d[k]] = 1;  /* initialize */

    for (int i = 1; i < max_number; i++) {
        int idx = i * (value + 1);
        int idx_pre = (i - 1) * (value + 1);  /* index of the line  before */
        for (int j = 1; j <= value; j++) {
            if (dp[idx_pre + j] != 0) {
                /* inherit the value from the pre-calculated values */
                dp[idx + j] = dp[idx_pre + j];
                continue;
            }
            /* else combine the value from the pre-calculated values */
            for (int k = 0; k < n; k++) {
                if (d[k] < j && dp[idx_pre + (j - d[k])] != 0) {
                    dp[idx + j] = dp[idx_pre + (j - d[k])] + dp[d[k]];
                    break;
                }
            }
        }
        /* truncate condition judge */
        if (dp[idx + value] != 0) {
            res = dp[idx + value]; break;
        }
    }
    return res;
}

/* 
* @function : stamp denomination trackback function 
* @depth : the depth (index) of the stamp denomination 
* @param max_con_val : maximum combined continuum value that can be reached by the stamps
*/
void stamp_trackback(int depth, int max_con_val) {
    if (depth == n) {
        if (max_con_val > maxval) { 
            maxval = max_con_val;
            memcpy(d_best, d, sizeof(d));
        }
        return;
    }

    /* iterate all possible next stamp value */
    for (int val = d[depth - 1] + 1; val <= max_con_val + 1; val++) {
        d[depth] = val;     /* refresh the least_stamp array */ 
        /* recalculate all possible combined value  (stop when no solution found for value j) */ 
        int j = val, res = 1;
        for (j = val; j < MAX_VAL_TEST && res != 0; j++) {
            //  max number of stamps is m 
            res = get_least_stamp_num(d, depth + 1, j, m);
            least_stamp[j] = res;
        }
        /* note : it self-add after reach 0, so -2 for true result */
        int conn_val_res = j - 2;  /* maximum possible combined value by current stamps */
        stamp_trackback(depth + 1, conn_val_res);
    }
}

/* continuum postage problems */ 
void continuum_postage_problem() {
    memset(d, 0, sizeof(d));
    d[0] = 1;   /* first stamp must have denomination 1 */
    /* initialize the "least_stamp" array */
    memset(least_stamp, 0, sizeof(least_stamp));
    least_stamp[1] = 1;           /* least stamp for combine $1 is 1 */
    stamp_trackback(1, m);        /* for $1 and number of m */

    
    for (int i = 0; i < n; i++) {
        cout << d_best[i] << " ";
    }
    cout << " max combinded number : " << maxval << endl;
}

int main()
{
    continuum_postage_problem();
    return 0;
}
```

输出为:
```cpp
1 3 11 15 32  max combinded number : 70
```

