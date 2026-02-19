import os

# 设置你的代码文件所在的目录
source_dir = r"D:\Github\Ataxx\Introduction-to-computing-Ataxx-master\Introduction-to-computing-Ataxx-master\source"  # 请根据实际路径修改
output_file = "merged.cpp"  # 输出合并后的文件名

# 获取所有 .h 和 .cpp 文件
header_files = sorted([f for f in os.listdir(source_dir) if f.endswith(".h")])
cpp_files = sorted([f for f in os.listdir(source_dir) if f.endswith(".cpp")])

merged_code = "// Merged C++ Code\n\n"

# 处理头文件，避免重复 include
included_headers = set()
for file in header_files:
    with open(os.path.join(source_dir, file), "r", encoding="utf-8") as f:
        lines = f.readlines()
    
    for line in lines:
        if line.strip().startswith("#include"):
            header_name = line.strip().split()[1]
            if header_name in included_headers:
                continue
            included_headers.add(header_name)
        
        merged_code += line
    merged_code += "\n"

# 处理 CPP 文件
for file in cpp_files:
    with open(os.path.join(source_dir, file), "r", encoding="utf-8") as f:
        lines = f.readlines()
    
    for line in lines:
        # 忽略对头文件的 include
        if not line.strip().startswith("#include"):
            merged_code += line
    merged_code += "\n"

# 写入合并后的文件
with open(output_file, "w", encoding="utf-8") as f:
    f.write(merged_code)

print(f"合并完成，文件保存为 {output_file}")