#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>

//并查集
//存储值为负数的位置是root，存储的值的绝对值是孩子的个数；存储值为正的孩子，存储的值是父亲的数组下标
template <class T>
class UnionFindSet {
public:
	UnionFindSet(size_t sz) 
		: _ufs(sz, -1)
	{}

	//将两个成员所在的集合合并
	void Union(int x1, int x2) {
		int parent2 = FindParent(x2);
		int parent1 = FindParent(x1);
		if (parent1 == parent2) {
			return;
		}
		_ufs[parent1] += _ufs[parent2];
		_ufs[parent2] = parent1;
	}

	//寻找某个成员的根节点
	size_t FindParent(int x) {
		int parent = x;
		while (_ufs[x] >= 0) {
			parent = _ufs[root];
		}
		return parent;
	}

	//判断两个成员是否在一个相同的集合
	bool InSet(int x1, int x2) {
		return FindParent(x1) == FindParent(x2);
	}

	//统计并查集中集合的个数
	size_t SetSize() {
		int size = 0;
		for (auto& e : _ufs) {
			if (e < 0) {
				size++;
			}
		}
		return size;
	}
private:
	std::vector<int> _ufs;
};
//关于压缩路径，多个集合合并在一起时可能会导致高度过高，那么在合并时就可以改变逻辑把多个层合并成一层，这样就能降低找根时的时间复杂度
