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

template <class V, class W, int MAX = INT_MAX, bool directional = false>
class Graph
{
public:
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

private:
    std::vector<V> _vertexs;              // 存储所有顶点
    std::unordered_map<V, int> _indexMap; // 存储顶点下标
    std::vector<std::vector<W>> _matrix;  // 邻接矩阵
};