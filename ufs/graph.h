/*存储边的两种主要方式
1. 邻接矩阵
优点：
（1）适合存储稠密图
（2）适合快速判断两个顶点的链接情况——O（N）
缺点：
（1）不适合存储稀疏图
（2）相对而言，不适合判断一个顶点的所有链接顶点——O（N）
2. 邻接表
优点：
（1）适合存储稀疏图
（2）适合快速查找一个顶点连接的所有顶点
缺点：
（1）不适合判断两个顶点是否相连
*/
#include <vector>
#include <unordered_map>
#include <climits>
#include <stdexcept>
#include <iostream>
#include <string>
#include <queue>

#include "ufs.h"

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

		void BFS(const V val) {
			//用来在邻接矩阵中寻找链接关系
			int src = getVertexIndex(val);
			int n = _vertexs.size();
			std::queue<int> q;
			q.push(src);
			std::vector<bool> check(n, false);
			check[src] = true;
			
			int d = 0;
			while (q.size()) {
				int sz = q.size();
				std::cout << d++ << "th layer: \t" << std::endl;
				for (int i = 0; i < sz; i++) {
					auto front = q.front(); q.pop();
					std::cout << _vertexs[front] << "\t";
					for (int k = 0; k < n; k++) {
						if (!check[k] && _matrix[front][k] != MAX) {
							q.push(k);
							check[k] = true;
						}
					}
				}
				std::cout << std::endl << std::endl;
			}
		}

		void DFS(const V val) {
			int src = getVertexIndex(val);
			std::vector<bool> check(_vertexs.size(), false);
			check[src] = true;
			_dfs(src, check);
		}

		void Print()
		{
			// 打印顶点和下标映射关系
			for (int i = 0; i < _vertexs.size(); ++i)
			{
				std::cout << _vertexs[i] << "-" << i << " ";
			}
			std::cout << std::endl << std::endl;
			std::cout << "  ";
			for (int i = 0; i < _vertexs.size(); ++i)
			{
				std::cout << i << " ";
			}
			std::cout << std::endl;
			// 打印矩阵
			for (int i = 0; i < _matrix.size(); ++i)
			{
				std::cout << i << " ";
				for (int j = 0; j < _matrix[i].size(); ++j)
				{
					if (_matrix[i][j] != MAX)
						std::cout << _matrix[i][j] << " ";
					else
						std::cout << "#" << " ";
				}
				std::cout << std::endl;
			}
			std::cout << std::endl << std::endl;
			// 打印所有的边
			for (int i = 0; i < _matrix.size(); ++i)
			{
				for (int j = 0; j < _matrix[i].size(); ++j)
				{
					if (i < j && _matrix[i][j] != MAX)
					{
						std::cout << _vertexs[i] << "-" << _vertexs[j] << ":" << _matrix[i][j] << std::endl;
					}
				}
			}
		}

		struct Edge {
			int srci;
			int dsti;
			W w;
			Edge(int srci, int dsti, W w)
				: srci(srci), dsti(dsti), w(w)
			{ }

			bool operator>(const Edge& e2) const 
			{
				return w > e2.w;
			}
		};

		//最小生成树算法一：克鲁斯卡尔算法
		//每次路径都选最短的一条，通过并查集来确定不成环
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

		// 最小生成树策略二：克里姆算法
		// 进行的是局部贪心的策略
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

		//单源最短路径dijkstra，不适合负权的情况
		//时间复杂度O(N^2)，空间复杂度O(N)
		void Dijkstra(const V& val, std::vector<W>& dist, std::vector<int>& pPath) {
			//dist用来表示起点到某个点的权值之和
			//pPath用来记录最短路径中从起点到当前点的路线中终点前一个点的下表
			//S用来记录当前位置是否已经确定了最小值
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

				//松弛更新
				for (int i = 0; i < n; i++) {
					if (!S[i] && _matrix[u][i] != MAX && dist[u] + _matrix[u][i] < dist[i]) {
						dist[i] = dist[u] + _matrix[u][i];
						pPath[i] = u;
					}
				}
			}
		}

		bool BellmanFord(const V& src, std::vector<W>& dist, std::vector<int>& pPath) {
			int srci = getVertexIndex(src);
			int n = _vertexs.size();
			dist.resize(n, MAX);
			//dist[srci] = W();
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

			//一次尝试用所有点作为中间点，去更新任意两点之间的距离
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

		void PrinrtShotPath(const V& src, const std::vector<W>& dist, const std::vector<int>& parentPath)
		{
			int N = _vertexs.size();
			int srci = getVertexIndex(src);
			for (int i = 0; i < N; ++i)
			{
				if (i == srci)
					continue;
				std::vector<int> path;
				int parenti = i;
				while (parenti != srci)
				{
					path.push_back(parenti);
					parenti = parentPath[parenti];
				}
				path.push_back(srci);
				reverse(path.begin(), path.end());
				for (auto pos : path)
				{
					std::cout << _vertexs[pos] << "->";
				}
				std::cout << dist[i] << std::endl;
			}
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

			if (directional) {
				edge<W>* new_e = new edge(srci, w);
				new_e->next = _edges[desi];
				_edges[desi] = new_e;
			}
		}

		void Print() 
		{
			for (int i = 0; i < _edges.size(); i++) {
				V val = _vertexs[i];
				std::cout << val << "\t -> ";
				auto ptr = _edges[i];
				while (ptr) {
					int index = ptr->index;
					std::cout << _vertexs[index] << "\t -> ";
					ptr = ptr->next;
				}
				std::cout << std::endl;
			}
		}


	private:
		std::vector<V> _vertexs; //存储所有顶点
		std::unordered_map<V, int> _indexMap; //存储顶点对应的下标
		std::vector<edge<W>*> _edges; //邻接表
	};
}