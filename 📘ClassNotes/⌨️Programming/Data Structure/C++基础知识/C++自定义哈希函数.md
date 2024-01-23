
```cpp 
#include <iostream>
#include <unordered_map>

// 自定义哈希函数
struct PairHash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2;
    }
};

int main()
{
    // 定义一个哈希表
    std::unordered_map<std::pair<int, int>, double, PairHash> myMap;

    // 向哈希表中添加元素
    myMap[std::make_pair(1, 2)] = 3.14;
    myMap[std::make_pair(3, 4)] = 2.71;

    // 从哈希表中获取元素
    std::cout << "Value at (1, 2): " << myMap[std::make_pair(1, 2)] << std::endl;
    std::cout << "Value at (3, 4): " << myMap[std::make_pair(3, 4)] << std::endl;

    return 0;
}

```