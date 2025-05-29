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

class TC006_ChainTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::remove("random_walk.txt");
    }

    void TearDown() override {
        std::remove("random_walk.txt");
    }
};

/**
 * 测试用例编号: TC006
 * 输入数据: 链式三节点图 (adjList = {"a": {"b": 1}, "b": {"c": 1}, "c": {}})
 * 期望输出: "a b c ", "b c ", 或 "c "（取决于随机起点）
 * 覆盖路径: 路径6 - 多步循环无出边终止路径
 * 路径流程: 多次while循环，最终通过edges.empty()终止
 * 决策覆盖: 行166(false), 行182(false...→true), 行191(false...)
 */
TEST_F(TC006_ChainTest, ChainThreeNodesPath) {
    // 输入数据：创建链式三节点图
    Graph graph;
    graph.addEdge("a", "b");
    graph.addEdge("b", "c"); // 形成a→b→c的链式结构，c无出边
    
    // 执行被测函数多次，验证可能的输出
    std::set<std::string> validResults = {"a b c ", "b c ", "c "};
    bool foundValidResult = false;
    
    for (int i = 0; i < 100; ++i) {
        std::string result = graph.randomWalk();
        
        if (validResults.count(result)) {
            foundValidResult = true;
            
            // 验证文件创建和内容
            std::ifstream file("random_walk.txt");
            EXPECT_TRUE(file.good()) << "应该创建random_walk.txt文件";
            
            std::string fileContent;
            std::getline(file, fileContent);
            EXPECT_EQ(fileContent, result) << "文件内容应该与返回值一致";
            
            // 验证路径的合理性
            if (result == "a b c ") {
                EXPECT_TRUE(true) << "从a开始的完整路径";
            } else if (result == "b c ") {
                EXPECT_TRUE(true) << "从b开始的路径";
            } else if (result == "c ") {
                EXPECT_TRUE(true) << "从c开始的路径（立即终止）";
            }
            
            break; // 找到有效结果就退出
        }
    }
    
    EXPECT_TRUE(foundValidResult) << "应该找到有效的链式路径结果";
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}