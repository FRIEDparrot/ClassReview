- [ ] 博文《中序线索化排序二叉树的节点插入和删除》
- [ ] 计算流体力学的案例以及代码


代码部分为: 
```cpp
#include <iostream>
#include <vector>
#define Inf 32767
using namespace std;

typedef int DataType;

typedef struct TreeNode {
    DataType data = 0;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    bool left_tag = false;
    bool right_tag = false;
};

TreeNode* create_tree() {
    int data;
    cin >> data;
    TreeNode* node = nullptr;
    if (data != -1) {
        node = new TreeNode();
        node->data = data;
        node->left = create_tree();
        node->right = create_tree();
    }
    return node;
}

TreeNode* pre = nullptr, *last = nullptr;

// 进行中序遍历线索化
void midThread(TreeNode* root) {
    if (!root) return;
    midThread(root->left);

    if (pre && !pre->right) {
        pre->right_tag = true;
        pre->right = root;
    }
    if (!root->left) {
        root->left_tag = true;
        root->left = pre;
    }
    pre = root; // 将该子树设置为前一颗,注意防在中间
    midThread(root->right);
}

// 查找时， 如果right_tag = true则为右节点，否则为右子树的左下节点
TreeNode* next_find(TreeNode* root) {
    if (!root || !root->right) return nullptr;
    if (root->right_tag) return root->right;

    TreeNode* p = root->right;
    for (; p->left && !p->left_tag; p = p->left);
    return p;
}

// 对于寻找前一个节点, 如果有leftTag 则返回左节点, 如果没有则返回左子树右下端点
TreeNode* prev_find(TreeNode* root) {
    if (!root || !root->left) return nullptr;
    if (root->left_tag) return root->left;
    TreeNode* p = root->left;
    for (; p->right && !p->right_tag; p = p->right);
    return p;
}

// 遍历线索树
void mid_travel(TreeNode* root, vector<int> &res) { // m, n为上下限
    while (root->left) {
        root = root->left;
    }
    for (TreeNode* p = root; p ; p = next_find(p)) {
        res.push_back(p->data);
    }
}

// 在中序遍历的线索树中插入
void insert_elem(TreeNode* & root, DataType insert) {
    TreeNode* p = root, *pre = root, *L = new TreeNode();
    L->data = insert;
    L->left_tag = true; // 因为是插到叶节点上
    L->right_tag = true;
    if (!root) {
        root = L; return;
    }
    // 不考虑相同元素, 左边小右边大的方法插入
    while (true) {
        pre = p;
        // 注意线索化的树插入时不能经过虚节点
        if (p->data > L->data) {
            if (p->left_tag) break;
            p = p->left;
        }
        else {
            if (p->right_tag) break;
            p = p->right;
        }
    }
    // 更新节点
    if (L->data < pre->data) { // 此时显然pre->left_tag = true;
        // 小的部分, 插入时排在前面，即左节点
        pre->left_tag = false;

        L->left = pre->left;
        L->right = pre;
        
        pre->left = L;
    }
    else {// pre->  right_tag
        // 大的部分，插入到右节点上
        pre->right_tag = false;
    
        L->right = pre->right;
        L->left = pre;

        pre->right = L;
    }
}

// 删除元素
void delete_elem(TreeNode* & root, DataType Delete) {
    TreeNode* p = root;
    // 首先搜索到需要删除的节点
    if (!root) return;
    while (true){
        if (p->data == Delete) {
            bool hasL = !p->left_tag && p->left;
            bool hasR = !p->right_tag && p->right;
            if (!hasL && !hasR) {
                // 叶节点，删除并修改前驱和后继节点
                TreeNode* prev = prev_find(p);
                TreeNode* next = next_find(p);
                if (prev) {
                    // 有两种可能，一种是通过prev->right_tag连的, 一种是直接的prev->right
                    if (prev->right == p)prev->right_tag = true;
                    prev->right = next;
                }
                if (next) {
                    if (next->left == p)  next->left_tag = true; // 删除左节点时，将虚置为true
                    next->left = prev;
                }
                // 这样修改叶节点
                delete p;
                return;
            }
            else if (hasL && hasR) { // 对于左右节点均存在
                // 首先访问前面的一个节点,然后检查p是否为可删节点并执行删除操作
                TreeNode* prev = prev_find(p);
                int temp = prev->data;
                delete_elem(root, prev->data); // 删除这个节点
                p->data = temp;
            }
            else{ // 有一颗子树，那么将其子树挪放到该节点原先的位置即可(不需要删除节点，只需要将子树特性copy即可)
                if (hasL) {
                    p->data = p->left->data;
                    TreeNode* d = p->left;

                    TreeNode* prev = prev_find(p->left);
                    
                    // p->right 不变(因为有right_tag)
                    // 查找L部分的前驱节点并修改其后继指向 (但是right以及next不做修改)
                    if (prev && prev->right_tag) prev->right = p;
                    p->left = p->left->left;

                    delete d;
                }

                if (hasR) {
                    p->data = p->right->data;

                    TreeNode* d = p->right;
                    TreeNode* next = next_find(p->right);
                    // p->left 以及left_tag不变
                    // 查找R部分的后继节点并修改其前驱指向 (left和prev不做修改)
                    if (next && next->left_tag) next->left = p;
                    p->right = p->right->right;

                    delete d;
                }
            }
            return;
        }
        else if ( p->data > Delete) {
            if (p->left_tag) return; // 说明已经没有左子树
            p = p->left;
        }
        else if ( p->data < Delete) {
            if (p->right_tag) return;
            p = p->right;
        }
    }
}

int main_sc() {
    TreeNode* root = create_tree();
    midThread(root);
    

    vector<DataType> res1, res2;

    DataType insert,dele;
    
    cin >> insert;
    insert_elem(root, insert);
    mid_travel(root, res1);
    
    cin >> dele;
    delete_elem(root, dele);
    mid_travel(root, res2);

    for (int i = 0; i < res1.size(); i++) {
        cout << res1[i] << " ";
    }
    cout << endl;

    for (int i = 0; i < res2.size(); i++) {
        cout << res2[i] << " ";
    }
    cout << endl;

    return 0;
}


/* TestCode1
12 8 4 -1 -1 10 -1 -1 16 13 -1 -1 18 -1 -1
6
16
// 也可以是12
*/

/* Test Code2
45 24 12 -1 -1 37 -1 -1 53 -1 93 -1 -1
24
*/
```