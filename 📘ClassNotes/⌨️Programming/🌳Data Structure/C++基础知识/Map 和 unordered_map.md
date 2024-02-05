
```cpp
vector<int> twoSum(vector<int>& nums, int target) {
	map <int, int> has;  
	// C++ 中的map是红黑树查找,时间的复杂度是logn
	// 可以使用哈希表unordered_map来寻找,时间复杂度为O(1)
	int n = nums.size();
	for (int i = 0; i < n; ++i) {
		// 对于红黑树中的每一个元素
		if (has.find(target - nums[i]) != has.end()) {
			// find函数，如果找到返回指向关键词的迭代器，否则指向end的迭代器
			return {has[target - nums[i]], i};
			// 返回这两个数的下标
		}
		has[nums[i]] = i;
		// 没有找到，则将nums[i]的下标置为i,即将数字加入红黑树
	}

}
```