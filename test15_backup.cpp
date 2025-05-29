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

class TC005_BidirectionalTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::remove("random_walk.txt");
    }

    void TearDown() override {
        std::remove("random_walk.txt");
    }
};

/**
 * 测试用例编号: TC005
 * 输入数据: 双向二节点图 (adjList = {"a": {"b": 1}, "b": {"a": 1}})
 * 期望输出: "a b " 或 "b a "（取决于随机起点和选择）
 * 覆盖路径: 路径5 - 两步后重复边终止路径
 * 路径流程: 行165→166(false)→169→180→182(false)→186→191(false)→193-195→180→182(false)→186→191(true)→192→199→207
 * 决策覆盖: 行166(false), 行182(false→false), 行191(false→true)
 */
TEST_F(TC005_BidirectionalTest, BidirectionalTwoNodesPath) {
    // 输入数据：创建双向二节点图
    Graph graph;
    graph.addEdge("a", "b");
    graph.addEdge("b", "a"); // 形成a↔b的双向连接
    
    // 执行被测函数
    std::string result = graph.randomWalk();
    
    // 验证期望输出（由于重复边检测，路径长度应该是2）
    EXPECT_TRUE(result == "a b " || result == "b a ") 
        << "结果应该是'a b '或'b a '，实际是: " << result;
    
    // 验证文件创建和内容
    std::ifstream file("random_walk.txt");
    EXPECT_TRUE(file.good()) << "应该创建random_walk.txt文件";
    
    std::string fileContent;
    std::getline(file, fileContent);
    EXPECT_EQ(fileContent, result) << "文件内容应该与返回值一致";
    
    // 验证路径长度（应该正好是2个节点，因为第二步会触发重复边检测）
    std::istringstream iss(result);
    std::vector<std::string> path;
    std::string word;
    while (iss >> word) {
        path.push_back(word);
    }
    EXPECT_EQ(path.size(), 2) << "双向图路径应该包含2个节点（因重复边终止）";
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}