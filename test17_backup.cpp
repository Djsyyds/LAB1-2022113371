#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <random>
#include <sstream>
#include <set>
#include <cstdio>

using namespace std;

class Graph {
private:
    unordered_map<string, unordered_map<string, int>> adjList;

public:
    void addEdge(const string& src, const string& dest) {
        adjList[src][dest]++;
    }

    auto randomWalk() -> string {
        if (adjList.empty()) { return ""; }
        vector<string> nodes;
        for (auto& [node, _] : adjList) { nodes.push_back(node); }
        static random_device rd;
        static mt19937 gen(rd());
        uniform_int_distribution<> dis(0, nodes.size()-1);
        
        string current = nodes[dis(gen)];
        set<pair<string, string>> visitedEdges;
        vector<string> path;
        path.push_back(current);

        while (true) {
            auto& edges = adjList[current];
            if (edges.empty()) { break; }
            
            vector<string> targets;
            for (auto& [t, _] : edges) { targets.push_back(t); }
            uniform_int_distribution<> edgeDis(0, targets.size()-1);
            string next = targets[edgeDis(gen)];
            
            if (visitedEdges.count({current, next}) != 0u) { break; }
            visitedEdges.insert({current, next});
            current = next;
            path.push_back(current);
        }

        stringstream ss;
        for (auto& word : path) { ss << word << " "; }
        
        ofstream outFile("random_walk.txt");
        outFile << ss.str();
        outFile.close();
        
        return ss.str();
    }
};

class TC007_CycleTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::remove("random_walk.txt");
    }

    void TearDown() override {
        std::remove("random_walk.txt");
    }
};

/**
 * 测试用例编号: TC007
 * 输入数据: 三节点环图 (adjList = {"a": {"b": 1}, "b": {"c": 1}, "c": {"a": 1}})
 * 期望输出: 各种可能的路径，如"a b a ", "b c b ", "c a c "等
 * 覆盖路径: 路径7 - 多步循环重复边终止路径
 * 路径流程: 多次while循环，最终通过重复边检测终止
 * 决策覆盖: 行166(false), 行182(false...→false), 行191(false...→true)
 */
TEST_F(TC007_CycleTest, CycleThreeNodesPath) {
    // 输入数据：创建三节点环图
    Graph graph;
    graph.addEdge("a", "b");
    graph.addEdge("b", "c");
    graph.addEdge("c", "a"); // 形成a→b→c→a的环形结构
    
    // 执行被测函数
    std::string result = graph.randomWalk();
    
    // 验证结果不为空
    EXPECT_FALSE(result.empty()) << "环形图应该产生非空路径";
    
    // 验证文件创建和内容
    std::ifstream file("random_walk.txt");
    EXPECT_TRUE(file.good()) << "应该创建random_walk.txt文件";
    
    std::string fileContent;
    std::getline(file, fileContent);
    EXPECT_EQ(fileContent, result) << "文件内容应该与返回值一致";
    
    // 解析路径
    std::istringstream iss(result);
    std::vector<std::string> path;
    std::string word;
    while (iss >> word) {
        path.push_back(word);
    }
    
    // 验证路径长度（环形图中，路径应该在检测到重复边时终止）
    EXPECT_GE(path.size(), 2) << "环形图路径至少应该包含2个节点";
    
    // 验证路径中的节点都是有效的
    std::set<std::string> validNodes = {"a", "b", "c"};
    for (const auto& node : path) {
        EXPECT_TRUE(validNodes.count(node)) << "路径中包含无效节点: " << node;
    }
    
    // 验证路径的连通性（相邻节点应该有边连接）
    for (size_t i = 0; i < path.size() - 1; ++i) {
        const std::string& current = path[i];
        const std::string& next = path[i + 1];
        
        bool hasEdge = false;
        if (current == "a" && next == "b") hasEdge = true;
        if (current == "b" && next == "c") hasEdge = true;
        if (current == "c" && next == "a") hasEdge = true;
        
        EXPECT_TRUE(hasEdge) << "路径中" << current << "->" << next << "没有对应的边";
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}