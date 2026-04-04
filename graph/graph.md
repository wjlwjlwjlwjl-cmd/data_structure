## 图

### 1. 基本概念

* 图是由顶点集合及顶点间的关系组成的一组数据结构，图中的节点称为顶点；两个顶点相关联称作两个顶点之间有一条边
* 图分为有向图和无向图。就像不同类型的社交软件一样，强社交关系的软件，比如微信，你是我的好友，那么我就是你的好友，类似于无向图；而比如抖音、B站等等，你关注我不等于我关注你，类似于有向图
* 我们学过的树，比如AVL树，红黑树，都可以认为是一种特殊的图
* **完全图**。在n个顶点的**无向图**中，若有`n * (n - 1) / 2`条边，即任意两个顶点之间有且仅有一条边，那么这个图就是无向完全图；在有n个顶点的**有向图**中，若有`n * (n - 1)`条边，即任意两个顶点之间有且仅有两个方向相反的边，那么这个图就是有向完全图
* **邻接顶点**。简单来说就是两个被一条边直接连起来的点
* **顶点的度**。对于有向图而言，为该点入读和出度之和，所谓入度即该点是多少条边的终点，所谓出度就是该点是多少条边的起点；对于无向图而言，顶点的度=入度=出度
* **路径**。能够从一个顶点到达另一个顶点的一组边
* **路径长度**。对于不带权的图，即路径中边的个数；对于带权的图，即路径中各个边权值总和
* **简单路径与回路**。如果一个路径上各个顶点均不重复，则这样的路径为简单路径；若路径上第一个顶点和最后一个顶点重合，则这样的路径为回路

> 在后面判断回路时，比如最小生成树的Kruskal算法，就可以采用并查集的方式来判断是否连接某两个点后会产生回路

* **子图**。顶点都是原来的图中的顶点，边都是原来的图中的边，则这个图就是原来的图的子图
* **连通图**。在无向图中，若两个点之间有路径，那么这两个顶点之间就是联通的；如果图中任意两个节点都是连通的，那么就说这个图是连通图
* **强连通图**。在有向图中，任意两个节点之间都存在到彼此的路径，那么这个图就是强连通图

### 2. 图的存储结构

#### 方式一：邻接矩阵

比如说一个图中有n个节点，那么就可以用一个`n*n`的二维数组来记录任意两个节点之间的连接情况。对于带有权值的图，可以直接使用他来记录两个顶点之间的权值，并规定一个无穷大值表示两点之间没有路径；对于不带权值的图，可以直接使用0、1表示是否连接



**关于对称问题**。无向图的邻接矩阵是左右对称的，所以在后面的代码中可以只看一半；有向图的邻接矩阵不一定对称

##### 邻接矩阵的优点

(1) 适合表示稠密图

(2) 可以很方便的判断两个顶点是否相连(O(1)的时间复杂度)

##### 邻接矩阵的缺点

(1) 不适合表示稀疏图，空间浪费比较大

(2) 不适合寻找一个顶点连接的所有顶点

```cpp
namespace graph_matrix {
	template <class V, class W, int MAX = INT_MAX, bool directional = false>
	class Graph
	{
	public:
		using Self = Graph<V, W, MAX, directional>;
		Graph(){}

		Graph(const V *vals, int sz)
		{
			_vertexs.reserve(sz);
			for (int i = 0; i < sz; i++)
			{
				_vertexs.push_back(vals[i]);
				_indexMap[vals[i]] = i;
			}

			_matrix.resize(sz);
			for (int i = 0; i < sz; i++)
			{
				_matrix[i] = std::vector<W>(sz, MAX);
			}
		}

		int getVertexIndex(const V target)
		{
			auto it = _indexMap.find(target);
			if (it == _indexMap.end())
			{
				throw std::invalid_argument("wrong vertex val");
				return -1;
			}
			else
			{
				return it->second;
			}
		}

		void addEdge(const V src, const V des, W w)
		{
			int srci = getVertexIndex(src);
			int desi = getVertexIndex(des);
			_matrix[srci][desi] = w;
			if (directional == false)
			{
				_matrix[desi][srci] = w;
			}
		}
	private:
		std::vector<V> _vertexs;              
		std::unordered_map<V, int> _indexMap; 
		std::vector<std::vector<W>> _matrix;  
	};
}

```

