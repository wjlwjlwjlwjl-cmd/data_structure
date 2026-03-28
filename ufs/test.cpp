#include "ufs.h"
#include "graph.h"

void TestMatrixGraph() {
	graph_matrix::Graph<char, int, INT_MAX, true> g("0123", 4);
	g.addEdge('0', '1', 1);
	g.addEdge('0', '3', 4);
	g.addEdge('1', '3', 2);
	g.addEdge('1', '2', 9);
	g.addEdge('2', '3', 8);
	g.addEdge('2', '1', 5);
	g.addEdge('2', '0', 3);
	g.addEdge('3', '2', 6);
	g.Print();
}

void TestGraphBDFS() {
	std::string a[] = { "zhangsan", "lisi", "wangwu", "zhaoliu", "zhouqi"};
	graph_matrix::Graph<std::string, int> g1(a, sizeof(a) / sizeof(std::string));
	g1.addEdge("zhangsan", "lisi", 100);
	g1.addEdge("zhangsan", "wangwu", 200);
	g1.addEdge("wangwu", "zhaoliu", 30);
	g1.addEdge("wangwu", "zhouqi", 30);
	g1.BFS("zhangsan");
	g1.DFS("zhangsan");
	auto g2 = g1;
	g1.Kruskal(g2);
	std::cout << "--------------------------" << std::endl;
	g2.Print();
	
}

void TestTableGraph() {
	std::string a[] = { "zhangsan", "lisi", "wangwu", "zhaoliu" };

	graph_table::Graph<std::string, int> g1(a, 4);
	g1.AddEdge("zhangsan", "lisi", 100);
	g1.AddEdge("zhangsan", "wangwu", 200);
	g1.AddEdge("wangwu", "zhaoliu", 30);
	g1.Print();
}

void TestMinTree() {
	const char* str = "abcdefghi";
	graph_matrix::Graph<char, int> g(str, strlen(str));
	g.addEdge('a', 'b', 4);
	g.addEdge('a', 'h', 8);
	g.addEdge('b', 'c', 8);
	g.addEdge('b', 'h', 11);
	g.addEdge('c', 'i', 2);
	g.addEdge('c', 'f', 4);
	g.addEdge('c', 'd', 7);
	g.addEdge('d', 'f', 14);
	g.addEdge('d', 'e', 9);
	g.addEdge('e', 'f', 10);
	g.addEdge('f', 'g', 2);
	g.addEdge('g', 'h', 1);
	g.addEdge('g', 'i', 6);
	g.addEdge('h', 'i', 7);
	graph_matrix::Graph<char, int> kminTree1;
	std::cout << "Kruskal:" << g.Kruskal(kminTree1) << std::endl;
	kminTree1.Print();

	std::cout << std::endl << std::endl << std::endl;

	graph_matrix::Graph<char, int> kminTree2;
	std::cout << "Prim:" << g.Prim(kminTree2, 'a') << std::endl;
	kminTree2.Print();
}

int main() {
	UnionFindSet ufs(5);
	//TestTableGraph();
	//TestMatrixGraph();
	//TestGraphBDFS();
	TestMinTree();
	return 0;
}