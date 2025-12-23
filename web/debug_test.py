import subprocess
import os

# 1. 检查 exe 是否存在
exe_path = os.path.join(os.getcwd(), 'main.exe')
print(f"正在检查: {exe_path}")

if not os.path.exists(exe_path):
    print("错误: 找不到 main.exe！")
    exit()

print("找到 main.exe，开始尝试调用获取图书列表...")

try:
    # 2. 尝试调用 C++，设置 5 秒超时
    # 如果 C++ 里有 cin >> 或者是死循环，这里会报错
    result = subprocess.run(
        [exe_path, "-api_show_books"],
        capture_output=True,
        text=True,
        encoding='gbk',     # 如果乱码改成 utf-8
        errors='ignore',
        timeout=5           # 【关键】只等5秒，超时就报错
    )

    print("-" * 20)
    print("调用成功！返回代码:", result.returncode)
    print("【C++ 标准输出内容】:\n", result.stdout)
    print("-" * 20)

    if not result.stdout.strip():
        print("警告: C++ 返回的内容是空的！")
    else:
        # 简单测试一下解析
        lines = result.stdout.strip().split('\n')
        print(f"C++ 返回了 {len(lines)} 行数据。")

except subprocess.TimeoutExpired:
    print("\n【严重错误】: C++ 程序卡死了！")
    print("原因: 你的 C++ 代码里可能包含 'cin >>' 或 'system(\"pause\")'。")
    print("解决方法: 必须去掉所有用户交互代码。")
except Exception as e:
    print(f"发生其他错误: {e}")