#### 邻接表

邻接表采取的是类似于哈希桶的方式，每个顶点都对应链表数组的一个位置，每个位置指向一个链表，链表中存储着这个节点连接的所有顶点的信息



对于无向图，连接的两个顶点彼此都要记录，对于有向图，只需在起点的链表中记录即可



##### 邻接表的优点

1. 适合存储稀疏图
2. 可以快速寻找一个顶点所有连接的顶点

##### 邻接表的缺点

不适合判断两个顶点的连接情况

```cpp
namespace graph_table {
	template <class W>
	struct edge {
		int index;
		W w;
		struct edge* next;
		edge(int index, W w) 
			: index(index), w(w), next(nullptr)
		{}
	};

	template <class V, class W, bool directional = false>
	class Graph {
	public:
		Graph(const V* vals, int n) {
			_vertexs.reserve(n);
			for (int i = 0; i < n; i++) {
				_vertexs.push_back(vals[i]);
				_indexMap[vals[i]] = i;
			}

			_edges.resize(n, nullptr);
		}

		int GetVertexIndex(const V val) {
			auto it = _indexMap.find(val);
			if (it == _indexMap.end()) {
				throw std::invalid_argument("wrong index");
				return -1;
			}
			else {
				return it->second;
			}
		}

		void AddEdge(const V src, const V des, const W w) {
			int srci = GetVertexIndex(src);
			int desi = GetVertexIndex(des);

			edge<W>* new_e = new edge(desi, w);
			new_e->next = _edges[srci];
			_edges[srci] = new_e;

			if (!directional) {
				edge<W>* new_e = new edge(srci, w);
				new_e->next = _edges[desi];
				_edges[desi] = new_e;
			}
		}
	private:
		std::vector<V> _vertexs; 
		std::unordered_map<V, int> _indexMap; 
		std::vector<edge<W>*> _edges; 
	};
}
```

>邻接矩阵和邻接表其实是互补的关系，都有自己适合的情况，但相对而言使用邻接矩阵可能会多一些

 

### 图的遍历

#### 1. BFS，图的广度优先遍历

简单来说，就是一层一层的去往外遍历，就像水的涟漪一样，从中心往外扩散，广度优先遍历就是遍历同一层的顶点，再接着遍历下一层



**实现方式**，使用队列来保存每一层的顶点和每一层的每个顶点带出的下一层的顶点，并在开始遍历新的一层时保存下这一层的大小，来做层之间的区分；使用一个`vector<bool>`来判断是否已经遍历过某个顶点

```cpp
void BFS(const V val) {
    int src = getVertexIndex(val);
    int n = _vertexs.size();
    std::queue<int> q;
    q.push(src);
    std::vector<bool> check(n, false);
    check[src] = true;

    int d = 0;
    while (q.size()) {
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            auto front = q.front(); q.pop();
            for (int k = 0; k < n; k++) {
                if (!check[k] && _matrix[front][k] != MAX) {
                    q.push(k);
                    check[k] = true;
                }
            }
        }
    }
}

```

#### 2. DFS，深度优先遍历

一般采取递归的方式实现，从起点开始，依次向起点连接的每个点往下一条道走到黑，最后把所有顶点连接的点都做完DFS后，就完成深度优先遍历了

```cpp
namespace graph_matrix{
public:
    class Graph{
		void DFS(const V val) {
			int src = getVertexIndex(val);
			std::vector<bool> check(_vertexs.size(), false);
			check[src] = true;
			_dfs(src, check);
		}
	private:
		std::vector<V> _vertexs;              // 存储所有顶点
		std::unordered_map<V, int> _indexMap; // 存储顶点下标
		std::vector<std::vector<W>> _matrix;  // 邻接矩阵

		void _dfs(int src, std::vector<bool>& check) {
			std::cout << _vertexs[src] << std::endl;
			for (int i = 0; i < _vertexs.size(); i++) {
				if (!check[i] && _matrix[src][i] != MAX) {
					check[i] = true;
					_dfs(i, check);
				}
			}
		}
	};
}

```

### 最小生成树

首先最小生成树是一棵连通树，有n个顶点和`(n-1)`条边。删除任意一条边都不再是一棵连通树，同时添加任意一条边也都会形成一个回路



所以最小生成树应满足以下三点要求

