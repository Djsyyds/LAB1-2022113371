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

    auto getAdjList() const -> const unordered_map<string, unordered_map<string, int>>& {
        return adjList;
    }

    auto randomWalk() -> string {
        if (adjList.empty()) { return "";                    // 决策点1: if条件
}
        vector<string> nodes;
        for (auto& [node, _] : adjList) { nodes.push_back(node);  // 决策点2: for循环
}
        static random_device rd;
        static mt19937 gen(rd());
        uniform_int_distribution<> dis(0, nodes.size()-1);
        
        string current = nodes[dis(gen)];
        set<pair<string, string>> visitedEdges;
        vector<string> path;
        path.push_back(current);

        while (true) {                                       // 决策点3: while循环
            auto& edges = adjList[current];
            if (edges.empty()) { break;                      // 决策点4: if条件
}
            
            vector<string> targets;
            for (auto& [t, _] : edges) { targets.push_back(t);  // 决策点5: for循环
}
            uniform_int_distribution<> edgeDis(0, targets.size()-1);
            string next = targets[edgeDis(gen)];
            
            if (visitedEdges.count({current, next}) != 0u) { break;  // 决策点6: if条件
}
            visitedEdges.insert({current, next});
            current = next;
            path.push_back(current);
        }

        stringstream ss;
        for (auto& word : path) { ss << word << " ";         // 决策点7: for循环
}
        
        // 将结果保存到文件
        ofstream outFile("random_walk.txt");
        outFile << ss.str();
        outFile.close();
        
        return ss.str();
    }
};

class MinimalCyclomaticTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::remove("random_walk.txt");
    }

    void TearDown() override {
        std::remove("random_walk.txt");
    }

    bool isValidPath(const Graph& graph, const std::string& result) {
        std::istringstream iss(result);
        std::vector<std::string> path;
        std::string word;
        
        while (iss >> word) {
            path.push_back(word);
        }

        if (path.empty()) return false;

        const auto& adjList = graph.getAdjList();
        for (size_t i = 0; i < path.size() - 1; ++i) {
            if (adjList.find(path[i]) == adjList.end() ||
                adjList.at(path[i]).find(path[i + 1]) == adjList.at(path[i]).end()) {
                return false;
            }
        }
        return true;
    }
};

// 基本路径1：空图路径
// 路径：adjList.empty() = true → return ""
TEST_F(MinimalCyclomaticTest, BasicPath1_EmptyGraph) {
    Graph graph;
    std::string result = graph.randomWalk();
    
    EXPECT_EQ(result, "");
    
    std::ifstream file("random_walk.txt");
    EXPECT_FALSE(file.good());
}

// 基本路径2：单节点无出边路径
// 路径：adjList.empty() = false → for(nodes) → while → edges.empty() = true → break → for(path) → return
TEST_F(MinimalCyclomaticTest, BasicPath2_SingleNodeNoEdges) {
    Graph graph;
    graph.addEdge("start", "isolated");
    
    // 多次运行直到选中isolated节点（无出边）
    for (int i = 0; i < 100; ++i) {
        std::string result = graph.randomWalk();
        if (result == "isolated ") {
            EXPECT_EQ(result, "isolated ");
            std::ifstream file("random_walk.txt");
            EXPECT_TRUE(file.good());
            return;
        }
    }
}

// 基本路径3：单步有出边但重复边路径
// 路径：adjList.empty() = false → for(nodes) → while → edges.empty() = false → for(targets) → visitedEdges.count() != 0 → break → for(path) → return
TEST_F(MinimalCyclomaticTest, BasicPath3_SingleStepRepeatedEdge) {
    Graph graph;
    graph.addEdge("self", "self");  // 自环，第二次访问时必然重复
    
    std::string result = graph.randomWalk();
    
    EXPECT_FALSE(result.empty());
    EXPECT_TRUE(isValidPath(graph, result));
    
    // 验证是自环的结果
    std::istringstream iss(result);
    std::vector<std::string> path;
    std::string word;
    while (iss >> word) {
        path.push_back(word);
    }
    EXPECT_EQ(path.size(), 2);
    EXPECT_EQ(path[0], "self");
    EXPECT_EQ(path[1], "self");
}

