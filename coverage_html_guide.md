# 生成HTML覆盖率报告指南

## 方法一：使用 gcovr（推荐）

### 1. 安装 gcovr
```bash
# Ubuntu/Debian
sudo apt-get install gcovr

# CentOS/RHEL
sudo yum install gcovr

# 或使用 pip 安装
pip install gcovr
```

### 2. 编译测试程序
```bash
# 编译时添加覆盖率标志
g++ -std=c++17 -fprofile-arcs -ftest-coverage -lgtest -lgtest_main -pthread minimal_cyclomatic_test.cpp -o minimal_test
```

### 3. 运行测试
```bash
# 运行测试生成覆盖率数据
./minimal_test
```

### 4. 生成HTML报告
```bash
# 生成HTML覆盖率报告
gcovr --html --html-details -o coverage_report.html

# 或者指定源文件目录
gcovr --html --html-details -o coverage_report.html .

# 生成更详细的报告
gcovr --html --html-details --html-title "RandomWalk Function Coverage Report" -o coverage_report.html
```

### 5. 查看报告
```bash
# 在浏览器中打开报告
firefox coverage_report.html
# 或
google-chrome coverage_report.html
# 或
xdg-open coverage_report.html
```

---

## 方法二：使用 lcov + genhtml

### 1. 安装 lcov
```bash
# Ubuntu/Debian
sudo apt-get install lcov

# CentOS/RHEL
sudo yum install lcov
```

### 2. 编译和运行测试
```bash
# 编译
g++ -std=c++17 -fprofile-arcs -ftest-coverage -lgtest -lgtest_main -pthread minimal_cyclomatic_test.cpp -o minimal_test

# 运行测试
./minimal_test
```

### 3. 生成覆盖率数据
```bash
# 收集覆盖率数据
lcov --capture --directory . --output-file coverage.info

# 过滤掉系统头文件和测试框架文件
lcov --remove coverage.info '/usr/*' '*/gtest/*' --output-file coverage_filtered.info
```

### 4. 生成HTML报告
```bash
# 生成HTML报告
genhtml coverage_filtered.info --output-directory coverage_html_report

# 生成更详细的报告
genhtml coverage_filtered.info --output-directory coverage_html_report --title "RandomWalk Coverage Report" --show-details --legend
```

### 5. 查看报告
```bash
# 打开主页面
firefox coverage_html_report/index.html
```

---

## 方法三：完整的自动化脚本

创建一个自动化脚本来生成覆盖率报告：

### coverage_report.sh
```bash
#!/bin/bash

echo "=== 生成覆盖率报告 ==="

# 清理之前的文件
echo "清理之前的覆盖率文件..."
rm -f *.gcda *.gcno *.gcov
rm -rf coverage_html_report
rm -f coverage_report.html coverage.info coverage_filtered.info

# 编译测试程序
echo "编译测试程序..."
g++ -std=c++17 -fprofile-arcs -ftest-coverage -lgtest -lgtest_main -pthread minimal_cyclomatic_test.cpp -o minimal_test

if [ $? -ne 0 ]; then
    echo "编译失败！"
    exit 1
fi

# 运行测试
echo "运行测试..."
./minimal_test

if [ $? -ne 0 ]; then
    echo "测试运行失败！"
    exit 1
fi

# 检查是否安装了 gcovr
if command -v gcovr &> /dev/null; then
    echo "使用 gcovr 生成HTML报告..."
    gcovr --html --html-details --html-title "RandomWalk Function Coverage Report" -o coverage_report.html
    echo "gcovr 报告已生成: coverage_report.html"
fi

# 检查是否安装了 lcov
if command -v lcov &> /dev/null; then
    echo "使用 lcov 生成HTML报告..."
    
    # 收集覆盖率数据
    lcov --capture --directory . --output-file coverage.info
    
    # 过滤系统文件
    lcov --remove coverage.info '/usr/*' '*/gtest/*' --output-file coverage_filtered.info
    
    # 生成HTML报告
    genhtml coverage_filtered.info --output-directory coverage_html_report --title "RandomWalk Coverage Report" --show-details --legend
    
    echo "lcov 报告已生成: coverage_html_report/index.html"
fi

echo "=== 覆盖率报告生成完成 ==="
echo ""
echo "查看报告："
if [ -f "coverage_report.html" ]; then
    echo "  gcovr 报告: firefox coverage_report.html"
fi
if [ -d "coverage_html_report" ]; then
    echo "  lcov 报告:  firefox coverage_html_report/index.html"
fi
```

### 使用脚本
```bash
# 给脚本执行权限
chmod +x coverage_report.sh

# 运行脚本
./coverage_report.sh
```

---

## 高级选项和自定义

### gcovr 高级选项
```bash
# 生成包含分支覆盖率的报告
gcovr --html --html-details --branches -o coverage_report.html

# 排除特定文件
gcovr --html --html-details --exclude '.*test.*' -o coverage_report.html

# 设置覆盖率阈值
gcovr --html --html-details --html-medium-threshold 75 --html-high-threshold 90 -o coverage_report.html

# 生成JSON格式的数据
gcovr --json -o coverage.json
```

### lcov 高级选项
```bash
# 生成带有函数覆盖率的报告
genhtml coverage_filtered.info --output-directory coverage_html_report --function-coverage

# 设置覆盖率阈值
genhtml coverage_filtered.info --output-directory coverage_html_report --rc genhtml_hi_limit=90 --rc genhtml_med_limit=75

# 生成更详细的分支覆盖率
lcov --capture --directory . --output-file coverage.info --rc lcov_branch_coverage=1
genhtml coverage_filtered.info --output-directory coverage_html_report --branch-coverage
```

---

## 报告内容说明

### HTML报告通常包含：

1. **总体覆盖率统计**
   - 行覆盖率 (Line Coverage)
   - 函数覆盖率 (Function Coverage)
   - 分支覆盖率 (Branch Coverage)

2. **文件级别覆盖率**
   - 每个源文件的覆盖率百分比
   - 可点击查看详细信息

3. **代码级别详情**
   - 高亮显示已覆盖/未覆盖的代码行
   - 分支覆盖情况
   - 执行次数统计

4. **颜色编码**
   - 绿色：已覆盖
   - 红色：未覆盖
   - 黄色：部分覆盖（分支）

---

## 针对 randomWalk 函数的特定命令

```bash
# 完整的覆盖率报告生成流程
echo "=== RandomWalk 函数覆盖率分析 ==="

# 1. 编译
g++ -std=c++17 -fprofile-arcs -ftest-coverage -lgtest -lgtest_main -pthread minimal_cyclomatic_test.cpp -o minimal_test

# 2. 运行测试
./minimal_test

# 3. 生成详细的HTML报告
gcovr --html --html-details --branches --html-title "RandomWalk Function - 8 Basic Paths Coverage" -o randomwalk_coverage.html

# 4. 打开报告
firefox randomwalk_coverage.html
```

这样您就可以获得详细的HTML格式覆盖率报告，清楚地看到8个基本路径测试用例对randomWalk函数的覆盖情况。 