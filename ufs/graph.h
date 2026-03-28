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

		Graph(const V *vals, size_t sz)
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
			for (size_t i = 0; i < _vertexs.size(); ++i)
			{
				std::cout << _vertexs[i] << "-" << i << " ";
			}
			std::cout << std::endl << std::endl;
			std::cout << "  ";
			for (size_t i = 0; i < _vertexs.size(); ++i)
			{
				std::cout << i << " ";
			}
			std::cout << std::endl;
			// 打印矩阵
			for (size_t i = 0; i < _matrix.size(); ++i)
			{
				std::cout << i << " ";
				for (size_t j = 0; j < _matrix[i].size(); ++j)
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
			for (size_t i = 0; i < _matrix.size(); ++i)
			{
				for (size_t j = 0; j < _matrix[i].size(); ++j)
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

			std::priority_queue < Edge, std::vector<Edge>, std::greater<Edge >> pq;
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					if(i < j && _matrix[i][j] != MAX)
						pq.push(Edge(i, j, _matrix[i][j]));
				}
			}

			W total = W();
			int edgeCount = 0;
			UnionFindSet ufs(n);
			while (edgeCount < n - 1 && pq.size()) {
				auto top = pq.top();
				pq.pop();
				int srci = top.srci;
				int dsti = top.dsti;
				if (ufs.FindRoot(srci) != ufs.FindRoot(dsti)) {
					total += top.w;
					edgeCount += 1;
					ufs.Union(srci, dsti);
					self.addEdge(_vertexs[srci], _vertexs[dsti], top.w);
				}
			}

			if (edgeCount != n - 1) {
				return W();
			}
			else {
				return total;
			}
		}

		//克里姆算法进行的是局部贪心的策略
		W Prim(Self& self, const V& val) {
			int n = _vertexs.size();
			self._vertexs = _vertexs;
			self._indexMap = _indexMap;
			self._matrix = _matrix;
			
			int srci = getVertexIndex(val);
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

			int vertexCount = 1;
			W totalW = W();
			while (pq.size()) {
				auto top = pq.top();
				pq.pop();
				int srci = top.srci;
				int dsti = top.dsti;
				W w = top.w;
				if (!X[dsti] && Y[dsti]) {
					X[srci] = true;
					Y[dsti] = false;
					vertexCount++;
					self.addEdge(_vertexs[srci], _vertexs[dsti], w);
					totalW += w;
					for (int i = 0; i < n; i++) {
						if (_matrix[dsti][i] != MAX) {
							pq.push(Edge(dsti, i, _matrix[dsti][i]));
						}
					}
				}
			}

			if (vertexCount != n) {
				return W();
			}
			else {
				return totalW;
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
		Graph(const V* vals, size_t n) {
			_vertexs.reserve(n);
			for (int i = 0; i < n; i++) {
				_vertexs.push_back(vals[i]);
				_indexMap[vals[i]] = i;
			}

			_edges.resize(n, nullptr);
		}

		size_t GetVertexIndex(const V val) {
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