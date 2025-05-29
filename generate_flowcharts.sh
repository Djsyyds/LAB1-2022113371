#!/bin/bash

echo "=== RandomWalk 函数流程图生成器 ==="

# 检查是否安装了 graphviz
if ! command -v dot &> /dev/null; then
    echo "❌ Graphviz 未安装！"
    echo "   安装命令: sudo apt-get install graphviz"
    exit 1
fi

echo "✅ Graphviz 已安装"

# 生成控制流图
if [ -f "randomwalk_cfg.dot" ]; then
    echo "📊 生成控制流图..."
    dot -Tpng randomwalk_cfg.dot -o randomwalk_cfg.png
    dot -Tsvg randomwalk_cfg.dot -o randomwalk_cfg.svg
    dot -Tpdf randomwalk_cfg.dot -o randomwalk_cfg.pdf
    echo "✅ 控制流图已生成:"
    echo "   - PNG格式: randomwalk_cfg.png"
    echo "   - SVG格式: randomwalk_cfg.svg"
    echo "   - PDF格式: randomwalk_cfg.pdf"
else
    echo "❌ 找不到 randomwalk_cfg.dot 文件"
fi

# 生成程序流程图
if [ -f "randomwalk_flowchart.dot" ]; then
    echo "📊 生成程序流程图..."
    dot -Tpng randomwalk_flowchart.dot -o randomwalk_flowchart.png
    dot -Tsvg randomwalk_flowchart.dot -o randomwalk_flowchart.svg
    dot -Tpdf randomwalk_flowchart.dot -o randomwalk_flowchart.pdf
    echo "✅ 程序流程图已生成:"
    echo "   - PNG格式: randomwalk_flowchart.png"
    echo "   - SVG格式: randomwalk_flowchart.svg"
    echo "   - PDF格式: randomwalk_flowchart.pdf"
else
    echo "❌ 找不到 randomwalk_flowchart.dot 文件"
fi

# 生成高质量版本
echo "📊 生成高质量版本..."
if [ -f "randomwalk_cfg.dot" ]; then
    dot -Tpng -Gdpi=300 randomwalk_cfg.dot -o randomwalk_cfg_hq.png
    echo "✅ 高质量控制流图: randomwalk_cfg_hq.png"
fi

if [ -f "randomwalk_flowchart.dot" ]; then
    dot -Tpng -Gdpi=300 randomwalk_flowchart.dot -o randomwalk_flowchart_hq.png
    echo "✅ 高质量程序流程图: randomwalk_flowchart_hq.png"
fi

echo ""
echo "=== 生成完成 ==="
echo ""
echo "📁 生成的文件:"
ls -la *.png *.svg *.pdf 2>/dev/null | grep -E "(randomwalk_cfg|randomwalk_flowchart)"

echo ""
echo "🖼️  查看图像:"
echo "   控制流图:   xdg-open randomwalk_cfg.png"
echo "   程序流程图: xdg-open randomwalk_flowchart.png"

# 询问是否自动打开图像
read -p "是否自动打开生成的图像? (y/n): " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    if [ -f "randomwalk_cfg.png" ]; then
        echo "🚀 打开控制流图..."
        xdg-open randomwalk_cfg.png 2>/dev/null &
    fi
    if [ -f "randomwalk_flowchart.png" ]; then
        echo "🚀 打开程序流程图..."
        xdg-open randomwalk_flowchart.png 2>/dev/null &
    fi
fi

echo "✨ 完成！" 