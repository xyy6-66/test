import tkinter as tk
from tkinter import ttk, filedialog, messagebox
import matplotlib.pyplot as plt
import time
import pandas as pd

plt.rcParams["font.family"] = ["SimHei"]
plt.rcParams["axes.unicode_minus"] = False

# ---------------------- 全局变量 ----------------------
data = []
capacity = 0
item_count = 0
sorted_data = []
best_value = 0
best_weight = 0
best_selection = []
solve_time = 0

# ---------------------- 1. 读取数据 ----------------------
def load_data():
    global data, capacity, item_count, sorted_data
    path = filedialog.askopenfilename(filetypes=[("Text Files", "*.txt")])
    if not path:
        return
    try:
        with open(path, "r", encoding="utf-8") as f:
            lines = [line.strip() for line in f if line.strip()]

        # 严格按你的格式解析：第1行容量，第2行项集数，后面是项集
        capacity = int(lines[0])
        item_count = int(lines[1])
        data = []
        for line in lines[2:]:
            nums = list(map(int, line.split()))
            w1, v1, w2, v2, w3, v3 = nums
            data.append((w1, v1, w2, v2, w3, v3))

        sorted_data = data.copy()
        messagebox.showinfo("成功", f"读取完成：{item_count} 个项集，容量 {capacity}")
    except Exception as e:
        messagebox.showerror("错误", f"读取失败：{str(e)}")

# ---------------------- 2. 绘制散点图 ----------------------
def plot_graph():
    if not data:
        messagebox.showwarning("提示", "请先读取数据")
        return
    ws, vs = [], []
    for item in data:
        w1, v1, w2, v2, w3, v3 = item
        ws.extend([w1, w2, w3])
        vs.extend([v1, v2, v3])
    plt.figure(figsize=(7, 4))
    plt.scatter(ws, vs, c="blue", alpha=0.6)
    plt.xlabel("重量")
    plt.ylabel("价值")
    plt.title("D{0-1}KP 物品重量-价值散点图")
    plt.grid(True)
    plt.show()

# ---------------------- 3. 按第三项价值重量比 非递增排序 ----------------------
def sort_by_third_ratio():
    global sorted_data
    if not data:
        messagebox.showwarning("提示", "请先读取数据")
        return
    sorted_data = sorted(data, key=lambda x: (x[5] / x[4]) if x[4] != 0 else 0, reverse=True)
    messagebox.showinfo("完成", "已按第三项价值/重量比非递增排序")

# ---------------------- 4. 动态规划求解 D{0-1}KP ----------------------
def solve_dp():
    global best_value, best_weight, best_selection, solve_time
    if not data:
        messagebox.showwarning("提示", "请先读取数据")
        return
    start = time.time()
    C = capacity
    n = len(sorted_data)
    dp = [0] * (C + 1)
    select = [[-1] * (C + 1) for _ in range(n)]

    for i in range(n):
        w1, v1, w2, v2, w3, v3 = sorted_data[i]
        for j in range(C, -1, -1):
            op1 = dp[j]
            op2 = dp[j - w1] + v1 if j >= w1 else -1
            op3 = dp[j - w2] + v2 if j >= w2 else -1
            op4 = dp[j - w3] + v3 if j >= w3 else -1
            mval = max(op1, op2, op3, op4)
            dp[j] = mval
            if mval == op2:
                select[i][j] = 1
            elif mval == op3:
                select[i][j] = 2
            elif mval == op4:
                select[i][j] = 3
            else:
                select[i][j] = 0

    best_value = dp[C]
    j = C
    best_selection = []
    for i in reversed(range(n)):
        s = select[i][j]
        if s == 0:
            continue
        w1, v1, w2, v2, w3, v3 = sorted_data[i]
        if s == 1:
            best_selection.append((i + 1, 1, w1, v1))
            j -= w1
        elif s == 2:
            best_selection.append((i + 1, 2, w2, v2))
            j -= w2
        elif s == 3:
            best_selection.append((i + 1, 3, w3, v3))
            j -= w3
    best_weight = C - j
    solve_time = round(time.time() - start, 4)
    messagebox.showinfo("求解完成", f"最优价值：{best_value}\n总重量：{best_weight}\n耗时：{solve_time}s")

# ---------------------- 5. 保存结果 TXT ----------------------
def save_txt():
    if best_value == 0:
        messagebox.showwarning("提示", "请先求解")
        return
    path = filedialog.asksaveasfilename(defaultextension=".txt", filetypes=[("TXT", "*.txt")])
    if not path:
        return
    with open(path, "w", encoding="utf-8") as f:
        f.write(f"=== D{0-1}KP 最优解 ===\n")
        f.write(f"背包容量：{capacity}\n")
        f.write(f"项集数量：{item_count}\n")
        f.write(f"最优价值：{best_value}\n")
        f.write(f"最优重量：{best_weight}\n")
        f.write(f"求解时间：{solve_time}s\n\n")
        f.write("选择详情（项集号-物品号-重量-价值）：\n")
        for item in best_selection:
            f.write(f"{item}\n")
    messagebox.showinfo("成功", "已保存 TXT")

# ---------------------- 6. 导出 Excel ----------------------
def save_excel():
    if best_value == 0:
        messagebox.showwarning("提示", "请先求解")
        return
    path = filedialog.asksaveasfilename(defaultextension=".xlsx", filetypes=[("Excel", "*.xlsx")])
    if not path:
        return
    df = pd.DataFrame(best_selection, columns=["项集号", "选择物品", "重量", "价值"])
    info = pd.DataFrame([
        ["背包容量", capacity],
        ["最优价值", best_value],
        ["最优重量", best_weight],
        ["求解时间(s)", solve_time]
    ], columns=["参数", "数值"])
    with pd.ExcelWriter(path) as w:
        df.to_excel(w, sheet_name="选择方案", index=False)
        info.to_excel(w, sheet_name="结果汇总", index=False)
    messagebox.showinfo("成功", "已导出 Excel")

# ---------------------- GUI 界面 ----------------------
root = tk.Tk()
root.title("D{0-1}背包问题求解系统 — 动态规划")
root.geometry("600x420")

frame = ttk.Frame(root, padding=20)
frame.pack(expand=True, fill=tk.BOTH)

ttk.Button(frame, text="1. 读取D{0-1}KP数据", command=load_data).pack(pady=5, fill=tk.X)
ttk.Button(frame, text="2. 绘制重量-价值散点图", command=plot_graph).pack(pady=5, fill=tk.X)
ttk.Button(frame, text="3. 按第三项价值/重量比排序", command=sort_by_third_ratio).pack(pady=5, fill=tk.X)
ttk.Button(frame, text="4. 动态规划求最优解", command=solve_dp).pack(pady=5, fill=tk.X)
ttk.Button(frame, text="5. 保存结果到TXT", command=save_txt).pack(pady=5, fill=tk.X)
ttk.Button(frame, text="6. 导出结果到Excel", command=save_excel).pack(pady=5, fill=tk.X)
ttk.Label(frame, text="\n本程序支持：读数据→画图→排序→DP求解→保存结果", font=("", 10)).pack()

root.mainloop()