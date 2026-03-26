### 并查集

#### 核心思想

把具有同种属性的对象都放在一起，并在其中找到任意一个对象作为他们的代表，这样我们在处理问题时，在建立好的并查集中，只要能够找到相同的代表，那么就是具有同种属性的值

#### 实现原理

可以采用类似与堆的方法，通过数组 + 下标建立我们的并查集，具体原理如下图所示



<img src="C:\GitFiles\data_structure\graph\ufs_prin.png" alt="ufs principle" style="zoom:50%;" />



实现集合之间合并的原理如下图所示



<img src="C:\GitFiles\data_structure\graph\ufs_union.png" style="zoom:50%;" />



> 关于压缩路径的问题
>
> 1. 在上面的例子中，如果合并大小写字母，那么找root时就需要找两次，再合并到数字的话就是要找三次，当集合增多的话可能会出现找root耗费时间太长的问题，所以可以在查找时可以将节点都合并到一层，这样慢一次往后就都快了（合并时正常合并，因为不会往前判断是不是再往前还有root）
> 2. 但是其实没有很大的性能要求的情况下，压缩路径并没有太大的必要，直接正常合并就可以

#### 接口与方法

##### 1. 给定两个原始下标，将两个对象合并到同一个集合

```cpp
	void Union(int x1, int x2) {
		int parent2 = FindParent(x2);
		int parent1 = FindParent(x1);
		if (parent1 == parent2) {
			return;
		}
		_ufs[parent1] += _ufs[parent2];
		_ufs[parent2] = parent1;
	}
```

> 这里还需要注意的是，还需要提供两个工具函数，一个是寻找某个元素的root，另一个是通过找根判断两个元素是否已经在同一集合中（当然直接通过比较找到的根是否相同也可以）

```cpp
	size_t FindParent(int x) {
		int parent = x;
		while (_ufs[x] >= 0) {
			parent = _ufs[root];
		}
		return parent;
	}
```

##### 2. 统计集合的个数

```cpp
	size_t SetSize() {
		int size = 0;
		for (auto& e : _ufs) {
			if (e < 0) {
				size++;
			}
		}
		return size;
	}
```

##### 完整代码

```cpp
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
```

#### 并查集的应用

[990. 等式方程的可满足性 - 力扣（LeetCode）](https://leetcode.cn/problems/satisfiability-of-equality-equations/description/)

```cpp
class Solution {
public:
    bool equationsPossible(vector<string>& equations) {
        int n = equations.size();
        vector<int> ufs(26, -1);
        auto find_parent = [&ufs](int x){
            int parent = x;
            while(ufs[parent] >= 0){
                parent = ufs[parent];
            }
            return parent;
        };

        for(int i = 0; i < n; i++){
            int a = equations[i][0] - 'a';
            int b = equations[i][3] - 'a';
            int parent1 = find_parent(a);
            int parent2 = find_parent(b);
            if(equations[i][1] == '=' && parent1 != parent2){
                ufs[parent1] += ufs[parent2];
                ufs[parent2] = parent1;
            }
        }

        for(int i = 0; i < n; i++){
            int a = equations[i][0] - 'a';
            int b = equations[i][3] - 'a';
            int parent1 = find_parent(a);
            int parent2 = find_parent(b);
            if(equations[i][1] == '!' && parent1 == parent2){
                return false;
            }
        }
        return true;
    }
};
```

[LCR 116. 省份数量 - 力扣（LeetCode）](https://leetcode.cn/problems/bLyHh0/description/)

```cpp
class Solution {
public:
    int findCircleNum(vector<vector<int>>& isConnected) {
        int m = isConnected.size();
        int n = isConnected[0].size();
        vector<int> ufs(m, -1);

        auto find_root = [&ufs](int x){
            int root = x;
            while(ufs[root] >= 0){
                root = ufs[root];
            }
            return root;
        };

        for(int i = 0; i < m; i++){
            for(int j = 0; j < n; j++){
                if(isConnected[i][j] == 1){
                    int root1 = find_root(i);
                    int root2 = find_root(j);
                    if(root1 != root2){
                        ufs[root1] += ufs[root2];
                        ufs[root2] = root1;
                    }
                }
            }
        }

        int ret = 0;
        for(auto& e: ufs){
            if(e < 0){
                ret++;
            }
        }
        return ret;
    }
};
```