1. 只能使用图中的边来构造最小生成树（子图）
2. 恰好使用`(n-1)`条边去链接n个顶点
3. 选用的`(n-1)`条边不能构成回路

构造最小生成树的方法有两种：Kruskal算法、Prim算法，两者都使用了贪心的策略

#### Kruskal算法

采用的是全局贪心的算法，即：在一个图的所有边中，只要不构成环，我就总是选最小的边，直到选完`n-1`条边，或者无法构成最小生成树

##### 步骤

* 将所有的边放到一个小根堆中，这样每次取堆顶就能保证总是取到剩下的边中的最小的边
* 取到一个边后，使用并查集去判断是否这个边的两顶点已经在一个集合中（想象一下，如果已经在的话，再连接这两个点那必然会导致构成环），如果不在一个集合中的话，就连接这两个顶点，并添加到同一集合当中
* 接下来再取堆顶元素，以此往复，直到选完了`n-1`条边或者堆为空

##### 代码实现

```cpp
W Kruskal(Self& self) {
    int n = _vertexs.size();
    self._vertexs = _vertexs;
    self._indexMap = _indexMap;
    self._matrix = _matrix;

    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> pq;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if(_matrix[i][j] != MAX)
                pq.push(Edge(i, j, _matrix[i][j]));
        }
    }

    int edgeCount = 0;
    W totalW = W();
    UnionFindSet ufs(n);
    while (edgeCount != n - 1 && pq.size()) {
        auto min = pq.top();
        pq.pop();
        int srci = min.srci;
        int dsti = min.dsti;
        W w = min.w;
        if (ufs.FindRoot(srci) != ufs.FindRoot(dsti)) {
            ufs.Union(srci, dsti);
            edgeCount++;
            totalW += w;
            self.addEdge(_vertexs[srci], _vertexs[dsti], w);
        }
    }

    if (edgeCount != n - 1) {
        return W();
    }
    return totalW;
}

```

>时间复杂度：O(n^2*logn)，理论：O(ElogE)

#### Prim算法

采取的是局部贪心的策略，即：每次选择边时，总是选择当前最小生成树的节点的指向树外的所有边中最小的边

##### 步骤

* 采取优先级队列+辅助数组的方式实现
* 首先从给定的起点开始，在已连接、未连接的辅助数组中修改其信息，把它的所有边放到堆中
* 接下来从堆中不断取边，直到选完`n-1`条边或者堆空：如果一个边的另一个顶点未连接，那么就连接这个边，并把这个边向外连接的顶点的所有边再放到堆中，以此往复

##### 代码实现

```cpp
W Prim(Self& self, const V& val) {
    int n = _vertexs.size();
    int srci = getVertexIndex(val);
    self._vertexs = _vertexs;
    self._indexMap = _indexMap;
    self._matrix = _matrix;
    std::vector<bool> X(n, false);
    X[srci] = true;
    std::vector<bool> Y(n, true);
    Y[srci] = false;

    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> pq;
    for (int i = 0; i < n; i++) {
        if (_matrix[srci][i] != MAX) {
            pq.push(Edge(srci, i, _matrix[srci][i]));
        }
    }

    int edgeCount = 0;
    W totalW = W();
    while (edgeCount != n - 1 && pq.size()) {
        auto min = pq.top();
        pq.pop();
        int srci = min.srci;
        int dsti = min.dsti;
        W w = min.w;
        if (!X[dsti] && Y[dsti]) {
            self.addEdge(_vertexs[srci], _vertexs[dsti], w);
            edgeCount++;
            totalW += w;
            X[dsti] = true;
            Y[dsti] = false;
            for (int i = 0; i < n; i++) {
                if (_matrix[dsti][i] != MAX) {
                    pq.push(Edge(dsti, i, _matrix[dsti][i]));
                }
            }
        }
    }

    if (edgeCount != n - 1) {
        return W();
    }
    return totalW;
}
```

> 时间复杂度：O(N^2*logN)，理论：O(ElogV)，

### 最短路径

从带权有向图中的某一顶点出发，找出一条通往另一顶点的最短路径，使得这条路径上权值和最小



在下面的实现中，都是用了`dist`和`ppath`两个数组。前者用来表示当前点距离起点的距离，后者表示当前点到起点的最短路径中，当前点的前一个点是谁



