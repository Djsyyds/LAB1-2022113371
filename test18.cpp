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