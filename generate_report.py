#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
C++ 控制语句应用实验报告 — 海岛求生
依赖: pip install fpdf2
"""

import os, sys
from fpdf import FPDF

if sys.platform == 'win32':
    try:
        sys.stdout.reconfigure(encoding='utf-8')
    except Exception:
        pass

# ============================================================
# 封面信息
# ============================================================
STUDENT_NAME   = "张轩基"
STUDENT_ID     = "1030425213"
STUDENT_CLASS  = "计科2502"
SUBMIT_DATE    = "2026.6.26"
GITHUB_URL     = "https://github.com/JieMoJi/Island-survival"
TITLE_CN       = "海岛求生 — C++ 控制台交互游戏"

# ============================================================
# 字体
# ============================================================
def find_font():
    for p in ["C:/Windows/Fonts/msyh.ttc","C:/Windows/Fonts/simhei.ttf","C:/Windows/Fonts/simsun.ttc"]:
        if os.path.exists(p): return p
    raise RuntimeError("未找到中文字体")

FONT_PATH = find_font()
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
PICS_DIR = os.path.join(BASE_DIR, "Pictures")

# ============================================================
# PDF 类
# ============================================================
class PDF(FPDF):
    def __init__(self, fp):
        super().__init__('P','mm','A4')
        self.set_auto_page_break(True, 20)
        self.add_font('C','',fp)
        self.fn = 'C'
        self.fb = 11; self.fs = 9; self.fh2 = 15; self.fh3 = 12

    def footer(self):
        self.set_y(-15); self.set_font(self.fn,'',8)
        self.set_text_color(128,128,128)
        self.cell(0,10, f"{TITLE_CN} — 实验报告  |  第{self.page_no()}页", align='C')

    def body(self, t, s=None):
        if s is None: s = self.fb
        self.set_font(self.fn,'',s); self.multi_cell(0, s*0.62, t, align='L'); self.ln(2)

    def sec(self, title):
        self.ln(3); self.set_font(self.fn,'',self.fh2)
        self.set_text_color(0,51,102)
        self.cell(0,10,title,new_x="LMARGIN",new_y="NEXT")
        y=self.get_y(); self.set_draw_color(0,51,102); self.set_line_width(0.6)
        self.line(self.l_margin,y,self.w-self.r_margin,y); self.ln(5)
        self.set_text_color(0,0,0)

    def sub(self, title):
        self.set_font(self.fn,'',self.fh3); self.cell(0,8,title,new_x="LMARGIN",new_y="NEXT"); self.ln(3)

    def code(self, t, s=None):
        if s is None: s = self.fs
        self.set_font(self.fn,'',s); self.set_fill_color(245,245,245)
        self.set_draw_color(200,200,200); self.set_text_color(40,40,40)
        n = t.count('\n')+1; h = max(n*s*0.52, 8)
        if self.get_y()+h+6 > self.h-20: self.add_page()
        self.rect(self.get_x(),self.get_y(),self.w-self.l_margin-self.r_margin, h+5, style='DF')
        self.set_xy(self.get_x()+2,self.get_y()+3)
        self.multi_cell(self.w-self.l_margin-self.r_margin-4, s*0.52, t)
        self.set_text_color(0,0,0); self.ln(3)

    def cover(self):
        self.add_page(); self.ln(20)
        self.set_font(self.fn,'',30); self.set_text_color(0,51,102)
        self.cell(0,16,"海岛求生",align='C',new_x="LMARGIN",new_y="NEXT")
        self.set_font(self.fn,'',14); self.set_text_color(80,80,80)
        self.cell(0,10,"C++ 控制语句应用 — 控制台交互游戏实验",align='C',new_x="LMARGIN",new_y="NEXT"); self.ln(8)
        self.set_draw_color(0,51,102); self.set_line_width(0.8)
        cx=self.w/2-45; self.line(cx,self.get_y(),cx+90,self.get_y()); self.ln(14)
        self.set_font(self.fn,'',13); self.set_text_color(0,0,0)
        for l,v in [("姓    名：",STUDENT_NAME),("学    号：",STUDENT_ID),
                     ("班    级：",STUDENT_CLASS),("提交日期：",SUBMIT_DATE),
                     ("源代码  ：",GITHUB_URL)]:
            self.cell(0,10,l+v,align='C',new_x="LMARGIN",new_y="NEXT"); self.ln(2)
        self.ln(18)
        self.set_font(self.fn,'',9); self.set_text_color(150,150,150)
        self.cell(0,8,"while  do-while  for  switch  if-else  break  continue",align='C',new_x="LMARGIN",new_y="NEXT")
        self.set_text_color(0,0,0)

    def insert_screenshot(self, filename, caption):
        filepath = os.path.join(PICS_DIR, filename)
        img_w = 130; img_h = 82
        margin_x = (self.w - img_w) / 2
        if self.get_y() + img_h + 12 > self.h - 20:
            self.add_page()
        if os.path.exists(filepath):
            self.image(filepath, x=margin_x, w=img_w, h=img_h)
            self.ln(2)
            self.set_font(self.fn, '', 9)
            self.set_text_color(100, 100, 100)
            self.cell(0, 5, caption, align='C', new_x="LMARGIN", new_y="NEXT")
            self.set_text_color(0, 0, 0)
            self.ln(4)
        else:
            self.body(f"(未找到截图: {filename})")


# ============================================================
# 报告内容
# ============================================================

# --- 第一部分：教材游戏分析 ---
S1_PONG_INTRO = (
    "教材游戏为《现代C++编程实战》第4章实战项目之二——Pong游戏。"
    "这是一个经典的双人对战弹球游戏：画面中间有一个球(O)自动运动，"
    "左右两侧各有一块挡板(Z)分别由两位玩家通过键盘控制。"
    "球碰到挡板反弹并得分，球出界则对方得分，按 Q 键退出游戏。\n\n"
    "该游戏使用 C++ 标准库和少量平台 API 实现，核心逻辑约 120 行（不含平台适配代码），"
    "展示了控制语句在实时游戏循环中的应用。"
)

S1_CONTROL_TABLE_HEADER = ["控制语句", "行号", "游戏功能", "分析说明"]
S1_CONTROL_TABLE_DATA = [
    ("while", "100", "游戏主循环",
     "while(true) 构成无限循环，驱动每一帧的「事件处理→数据更新→画面渲染」三步流程。游戏持续运行直到 break 退出。"),
    ("if-else if-else", "107-123", "键盘输入分发",
     "根据按键值分支：W/S 移动左挡板、方向键(72/80)移动右挡板、Q 触发退出。每个分支包含边界检测（挡板不可移出画面）。"),
    ("if / if-else", "130-153", "碰撞检测与计分",
     "独立 if 判断球碰上下墙（速度反转）；if-else 判断球碰左/右挡板（反弹+计分）；if 判断球出界（对方得分+球重置）。"),
    ("for (嵌套)", "170-205", "帧缓冲绘制",
     "外层 for 遍历行，内层 for 遍历列。每格根据坐标判断绘制什么：O=球, Z=挡板, |=中线/边线, ==顶部墙，一次构建整屏画面。"),
    ("break", "122", "退出游戏主循环",
     "玩家按 Q 键时，break 跳出 while(true) 主循环，结束程序。是游戏唯一的正常退出路径。"),
    ("#ifdef / #else", "多处", "跨平台条件编译",
     "预处理指令区分 Windows（使用 conio.h）和 Linux（手动实现 _kbhit/_getch），确保不同平台的终端兼容性。"),
]

S1_FLOW_TEXT = (
    "Pong 游戏流程简述：\n\n"
    "程序启动 → 初始化变量(球位置/速度、挡板位置、分数) → 随机初始球方向 → 隐藏光标\n"
    "→ 进入 while(true) 主循环：\n"
    "   (1) 检测键盘：if-else 分支处理 W/S/↑/↓/Q\n"
    "   (2) 更新数据：球移动 + if 检测碰撞反弹/出界计分\n"
    "   (3) 渲染画面：嵌套 for 逐像素填充帧缓冲 → 整屏输出\n"
    "   (4) sleep 50ms 控制帧率\n"
    "→ Q 触发 break 退出循环 → 显示光标 → 程序结束\n\n"
    "（详细流程图见 PDF 中的 ASCII 流程图）"
)

S1_FLOWCHART = (
    "Pong 游戏简化流程图：\n\n"
    "  ┌──────────────┐\n"
    "  │   程序启动     │\n"
    "  │ 初始化变量    │\n"
    "  │ 随机球速度    │\n"
    "  │ 隐藏光标      │\n"
    "  └──────┬───────┘\n"
    "         ▼\n"
    "  ┌─────────────────────────────────────────┐\n"
    "  │        while (true)  游戏主循环           │\n"
    "  │                                          │\n"
    "  │  ┌──────────────────────────────────┐    │\n"
    "  │  │ 1. 检测键盘 (_kbhit)              │    │\n"
    "  │  │    if-else if-else 分支:          │    │\n"
    "  │  │      W/S → 左挡板移动             │    │\n"
    "  │  │      ↑/↓ → 右挡板移动             │    │\n"
    "  │  │      Q   → break ──────────────┐  │    │\n"
    "  │  └────────────────┬───────────────┘  │    │\n"
    "  │                   ▼                   │    │\n"
    "  │  ┌──────────────────────────────────┐  │    │\n"
    "  │  │ 2. 更新球位置                     │  │    │\n"
    "  │  │    if 碰上下墙 → 垂直速度取反     │  │    │\n"
    "  │  │    if-else 碰挡板 → 反弹+计分     │  │    │\n"
    "  │  │    if 出界 → 对方得分+球重置      │  │    │\n"
    "  │  └────────────────┬───────────────┘  │    │\n"
    "  │                   ▼                   │    │\n"
    "  │  ┌──────────────────────────────────┐  │    │\n"
    "  │  │ 3. 光标复位 + 嵌套 for 绘制画面   │  │    │\n"
    "  │  │    输出 framebuffer 到控制台      │  │    │\n"
    "  │  └────────────────┬───────────────┘  │    │\n"
    "  │                   ▼                   │    │\n"
    "  │  ┌──────────────────────────────────┐  │    │\n"
    "  │  │ 4. sleep 50ms 控制帧率            │  │    │\n"
    "  │  └────────────────┬───────────────┘  │    │\n"
    "  │                   │                   │    │\n"
    "  │         ◄─────────┘  (循环)            │    │\n"
    "  └─────────────────────────────────────────┘  │\n"
    "         │                                     │\n"
    "         ▼ (break 触发)  ◄─────────────────────┘\n"
    "  ┌──────────────┐\n"
    "  │ 显示光标      │\n"
    "  │ 程序结束      │\n"
    "  └──────────────┘"
)

S1_PONG_SUMMARY = (
    "教材游戏使用的控制语句汇总：while (主循环)、if-else if-else (按键分发)、"
    "if / if-else (碰撞与计分)、for 嵌套 (帧缓冲渲染)、break (退出循环)、"
    "#ifdef/#else (跨平台条件编译)。\n"
    "未使用：switch、do-while、continue。"
)

# --- 第二部分：自主设计游戏 ---
S2_INTRO = (
    "海岛求生是一款自创的回合制生存策略游戏。玩家流落荒岛，需要管理生命值、饱食度、"
    "饮水量、士气值四项生存数值。白天从 5 种行动中选择一项执行，夜晚经历随机事件"
    "（暴风雨、野兽袭击、星空之夜等），在资源持续衰减的压力下，集齐 3 个无线电零件并"
    "搭建 2 级以上庇护所即可点燃信号火获救。"
)

S2_RULES = (
    "核心规则：\n"
    "(1) 四项数值：生命值(100)、饱食度(100)、饮水量(100)、士气值(50)。任一归零则游戏失败。\n"
    "(2) 白天行动（五选一）：寻找食物、收集淡水、修建庇护所、探索岛屿、休息恢复。\n"
    "(3) 每日衰减：饱食度 -10，饮水量 -15，士气值 -3（迫使玩家策略性选择行动）。\n"
    "(4) 夜间事件（4种随机）：暴风雨、野兽袭击、星空之夜、平安夜。庇护所等级影响减伤。\n"
    "(5) 胜利条件：集齐 3 个无线电零件（通过探索岛屿随机获得）+ 庇护所 ≥ 2 级。\n"
    "(6) 得分公式：存活天数 × 100 + 四项数值 + 获救奖励 500。"
)

S2_CONTROL_TABLE_HEADER = ["控制语句", "出现位置", "实现功能"]
S2_CONTROL_TABLE_DATA = [
    ("while (3处)", "外层重玩循环 / 内层日循环 / 坏输入消费",
     "外层 while(playAgain==1) 控制整局重玩；内层 while(gameOver==0) 驱动每日循环；嵌套 while 配合 cin.get() 消费非法输入字符。"),
    ("do-while (2处)", "行动菜单验证 / 重玩询问验证",
     "先显示菜单再检查输入，保证菜单至少展示一次。cin.fail() + 范围校验确保输入为 1-5 或 0-1。"),
    ("switch (1处)", "白天行动选择 (5 case + default)",
     "根据 1-5 分别执行：寻找食物、收集淡水、修建庇护所、探索岛屿、休息恢复。每个 case 内部用 if-else 实现随机产出。"),
    ("if-else (10+处)", "夜间事件 / 死亡判定 / 胜利判定 / 数值钳制",
     "4 分支 if-else 链随机选择夜间事件；多层 if-else 检测四项数值归零并给出不同死因提示；if 检测胜利条件；每次修改数值后 clamp 到 [0,100]。"),
    ("for (6处)", "状态栏文本进度条 / 倒计时动画 / 延时循环",
     "4 个 for 循环绘制 [####----] 风格进度条；1 个 for 循环实现 3-2-1 救援倒计时；1 个 for 空循环制造延时效果。"),
    ("break (8处)", "switch case 退出 / 胜利跳出日循环",
     "每个 switch case 末尾 break 防止穿透；胜利后 break 跳出内层 while 循环进入结算。"),
    ("continue (6处)", "死亡后跳过夜间阶段 / 夜间死后跳过胜利检测",
     "白天行动后若死亡，continue 跳过夜间阶段和胜利检测，直接结算；夜间事件后再次死亡检测同理。"),
]

S2_FLOW_TEXT = (
    "海岛求生游戏流程：\n\n"
    "标题画面 → 外层 while(playAgain==1) 重玩循环 → 初始化数据\n"
    "→ 内层 while(gameOver==0) 日循环：\n"
    "   (1) 显示状态栏（4 个 for 进度条）\n"
    "   (2) do-while 输入验证 → switch 执行白天行动\n"
    "   (3) 每日被动衰减（hunger-10, thirst-15, morale-3）\n"
    "   (4) if-else 死亡检测 → continue（跳过夜间）\n"
    "   (5) if-else 夜间随机事件 + 数值钳制\n"
    "   (6) if-else 二次死亡检测 → continue\n"
    "   (7) if 胜利检测 → for 倒计时 3-2-1 → break\n"
    "   (8) day++\n"
    "→ 结算界面（得分计算+显示）\n"
    "→ do-while 重玩询问 → 回到外层 while / 退出\n"
    "→ 结束语 → return 0"
)

# --- 第三部分：对比分析 ---
S3_COMPARE = (
    "教材 Pong 游戏 vs 自主设计海岛求生的控制语句对比：\n\n"
    "1. 都使用了 while 作为游戏主循环，但结构不同：\n"
    "   教材用单层 while(true)+break；海岛求生用双层 while（外层重玩+内层日循环）"
    "+break+continue，结构更丰富。\n\n"
    "2. 教材使用 if-else 处理键盘分支和碰撞判断；海岛求生使用 if-else 处理更大的分支"
    "——死亡判定（4种死因）、夜间事件（4种事件，且与庇护所等级交互）、胜利条件。"
    "分支的嵌套层级和组合复杂度更高。\n\n"
    "3. switch 和 do-while 仅在自主设计游戏中使用：\n"
    "   教材的按键处理用 if-else if（符合实时检测场景），海岛求生的菜单选择用 switch（"
    "更清晰的枚举型分支）和 do-while（保证菜单至少显示一次后验证输入）。\n\n"
    "4. continue 仅在自主设计游戏中使用：\n"
    "   死亡后 continue 跳过夜间或胜利检测，避免了深层嵌套的 if 块，保持代码扁平化。\n\n"
    "5. for 在两款游戏中的用途截然不同：\n"
    "   教材用嵌套 for 构建像素级帧缓冲；海岛求生用 for 绘制文本进度条和倒计时动画。\n"
    "   前者面向像素渲染，后者面向文本显示。\n\n"
    "6. 教材未使用的三种控制语句（switch、do-while、continue），自主设计游戏均自然引入，"
    "覆盖了章节所有核心语法点。"
)

# ============================================================
# 主流程
# ============================================================
def main():
    print(f"使用字体: {FONT_PATH}")
    pdf = PDF(FONT_PATH)

    # ==== 封面 ====
    pdf.cover()

    # ================================================================
    # 第 1 章：教材游戏分析
    # ================================================================
    pdf.add_page()
    pdf.sec("1. 教材游戏分析 — Pong 游戏")
    pdf.sub("1.1 游戏简介")
    pdf.body(S1_PONG_INTRO)

    pdf.sub("1.2 控制语句分析")
    # 分析表格
    col_w = [26, 16, 38, 110]  # 总宽 ~190mm
    headers = S1_CONTROL_TABLE_HEADER
    data   = S1_CONTROL_TABLE_DATA
    line_h = 5.5

    # 表头
    pdf.set_fill_color(0, 51, 102)
    pdf.set_text_color(255, 255, 255)
    pdf.set_font(pdf.fn, '', 8)
    y_top = pdf.get_y()
    for i, h in enumerate(headers):
        pdf.set_xy(pdf.l_margin + sum(col_w[:i]), y_top)
        pdf.cell(col_w[i], 9, " " + h, border=1, fill=True)
    pdf.set_xy(pdf.l_margin, y_top + 9)

    # 数据行
    for j, row in enumerate(data):
        pdf.set_fill_color(245, 248, 252) if j % 2 == 0 else pdf.set_fill_color(255, 255, 255)
        pdf.set_text_color(0, 0, 0)
        pdf.set_font(pdf.fn, '', 7.5)

        desc_text = row[3]
        try:
            lines = pdf.multi_cell(col_w[3], line_h, desc_text, dry_run=True, output="LINES")
            line_count = len(lines)
        except Exception:
            line_count = desc_text.count('\n') + 1
        row_h = line_count * line_h

        y_row = pdf.get_y()
        if y_row + row_h > pdf.h - 25:
            pdf.add_page()
            y_row = pdf.get_y()

        for i, cell_text in enumerate(row):
            x_cell = pdf.l_margin + sum(col_w[:i])
            pdf.set_xy(x_cell, y_row)
            if i == 3:
                pdf.multi_cell(col_w[i], line_h, cell_text, border=1, fill=True)
            else:
                pdf.cell(col_w[i], row_h, " " + str(cell_text).replace('\n', ' '), border=1, fill=True)
        pdf.set_xy(pdf.l_margin, y_row + row_h)
    pdf.ln(6)

    pdf.sub("1.3 游戏流程图")
    pdf.code(S1_FLOWCHART)
    pdf.sub("1.4 控制语句汇总")
    pdf.body(S1_PONG_SUMMARY)

    # ================================================================
    # 第 2 章：自主设计游戏 — 海岛求生
    # ================================================================
    pdf.add_page()
    pdf.sec("2. 自主设计游戏 — 海岛求生 (Island Survival)")
    pdf.sub("2.1 游戏简介")
    pdf.body(S2_INTRO)

    pdf.sub("2.2 核心规则")
    pdf.body(S2_RULES)

    pdf.sub("2.3 控制语句使用分析")

    # 表格2
    col_w2 = [30, 50, 110]
    headers2 = S2_CONTROL_TABLE_HEADER
    data2   = S2_CONTROL_TABLE_DATA

    pdf.set_fill_color(0, 51, 102)
    pdf.set_text_color(255, 255, 255)
    pdf.set_font(pdf.fn, '', 8)
    y_top2 = pdf.get_y()
    for i, h in enumerate(headers2):
        pdf.set_xy(pdf.l_margin + sum(col_w2[:i]), y_top2)
        pdf.cell(col_w2[i], 9, " " + h, border=1, fill=True)
    pdf.set_xy(pdf.l_margin, y_top2 + 9)

    for j, row in enumerate(data2):
        pdf.set_fill_color(245, 248, 252) if j % 2 == 0 else pdf.set_fill_color(255, 255, 255)
        pdf.set_text_color(0, 0, 0)
        pdf.set_font(pdf.fn, '', 7.5)

        desc_text = row[2]
        try:
            lines = pdf.multi_cell(col_w2[2], line_h, desc_text, dry_run=True, output="LINES")
            line_count = len(lines)
        except Exception:
            line_count = desc_text.count('\n') + 1
        row_h = line_count * line_h

        y_row = pdf.get_y()
        if y_row + row_h > pdf.h - 25:
            pdf.add_page()
            y_row = pdf.get_y()

        for i, cell_text in enumerate(row):
            x_cell = pdf.l_margin + sum(col_w2[:i])
            pdf.set_xy(x_cell, y_row)
            if i == 2:
                pdf.multi_cell(col_w2[i], line_h, cell_text, border=1, fill=True)
            else:
                pdf.cell(col_w2[i], row_h, " " + str(cell_text).replace('\n', ' '), border=1, fill=True)
        pdf.set_xy(pdf.l_margin, y_row + row_h)
    pdf.ln(6)

    pdf.sub("2.4 游戏流程")
    pdf.body(S2_FLOW_TEXT)

    # ================================================================
    # 第 3 章：对比分析
    # ================================================================
    pdf.add_page()
    pdf.sec("3. 两款游戏的控制语句对比分析")
    pdf.body(S3_COMPARE)

    # 对比汇总表
    pdf.ln(3)
    pdf.set_fill_color(0, 51, 102)
    pdf.set_text_color(255, 255, 255)
    pdf.set_font(pdf.fn, '', 9)
    col_w3 = [40, 50, 50, 50]
    headers3 = ["控制语句", "教材 Pong", "海岛求生", "差异说明"]
    y_top3 = pdf.get_y()
    for i, h in enumerate(headers3):
        pdf.set_xy(pdf.l_margin + sum(col_w3[:i]), y_top3)
        pdf.cell(col_w3[i], 9, " " + h, border=1, fill=True)
    pdf.set_xy(pdf.l_margin, y_top3 + 9)

    compare_data = [
        ("while",    "✓ (主循环)",       "✓ (双层: 重玩+日循环)",  "自主设计结构更复杂"),
        ("if-else",  "✓ (按键+碰撞)",    "✓ (死亡+事件+胜利)",    "自主设计分支维度更多"),
        ("for",      "✓ (嵌套像素渲染)", "✓ (进度条+倒计时)",     "完全不同的应用场景"),
        ("break",    "✓ (退出循环)",     "✓ (case+胜利退出)",     "自主设计使用场景更多"),
        ("switch",   "✗ 未使用",         "✓ (行动菜单)",          "教材用 if-else 替代"),
        ("do-while", "✗ 未使用",         "✓ (输入验证×2)",        "教材无需输入验证"),
        ("continue", "✗ 未使用",         "✓ (跳过夜间阶段)",      "教材无需此模式"),
    ]
    for j, row in enumerate(compare_data):
        pdf.set_fill_color(245, 248, 252) if j % 2 == 0 else pdf.set_fill_color(255, 255, 255)
        pdf.set_text_color(0, 0, 0)
        pdf.set_font(pdf.fn, '', 8)
        for i, cell_text in enumerate(row):
            x_cell = pdf.l_margin + sum(col_w3[:i])
            pdf.set_xy(x_cell, pdf.get_y())
            pdf.cell(col_w3[i], 8, " " + cell_text, border=1, fill=True)
        pdf.ln(8)
    pdf.ln(6)

    # ================================================================
    # 第 4 章：游戏截图
    # ================================================================
    pdf.add_page()
    pdf.sec("4. 游戏运行截图")

    screenshots = [
        ("标题画面.png",    "图1: 游戏标题画面与规则说明"),
        ("游戏中.png",      "图2: 游戏中 — 状态栏与行动菜单"),
        ("夜间事件.png",    "图3: 夜间随机事件"),
        ("胜利结局.png",    "图4: 胜利结局 — 获救"),
        ("游戏失败.png",    "图5: 失败结局 — 数值归零"),
    ]
    for filename, caption in screenshots:
        pdf.insert_screenshot(filename, caption)

    # ================================================================
    # 第 5 章：实验总结
    # ================================================================
    pdf.add_page()
    pdf.sec("5. 实验总结与心得")

    S5_SUMMARY = (
        "通过本次实验，我对 C++ 控制语句的理解从\"知道语法\"提升到了\"知道何时用、"
        "如何组合用\"的层次。具体收获如下：\n\n"
        "(1) 控制语句的选择与场景匹配：\n"
        "switch 适合固定选项的菜单（如白天行动 1-5），每个 case 语义独立清晰；\n"
        "if-else 适合条件范围判断（如数值是否 <= 0、随机事件分发），表达力更强；\n"
        "do-while 适合\"先展示后验证\"的交互场景，保证用户至少看到一次菜单；\n"
        "for 适合\"明确次数\"的重复操作（如进度条 10 格、倒计时 3 秒）；\n"
        "while 适合\"条件驱动\"的循环（如 gameOver==0 时继续每日循环）。\n\n"
        "(2) break 与 continue 的语义差异：\n"
        "break 表示\"终止当前循环\"——用于 switch case 退出和胜利后跳出日循环；\n"
        "continue 表示\"跳过本次迭代剩余部分\"——用于死亡后跳过夜间阶段。\n"
        "两者的语义差异在游戏逻辑中体现得非常直观。\n\n"
        "(3) 结构清晰性：\n"
        "在没有函数/数组/类的情况下组织约 500 行代码，变量命名和逻辑分块至关重要。\n"
        "通过 ===== 分隔线注释 + 缩进层级控制 + 每段逻辑前的中文说明，保持了代码在"
        "单函数（main）内的可读性。\n\n"
        "(4) 输入容错的价值：\n"
        "cin.fail() + cin.clear() + 消费坏输入的 while 循环是实际编程中非常实用的模式，"
        "课本上很少讲到，但实际编程中必不可少。\n\n"
        "不足与改进方向：\n"
        "· 倒计时的延时使用空 for 循环，依赖于 CPU 速度，不够精确。后续学习后可改用 sleep()。\n"
        "· 随机事件使用简单的 rand()%N，存在微小偏差。可升级为 C++11 <random> 库。\n"
        "· 游戏平衡性（衰减速率、随机产出范围、夜间事件概率）可进一步调优。\n"
        "· 后续学习了函数后可重构代码，将日循环、夜间事件等模块化。\n\n"
        "总体而言，本次实验让我在实践中掌握了控制语句的选择与组合运用，"
        "为后续学习函数、数组、类和更复杂的程序结构打下了扎实的基础。"
    )
    pdf.body(S5_SUMMARY)

    # ==== 输出 ====
    fname = f"{STUDENT_ID}_{STUDENT_NAME}_控制语句实验报告.pdf"
    out = os.path.join(BASE_DIR, fname)
    pdf.output(out)
    kb = os.path.getsize(out)/1024
    print(f"报告已生成: {out}")
    print(f"文件大小: {kb:.1f} KB  |  页数: {pdf.pages_count} 页")

if __name__ == "__main__":
    main()
