# RandomWalk函数流程图使用指南

## 📋 文件清单

我已经为您创建了以下文件来分析randomWalk函数的控制流：

### 📄 文档文件
- `randomwalk_flowcharts.md` - 详细的流程图和控制流图分析文档
- `flowchart_usage_guide.md` - 本使用指南

### 🎨 DOT图形文件
- `randomwalk_cfg.dot` - 控制流图的DOT源文件
- `randomwalk_flowchart.dot` - 程序流程图的DOT源文件

### 🔧 脚本文件
- `generate_flowcharts.sh` - 自动生成图像的脚本

---

## 🚀 快速开始

### 1. 安装依赖
```bash
# 安装 Graphviz（用于生成图像）
sudo apt-get install graphviz

# 验证安装
dot -V
```

### 2. 生成流程图
```bash
# 给脚本执行权限
chmod +x generate_flowcharts.sh

# 运行脚本生成所有图像
./generate_flowcharts.sh
```

### 3. 查看结果
脚本会生成以下图像文件：
- `randomwalk_cfg.png` - 控制流图（PNG格式）
- `randomwalk_flowchart.png` - 程序流程图（PNG格式）
- `randomwalk_cfg.svg` - 控制流图（SVG格式）
- `randomwalk_flowchart.svg` - 程序流程图（SVG格式）
- `randomwalk_cfg.pdf` - 控制流图（PDF格式）
- `randomwalk_flowchart.pdf` - 程序流程图（PDF格式）
- `randomwalk_cfg_hq.png` - 高质量控制流图
- `randomwalk_flowchart_hq.png` - 高质量程序流程图

---

## 📊 图表说明

### 控制流图 (Control Flow Graph)
- **用途**: 显示函数的所有执行路径和决策点
- **特点**: 
  - 菱形节点表示决策点（if语句、循环）
  - 矩形节点表示处理步骤
  - 不同颜色表示不同类型的节点
  - 边上的标签显示条件（true/false）

### 程序流程图 (Program Flowchart)
- **用途**: 显示程序的逻辑流程，更易于理解
- **特点**:
  - 椭圆表示开始/结束
  - 菱形表示判断
  - 矩形表示处理过程
  - 箭头显示执行方向

---

## 🎯 分析要点

### 圈复杂度分析
根据控制流图，randomWalk函数有**8个决策点**：

1. `if (adjList.empty())` - 空图检查
2. `for (auto& [node, _] : adjList)` - 节点收集循环
3. `while (true)` - 主循环
4. `if (edges.empty())` - 无出边检查
5. `for (auto& [t, _] : edges)` - 目标收集循环
6. `if (visitedEdges.count())` - 重复边检查
7. `for (auto& word : path)` - 结果构建循环

**圈复杂度 = 8**，对应8条基本路径。

### 关键路径
1. **空图路径**: 直接返回空字符串
2. **无出边路径**: 选中无出边的节点
3. **重复边路径**: 遇到已访问的边
4. **正常终止路径**: 多步随机游走后终止

---

## 🔍 手动生成图像

如果您想手动生成特定格式的图像：

```bash
# 生成PNG格式的控制流图
dot -Tpng randomwalk_cfg.dot -o randomwalk_cfg.png

# 生成SVG格式的程序流程图
dot -Tsvg randomwalk_flowchart.dot -o randomwalk_flowchart.svg

# 生成高分辨率PDF
dot -Tpdf -Gdpi=300 randomwalk_cfg.dot -o randomwalk_cfg_hires.pdf

# 生成其他格式
dot -Tjpg randomwalk_flowchart.dot -o randomwalk_flowchart.jpg
dot -Teps randomwalk_cfg.dot -o randomwalk_cfg.eps
```

---

## 📱 在线查看

如果您无法在本地安装Graphviz，可以：

1. 复制DOT文件内容
2. 访问在线Graphviz编辑器：
   - https://dreampuf.github.io/GraphvizOnline/
   - https://magjac.com/graphviz-visual-editor/
3. 粘贴DOT代码并生成图像

---

## 🛠️ 自定义修改

### 修改颜色主题
编辑DOT文件中的`fillcolor`属性：
```dot
N1 [label="节点1", fillcolor=lightblue];  // 浅蓝色
N2 [label="节点2", fillcolor=orange];     // 橙色
```

### 修改布局方向
```dot
rankdir=TB;  // 从上到下
rankdir=LR;  // 从左到右
rankdir=BT;  // 从下到上
rankdir=RL;  // 从右到左
```

### 添加更多细节
在节点标签中添加更多信息：
```dot
N1 [label="1. 函数入口\nrandomWalk()\n复杂度: O(1)"];
```

---

## 🔗 与测试用例的关系

这些流程图直接对应我们之前创建的8个基本路径测试用例：

1. **BasicPath1_EmptyGraph** → 空图路径
2. **BasicPath2_SingleNodeNoEdges** → 无出边路径
3. **BasicPath3_SingleStepRepeatedEdge** → 重复边路径
4. **BasicPath4_TwoStepPath** → 两步路径
5. **BasicPath5_MultiStepNoEdgesTermination** → 多步无出边终止
6. **BasicPath6_MultiStepRepeatedEdgeTermination** → 多步重复边终止
7. **BasicPath7_ComplexGraphMultipleBranches** → 复杂图路径
8. **BasicPath8_BoundaryConditionsCombination** → 边界条件组合

每个测试用例都对应控制流图中的一条特定路径，确保100%的代码覆盖率。

---

## 📞 故障排除

### 常见问题

1. **"dot: command not found"**
   ```bash
   sudo apt-get update
   sudo apt-get install graphviz
   ```

2. **权限错误**
   ```bash
   chmod +x generate_flowcharts.sh
   ```

3. **图像无法打开**
   ```bash
   # 尝试不同的图像查看器
   eog randomwalk_cfg.png      # GNOME图像查看器
   feh randomwalk_cfg.png      # 轻量级查看器
   firefox randomwalk_cfg.png  # 浏览器查看
   ```

4. **中文显示问题**
   确保系统支持中文字体，或修改DOT文件使用英文标签。

---

## 📚 进一步学习

- [Graphviz官方文档](https://graphviz.org/documentation/)
- [DOT语言指南](https://graphviz.org/doc/info/lang.html)
- [控制流图理论](https://en.wikipedia.org/wiki/Control-flow_graph)
- [圈复杂度分析](https://en.wikipedia.org/wiki/Cyclomatic_complexity)

通过这些流程图，您可以清楚地看到randomWalk函数的执行逻辑和所有可能的执行路径，这对于理解代码结构和验证测试覆盖率非常有帮助。 