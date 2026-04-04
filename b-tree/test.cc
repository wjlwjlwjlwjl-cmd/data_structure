#include "b-tree.h"
#include <set>
#include <chrono>
#include <iostream>
#include <vector>
#include <random>

using namespace std;
using namespace chrono;

int main() {
    const int DATA_SIZE = 1000000;
    const int TEST_TIMES = 1000000; // 查100万次，时间才准！

    // 生成随机查找目标（避免缓存干扰）
    vector<int> targets;
    targets.reserve(TEST_TIMES);
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist(0, DATA_SIZE - 1);
    for (int i = 0; i < TEST_TIMES; ++i) {
        targets.push_back(dist(rng));
    }

    cout << "Building structures... " << endl;

    // 构建 B树
    BTree<int> btree;
    for (int i = 0; i < DATA_SIZE; i++) {
        btree.Insert(i);
    }

    // 构建 set
    set<int> st;
    for (int i = 0; i < DATA_SIZE; i++) {
        st.insert(i);
    }

    // ====================== 测试 B树 ======================
    auto start = high_resolution_clock::now();
    for (int k : targets) {
        btree.Find(k);
    }
    auto end = high_resolution_clock::now();
    auto b_time = duration_cast<microseconds>(end - start).count();

    // ====================== 测试 set ======================
    start = high_resolution_clock::now();
    for (int k : targets) {
        st.find(k);
    }
    end = high_resolution_clock::now();
    auto st_time = duration_cast<microseconds>(end - start).count();

    // ====================== 输出 ======================
    cout << "B树    总耗时：" << b_time << " μs\n";
    cout << "set    总耗时：" << st_time << " μs\n";
    cout << "B树 速度提升：" << (double)st_time / b_time << " 倍\n";
    btree.Inorder();
}