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

class TC008_ComplexTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::remove("random_walk.txt");
    }

    void TearDown() override {
        std::remove("random_walk.txt");
    }
};

/**
 * 测试用例编号: TC008
 * 输入数据: 复杂多分支图 (hub有多个分支，每个分支有终端节点)
 * 期望输出: 各种可能的路径组合
 * 覆盖路径: 路径8 - 边界条件组合路径
 * 路径流程: 综合测试所有循环和判定的组合情况
 * 决策覆盖: 所有决策点的不同组合
 */
TEST_F(TC008_ComplexTest, ComplexMultiBranchPath) {
    // 输入数据：创建复杂多分支图
    Graph graph;
    graph.addEdge("hub", "s1");
    graph.addEdge("hub", "s2");
    graph.addEdge("s1", "e1");
    graph.addEdge("s2", "e2");
    // 形成树状分支结构: hub -> {s1, s2}, s1 -> e1, s2 -> e2
    
    // 定义所有可能的有效路径
    std::set<std::string> validResults = {
        "hub s1 e1 ",  // hub -> s1 -> e1
        "hub s2 e2 ",  // hub -> s2 -> e2
        "s1 e1 ",      // s1 -> e1
        "s2 e2 ",      // s2 -> e2
        "e1 ",         // e1 (无出边)
        "e2 "          // e2 (无出边)
    };
    
    // 执行多次测试，收集不同的结果
    std::set<std::string> actualResults;
    
    for (int i = 0; i < 100; ++i) {
        std::string result = graph.randomWalk();
        actualResults.insert(result);
        
        // 验证每个结果都是有效的
        EXPECT_TRUE(validResults.count(result)) 
            << "无效的路径结果: " << result;
        
        // 验证文件创建和内容
        std::ifstream file("random_walk.txt");
        EXPECT_TRUE(file.good()) << "应该创建random_walk.txt文件";
        
        std::string fileContent;
        std::getline(file, fileContent);
        EXPECT_EQ(fileContent, result) << "文件内容应该与返回值一致";
    }
    
    // 验证至少出现了一些不同的路径（由于随机性）
    EXPECT_GE(actualResults.size(), 1) << "应该至少出现一种路径";
    
    // 验证路径的结构合理性
    for (const auto& result : actualResults) {
        std::istringstream iss(result);
        std::vector<std::string> path;
        std::string word;
        while (iss >> word) {
            path.push_back(word);
        }
        
        // 验证路径长度合理
        EXPECT_GE(path.size(), 1) << "路径至少应该包含1个节点";
        EXPECT_LE(path.size(), 3) << "在此图结构中，路径长度不应超过3";
        
        // 验证节点有效性
        std::set<std::string> validNodes = {"hub", "s1", "s2", "e1", "e2"};
        for (const auto& node : path) {
            EXPECT_TRUE(validNodes.count(node)) << "路径中包含无效节点: " << node;
        }
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}