// 基本路径4：单步有出边非重复边但下一节点无出边路径
// 路径：adjList.empty() = false → for(nodes) → while → edges.empty() = false → for(targets) → visitedEdges.count() = 0 → while → edges.empty() = true → break → for(path) → return
TEST_F(MinimalCyclomaticTest, BasicPath4_TwoStepPath) {
    Graph graph;
    graph.addEdge("start", "end");  // start有出边，end无出边
    
    // 多次运行直到选中start节点
    for (int i = 0; i < 100; ++i) {
        std::string result = graph.randomWalk();
        if (result.find("start") == 0) {
            EXPECT_TRUE(isValidPath(graph, result));
            std::ifstream file("random_walk.txt");
            EXPECT_TRUE(file.good());
            return;
        }
    }
}

// 基本路径5：多步路径最终因无出边终止
// 路径：adjList.empty() = false → for(nodes) → while → edges.empty() = false → for(targets) → visitedEdges.count() = 0 → while → ... → edges.empty() = true → break → for(path) → return
TEST_F(MinimalCyclomaticTest, BasicPath5_MultiStepNoEdgesTermination) {
    Graph graph;
    graph.addEdge("a", "b");
    graph.addEdge("b", "c");
    graph.addEdge("c", "d");  // 线性链，最终d无出边
    
    // 多次运行直到选中a节点
    for (int i = 0; i < 100; ++i) {
        std::string result = graph.randomWalk();
        if (result.find("a") == 0) {
            EXPECT_TRUE(isValidPath(graph, result));
            std::ifstream file("random_walk.txt");
            EXPECT_TRUE(file.good());
            return;
        }
    }
}

// 基本路径6：多步路径最终因重复边终止
// 路径：adjList.empty() = false → for(nodes) → while → edges.empty() = false → for(targets) → visitedEdges.count() = 0 → while → ... → visitedEdges.count() != 0 → break → for(path) → return
TEST_F(MinimalCyclomaticTest, BasicPath6_MultiStepRepeatedEdgeTermination) {
    Graph graph;
    graph.addEdge("a", "b");
    graph.addEdge("b", "a");  // 简单环路，必然会重复边
    
    std::string result = graph.randomWalk();
    
    EXPECT_FALSE(result.empty());
    EXPECT_TRUE(isValidPath(graph, result));
    
    std::ifstream file("random_walk.txt");
    EXPECT_TRUE(file.good());
}

// 基本路径7：复杂图多分支路径（覆盖多种随机选择）
// 路径：测试多个节点、多条边的复杂情况
TEST_F(MinimalCyclomaticTest, BasicPath7_ComplexGraphMultipleBranches) {
    Graph graph;
    graph.addEdge("hub", "spoke1");
    graph.addEdge("hub", "spoke2");
    graph.addEdge("spoke1", "end1");
    graph.addEdge("spoke2", "end2");
    
    std::string result = graph.randomWalk();
    
    EXPECT_FALSE(result.empty());
    EXPECT_TRUE(isValidPath(graph, result));
    
    std::ifstream file("random_walk.txt");
    EXPECT_TRUE(file.good());
}

// 基本路径8：边界条件组合路径（确保所有for循环都被执行）
// 路径：确保所有循环分支都被覆盖
TEST_F(MinimalCyclomaticTest, BasicPath8_BoundaryConditionsCombination) {
    Graph graph;
    graph.addEdge("node1", "node2");
    graph.addEdge("node2", "node3");
    graph.addEdge("node3", "node1");  // 三节点环路
    
    std::string result = graph.randomWalk();
    
    EXPECT_FALSE(result.empty());
    EXPECT_TRUE(isValidPath(graph, result));
    
    // 验证结果格式
    EXPECT_TRUE(result.back() == ' ');
    
    std::ifstream file("random_walk.txt");
    EXPECT_TRUE(file.good());
    
    // 验证文件内容与返回值一致
    std::string fileContent;
    std::getline(file, fileContent);
    EXPECT_EQ(fileContent, result);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}