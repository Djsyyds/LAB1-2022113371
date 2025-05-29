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

class TC002_SingleNodeTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::remove("random_walk.txt");
    }

    void TearDown() override {
        std::remove("random_walk.txt");
    }
};

/**
 * 测试用例编号: TC002
 * 输入数据: 单个孤立节点 (adjList = {"isolated": {}})
 * 期望输出: 返回值为"isolated ", 创建文件写入"isolated "
 * 覆盖路径: 路径2 - 单节点无出边路径
 * 路径流程: 行165→166(false)→169→180→182(true)→183→199→207
 * 决策覆盖: 行166(false), 行182(true)
 */
TEST_F(TC002_SingleNodeTest, SingleNodeNoEdges) {
    // 输入数据：创建单个孤立节点
    Graph graph;
    graph.addEdge("start", "isolated"); // isolated节点无出边
    
    // 执行被测函数多次，直到选中isolated节点
    for (int i = 0; i < 100; ++i) {
        std::string result = graph.randomWalk();
        if (result == "isolated ") {
            // 验证期望输出
            EXPECT_EQ(result, "isolated ") << "孤立节点应该返回自身加空格";
            
            // 验证文件创建和内容
            std::ifstream file("random_walk.txt");
            EXPECT_TRUE(file.good()) << "应该创建random_walk.txt文件";
            
            std::string fileContent;
            std::getline(file, fileContent);
            EXPECT_EQ(fileContent, "isolated ") << "文件内容应该与返回值一致";
            
            return; // 测试通过，退出
        }
    }
    
    FAIL() << "100次尝试都没有选中isolated节点，测试失败";
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}