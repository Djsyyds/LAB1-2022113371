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

class TC003_SelfLoopTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::remove("random_walk.txt");
    }

    void TearDown() override {
        std::remove("random_walk.txt");
    }
};

/**
 * 测试用例编号: TC003
 * 输入数据: 自环图 (adjList = {"self": {"self": 1}})
 * 期望输出: 返回值为"self self ", 创建文件写入"self self "
 * 覆盖路径: 路径3 - 第一步重复边路径
 * 路径流程: 行165→166(false)→169→180→182(false)→186→191(true)→192→199→207
 * 决策覆盖: 行166(false), 行182(false), 行191(true)
 */
TEST_F(TC003_SelfLoopTest, SelfLoopRepeatedEdge) {
    // 输入数据：创建自环图
    Graph graph;
    graph.addEdge("self", "self"); // 自环，第二次访问时必然重复
    
    // 执行被测函数
    std::string result = graph.randomWalk();
    
    // 验证期望输出
    EXPECT_EQ(result, "self self ") << "自环应该产生两个相同节点";
    
    // 验证文件创建和内容
    std::ifstream file("random_walk.txt");
    EXPECT_TRUE(file.good()) << "应该创建random_walk.txt文件";
    
    std::string fileContent;
    std::getline(file, fileContent);
    EXPECT_EQ(fileContent, "self self ") << "文件内容应该与返回值一致";
    
    // 验证路径长度（应该正好是2个节点）
    std::istringstream iss(result);
    std::vector<std::string> path;
    std::string word;
    while (iss >> word) {
        path.push_back(word);
    }
    EXPECT_EQ(path.size(), 2) << "自环路径应该包含2个节点";
    EXPECT_EQ(path[0], "self") << "第一个节点应该是self";
    EXPECT_EQ(path[1], "self") << "第二个节点应该是self";
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}