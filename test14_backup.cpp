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

class TC004_LinearTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::remove("random_walk.txt");
    }

    void TearDown() override {
        std::remove("random_walk.txt");
    }
};

/**
 * 测试用例编号: TC004
 * 输入数据: 线性二节点图 (adjList = {"start": {"end": 1}, "end": {}})
 * 期望输出: "start end " 或 "end "（取决于随机起点）
 * 覆盖路径: 路径4 - 两步后无出边终止路径
 * 路径流程: 行165→166(false)→169→180→182(false)→186→191(false)→193-195→180→182(true)→183→199→207
 * 决策覆盖: 行166(false), 行182(false→true), 行191(false)
 */
TEST_F(TC004_LinearTest, LinearTwoNodesPath) {
    // 输入数据：创建线性二节点图
    Graph graph;
    graph.addEdge("start", "end"); // start有出边，end无出边
    
    // 执行被测函数多次，验证可能的输出
    bool foundStartEnd = false;
    bool foundEnd = false;
    
    for (int i = 0; i < 100; ++i) {
        std::string result = graph.randomWalk();
        
        if (result == "start end ") {
            foundStartEnd = true;
            
            // 验证文件创建和内容
            std::ifstream file("random_walk.txt");
            EXPECT_TRUE(file.good()) << "应该创建random_walk.txt文件";
            
            std::string fileContent;
            std::getline(file, fileContent);
            EXPECT_EQ(fileContent, "start end ") << "文件内容应该与返回值一致";
        } else if (result == "end ") {
            foundEnd = true;
        }
        
        // 验证结果格式
        EXPECT_TRUE(result == "start end " || result == "end ") 
            << "结果应该是'start end '或'end '，实际是: " << result;
    }
    
    // 由于随机性，可能只出现其中一种情况，这是正常的
    EXPECT_TRUE(foundStartEnd || foundEnd) << "应该至少出现一种预期结果";
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}