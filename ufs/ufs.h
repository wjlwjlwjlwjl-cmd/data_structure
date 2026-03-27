#pragma once
#include <vector>
#include <iostream>

class UnionFindSet
{
public:
	UnionFindSet(size_t size)
		: _ufs(size, -1)
	{
	}

	//在这里可以进行优化，将多层合并为一层，麻烦一次往后都快
	int FindRoot(int index)
	{ 
		int root = index;
		while (_ufs[root] >= 0)
		{
			root = _ufs[root];
		}

		//压缩路径
		while (_ufs[index] >= 0) {
			int parent = _ufs[index];
			_ufs[index] = root;
			index = parent;
		}
		return root;
	}

	bool Union(int x1, int x2)
	{
		int root1 = FindRoot(x1);
		int root2 = FindRoot(x2);

		if (root1 == root2)
			return false;

		//控制小的往大的合并
		if (std::abs(_ufs[root1]) < std::abs(_ufs[root2]))
		{
			std::swap(root1, root2);
		}
		_ufs[root1] += _ufs[root2];
		_ufs[root2] = root1;
		return true;
	}

	size_t Count() const
	{
		size_t count = 0;
		for (auto e : _ufs)
		{
			if (e < 0)
				++count;
		}
		return count;
	}

private:
	std::vector<int> _ufs;
};