最短路径的问题可以分为两类，一种是单源最短路径问题，可以使用Dijkstra算法和BellmanFord算法解决；另一种是多源最短路径问题，可以使用FloydWarShall算法解决

#### Dijkstra算法

在选取松弛更新的中间点时，采用了贪心策略，每次都选择所有还未确定最小路径的顶点中最小的一个顶点（算法设计能保证这里选到的是最小的，可以直接确定这个距离），将这个点作为中间点去松弛更新其他所有点，以此往复，N个点更新N次



需要注意的是，Dijkstra算法不适用于有负权的情况（因为贪心策略失效了，当前直接选到的顶点的直接相连的边中，即使是最大的也可能会因为负权在后续路径的影响下变为最小的）

##### 代码实现

```cpp
void Dijkstra(const V& val, std::vector<W>& dist, std::vector<int>& pPath) {
    int srci = getVertexIndex(val);
    int n = _vertexs.size();
    dist.resize(n, MAX);
    pPath.resize(n, -1);
    dist[srci] = W();
    std::vector<bool> S(n, false);

    for (int s = 0; s < n; s++) {
        int u = srci;
        int minW = MAX;
        for (int i = 0; i < n; i++) {
            if (!S[i] && minW > dist[i]) {
                u = i;
                minW = dist[i];
            }
        }
        S[u] = true;

        for (int i = 0; i < n; i++) {
            if (!S[i] && _matrix[u][i] != MAX && dist[u] + _matrix[u][i] < dist[i]) {
                dist[i] = dist[u] + _matrix[u][i];
                pPath[i] = u;
            }
        }
    }
}
```

>时间复杂度：O(N^2)，理论O(V^2)

#### BellmanFord算法

解决了Dijkstra算法不能处理负权的情况，当时效率相比Dijkstra算法要差很多，因为是暴力求解，所以避免了负权的影响（通过多次循环来避免对已权值的影响，比如因为负权的出现，使得两顶点之间直接相连的边也不一定是最小的）



总体思路就是用以此把所有点都作为中间点去松弛更新所有点，同时还需要注意负权回路的问题（就是一个回路中权值之和是负数，导致每次走最短路径都不一样，越走越小）

##### 代码实现

```cpp
bool BellmanFord(const V& src, std::vector<W>& dist, std::vector<int>& pPath) {
    int srci = getVertexIndex(src);
    int n = _vertexs.size();
    dist.resize(n, MAX);
    dist[srci] = W();
    pPath.resize(n, -1);

    for (int k = 0; k < n - 1; k++) {
        bool update = false;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (_matrix[i][j] != MAX && dist[i] + _matrix[i][j] < dist[j]) {
                    dist[j] = dist[i] + _matrix[i][j];
                    pPath[j] = i;
                    update = true;
                }
            }
        }
        if (!update) {
            break;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (_matrix[i][j] != MAX && dist[i] + _matrix[i][j] < dist[j]) {
                return false;
            }
        }
    }
    return true;
}

```

> 关于优化，可以采用类似于冒泡排序的方式进行优化，结合前面对负权回路的解释，不难明白，如果重复更新n-1轮之后仍然可以更新，那一定是负权回路导致的，神仙来了也难救，直接放弃求解即可
>
> 时间复杂度：O(N^3)，理论标准O(V*E)

#### FloydWarShall算法

其实本质也是暴力枚举，尝试用所有点作为中间点，作为中间点去松弛更新其他所有点，可以处理负权的情况

##### 代码实现

```cpp
void FloydWarShall(std::vector<std::vector<W>>& vvDist, std::vector<std::vector<int>>& vvPath) {
    int n = _vertexs.size();
    vvDist.resize(n);
    vvPath.resize(n);
    for (int i = 0; i < n; i++) {
        vvDist[i].resize(n, MAX);
        vvPath[i].resize(n, -1);
        vvDist[i][i] = 0;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (_matrix[i][j] != MAX) {
                vvDist[i][j] = _matrix[i][j];
                vvPath[i][j] = i;
            }
        }
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (vvDist[i][k] != MAX && vvDist[k][j] != MAX
                    && vvDist[i][k] + vvDist[k][j] < vvDist[i][j]) {
                    vvDist[i][j] = vvDist[i][k] + vvDist[k][j];
                    vvPath[i][j] = vvPath[k][j];
                }
            }
        }
    }
}

```

>时间复杂度：O(N^3)