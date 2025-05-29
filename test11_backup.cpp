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

class TC001_EmptyGraphTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::remove("random_walk.txt");
    }

    void TearDown() override {
        std::remove("random_walk.txt");
    }
};

TEST_F(TC001_EmptyGraphTest, EmptyGraphDirectReturn) {
    
    Graph graph;
    
    
    
    std::string result = graph.randomWalk();
    
    
    EXPECT_EQ(result, "") << "空图应该返回空字符串";
    
    
    std::ifstream file("random_walk.txt");
    EXPECT_FALSE(file.good()) << "空图情况下不应该创建random_walk.txt文件";
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}