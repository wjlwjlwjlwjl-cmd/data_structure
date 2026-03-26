#include <vector>

class UnionFindSet
{
public:
	UnionFindSet(size_t size)
		: _ufs(size, -1)
	{
	}

	int FindRoot(int index)
	{ 
		while (_ufs[index] >= 0)
		{
			index = _ufs[index];
		}
		return index;
	}
	bool Union(int x1, int x2)
	{
		int root1 = FindRoot(x1);
		int root2 = FindRoot(x2);

		if (root1 == root2)
			return false;

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