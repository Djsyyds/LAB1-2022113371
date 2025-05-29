#!/bin/bash

echo "=== RandomWalk 函数覆盖率报告生成器 ==="

# 清理之前的文件
echo "清理之前的覆盖率文件..."
rm -f *.gcda *.gcno *.gcov
rm -rf coverage_html_report
rm -f coverage_report.html coverage.info coverage_filtered.info
rm -f randomwalk_coverage.html

# 编译测试程序
echo "编译测试程序..."
g++ -std=c++17 -fprofile-arcs -ftest-coverage -lgtest -lgtest_main -pthread minimal_cyclomatic_test.cpp -o minimal_test

if [ $? -ne 0 ]; then
    echo "❌ 编译失败！"
    exit 1
fi

echo "✅ 编译成功"

# 运行测试
echo "运行测试..."
./minimal_test

if [ $? -ne 0 ]; then
    echo "❌ 测试运行失败！"
    exit 1
fi

echo "✅ 测试运行成功"

# 检查是否安装了 gcovr
if command -v gcovr &> /dev/null; then
    echo "📊 使用 gcovr 生成HTML报告..."
    
    # 生成基本HTML报告
    gcovr --html --html-details --branches \
          --html-title "RandomWalk Function - 8 Basic Paths Coverage Analysis" \
          --html-medium-threshold 75 \
          --html-high-threshold 90 \
          -o randomwalk_coverage.html
    
    if [ $? -eq 0 ]; then
        echo "✅ gcovr 报告已生成: randomwalk_coverage.html"
    else
        echo "❌ gcovr 报告生成失败"
    fi
    
    # 生成JSON数据（可选）
    gcovr --json -o coverage_data.json
    
else
    echo "⚠️  gcovr 未安装，跳过 gcovr 报告生成"
    echo "   安装命令: sudo apt-get install gcovr 或 pip install gcovr"
fi

# 检查是否安装了 lcov
if command -v lcov &> /dev/null; then
    echo "📊 使用 lcov 生成HTML报告..."
    
    # 收集覆盖率数据
    lcov --capture --directory . --output-file coverage.info --rc lcov_branch_coverage=1
    
    if [ $? -eq 0 ]; then
        # 过滤系统文件
        lcov --remove coverage.info '/usr/*' '*/gtest/*' '*/googletest/*' \
             --output-file coverage_filtered.info
        
        # 生成HTML报告
        genhtml coverage_filtered.info \
                --output-directory coverage_html_report \
                --title "RandomWalk Coverage Report - 8 Basic Paths" \
                --show-details \
                --legend \
                --branch-coverage \
                --function-coverage \
                --rc genhtml_hi_limit=90 \
                --rc genhtml_med_limit=75
        
        if [ $? -eq 0 ]; then
            echo "✅ lcov 报告已生成: coverage_html_report/index.html"
        else
            echo "❌ lcov 报告生成失败"
        fi
    else
        echo "❌ lcov 数据收集失败"
    fi
    
else
    echo "⚠️  lcov 未安装，跳过 lcov 报告生成"
    echo "   安装命令: sudo apt-get install lcov"
fi

# 生成覆盖率摘要
echo ""
echo "=== 覆盖率摘要 ==="
if command -v gcovr &> /dev/null; then
    echo "📈 总体覆盖率统计:"
    gcovr --branches
fi

echo ""
echo "=== 报告生成完成 ==="
echo ""
echo "📁 生成的文件:"
if [ -f "randomwalk_coverage.html" ]; then
    echo "  🌐 gcovr HTML报告: randomwalk_coverage.html"
    echo "     打开命令: firefox randomwalk_coverage.html"
fi
if [ -d "coverage_html_report" ]; then
    echo "  🌐 lcov HTML报告:  coverage_html_report/index.html"
    echo "     打开命令: firefox coverage_html_report/index.html"
fi
if [ -f "coverage_data.json" ]; then
    echo "  📄 JSON数据:      coverage_data.json"
fi

echo ""
echo "🎯 这些报告展示了8个基本路径测试用例对randomWalk函数的覆盖情况"
echo "   应该能看到接近100%的行覆盖率和分支覆盖率"

# 自动打开报告（可选）
read -p "是否自动打开HTML报告? (y/n): " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    if [ -f "randomwalk_coverage.html" ]; then
        echo "🚀 打开 gcovr 报告..."
        xdg-open randomwalk_coverage.html 2>/dev/null || \
        firefox randomwalk_coverage.html 2>/dev/null || \
        google-chrome randomwalk_coverage.html 2>/dev/null || \
        echo "请手动打开: randomwalk_coverage.html"
    elif [ -d "coverage_html_report" ]; then
        echo "🚀 打开 lcov 报告..."
        xdg-open coverage_html_report/index.html 2>/dev/null || \
        firefox coverage_html_report/index.html 2>/dev/null || \
        google-chrome coverage_html_report/index.html 2>/dev/null || \
        echo "请手动打开: coverage_html_report/index.html"
    fi
fi

echo "✨ 完成！" 