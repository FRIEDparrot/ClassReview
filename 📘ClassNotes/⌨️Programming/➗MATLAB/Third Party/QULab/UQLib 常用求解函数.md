`uq_gradient(mu_, g)` 相当于自己写的diff_function, 用于求解某个函数的导数
`[xmin, fmin] = uq_gso(g,[], 7, mu_ - 5*sigma_d, mu_ + 5 * sigma_d)` % 网格搜索函数, 方法一是第二个给定网格, 方法二是如此给定边界
`uq_ceo`          交叉熵优化算法寻找最小值, 需要初始高斯分布的均值和标准差

