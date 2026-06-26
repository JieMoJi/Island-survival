/*
 * 文件名: island_survival.cpp
 * 作业: C++ 控制语句应用——设计并实现控制台交互游戏
 * 游戏名: 海岛求生 (Island Survival)
 * 描述: 玩家流落荒岛，管理生命/饱食/饮水/士气四项数值，
 *       白天选择行动，夜晚经历随机事件，
 *       集齐3个无线电零件并搭建2级以上庇护所即可获救。
 * 控制语句: while, do-while, for, switch, if-else, break, continue
 * 编译: g++ -fexec-charset=UTF-8 island_survival.cpp -o island_survival.exe
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <windows.h>
using namespace std;

int main() {

    // 设置控制台编码为 UTF-8，解决中文乱码问题
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // ========== 变量声明 ==========
    int health, hunger, thirst, morale;          // 四项核心生存数值
    int shelter_level, radio_parts, day;          // 庇护所等级、收集的零件数、当前天数
    int gameOver, won, playAgain, choice;         // 游戏状态控制变量
    int event_roll, loot_roll, explore_roll;      // 随机数
    int find_amount, storm_damage;                // 临时数值
    int countdown, clean_char;                    // 倒计时和输入清理用变量
    int totalScore;                                // 最终得分

    // 初始化随机数种子
    srand((unsigned int)time(0));

    // ========== 标题画面 ==========
    cout << "\n";
    cout << "                ╔══════════════════════════════════╗\n";
    cout << "                ║                                ║\n";
    cout << "                ║    🏝    海  岛  求  生    🏝  ║\n";
    cout << "                ║      ISLAND  SURVIVAL          ║\n";
    cout << "                ║                                ║\n";
    cout << "                ╚══════════════════════════════════╝\n\n";
    cout << "                          ~  ~~~~ ~~~\n";
    cout << "           .       .  ~  ~~~  ~~~~~  ~~~  .\n";
    cout << "       ~  .      .      ~~~~~     ~~~       .     ~\n";
    cout << "     ~~~~~    ~~~     ~~~~~~~~~    ~~~~    ~~~   ~~~~\n";
    cout << "   ~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    cout << "  ===================================================\n";
    cout << "           ▒▒                    ▒▒▒▒\n";
    cout << "          ▒▒▒▒     🏝           ▒▒▒▒▒▒\n";
    cout << "         ▒▒▒▒▒▒               ▒▒▒▒▒▒▒▒\n";
    cout << "        ▒▒▒▒▒▒▒▒     🌴      ▒▒▒▒▒▒▒▒▒▒\n";
    cout << "       ▒▒▒▒▒▒▒▒▒▒           ▒▒▒▒▒▒▒▒▒▒▒▒\n";
    cout << "      ▒▒▒▒▒▒▒▒▒▒▒▒         ▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n";
    cout << "     ▒▒▒▒▒▒▒▒▒▒▒▒▒▒       ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n";
    cout << "    ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒     ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n";
    cout << "  ===================================================\n";
    cout << "   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";

    cout << "【背景故事】\n";
    cout << "你乘坐的航班在太平洋上空遭遇风暴，迫降在一座\n";
    cout << "无人荒岛上。你的目标是——活下去，并找到回家的方法。\n\n";

    cout << "【游戏规则】\n";
    cout << "- 你有四项生存数值：生命、饱食、饮水、士气\n";
    cout << "- 每天可以选择一个行动（白天），然后随机事件发生（夜晚）\n";
    cout << "- 任一数值降为 0，游戏失败\n";
    cout << "- 集齐 3 个无线电零件 + 庇护所 ≥ 2 级 → 发出求救信号 → 胜利！\n\n";

    cout << "【提示】\n";
    cout << "- 饱食每天 -10，饮水每天 -15，士气每天 -3（被动衰减）\n";
    cout << "- 你一天只能做一件事，优先保证存活！\n";
    cout << "- 庇护所可在暴风雨和野兽袭击时保护你\n\n";

    cout << "╔══════════════════════════════════════════╗\n";
    cout << "║       按 Enter 键开始你的求生之旅...     ║\n";
    cout << "╚══════════════════════════════════════════╝\n";
    cin.get();

    // ========== 外层循环：重玩控制 ==========
    playAgain = 1;
    while (playAgain == 1) {

        // ----- 游戏状态初始化 -----
        health = 100;
        hunger = 100;
        thirst = 100;
        morale = 50;
        shelter_level = 0;
        radio_parts = 0;
        day = 1;
        gameOver = 0;
        won = 0;

        // ========== 内层循环：每日循环 ==========
        while (gameOver == 0) {

            // ===== 状态栏显示 =====
            cout << "\n";
            cout << "  ┌──────────────────────────────────────────┐\n";
            cout << "  │         ☀  第 " << day << " 天  ☀";
            if (day < 10) { cout << " "; }
            cout << "                       │\n";
            cout << "  └──────────────────────────────────────────┘\n";

            // 生命值文本进度条
            cout << "生命值: [";
            for (countdown = 0; countdown < 10; countdown = countdown + 1) {
                if (countdown < health / 10) {
                    cout << "#";
                } else {
                    cout << "-";
                }
            }
            cout << "] " << health << "/100\n";

            // 饱食进度条
            cout << "饱食度: [";
            for (countdown = 0; countdown < 10; countdown = countdown + 1) {
                if (countdown < hunger / 10) {
                    cout << "#";
                } else {
                    cout << "-";
                }
            }
            cout << "] " << hunger << "/100\n";

            // 饮水进度条
            cout << "饮水量: [";
            for (countdown = 0; countdown < 10; countdown = countdown + 1) {
                if (countdown < thirst / 10) {
                    cout << "#";
                } else {
                    cout << "-";
                }
            }
            cout << "] " << thirst << "/100\n";

            // 士气显示
            cout << "士气值: [";
            for (countdown = 0; countdown < 10; countdown = countdown + 1) {
                if (countdown < morale / 10) {
                    cout << "#";
                } else {
                    cout << "-";
                }
            }
            cout << "] " << morale << "/100  (上限100)\n\n";

            // 庇护所和零件信息
            cout << "--- 设施 ---\n";
            cout << "庇护所等级: ";
            if (shelter_level == 0) {
                cout << "无 (0/3)";
            } else if (shelter_level == 1) {
                cout << "简易棚屋 (1/3)";
            } else if (shelter_level == 2) {
                cout << "牢固木屋 (2/3)  ← 满足求救条件";
            } else {
                cout << "固若金汤 (3/3)";
            }
            cout << "\n";

            cout << "无线电零件: " << radio_parts << "/3";
            if (radio_parts == 3) {
                cout << "  ← 已集齐！";
            }
            cout << "\n";

            // ===== 输入验证：行动选择菜单 =====
            do {
                cout << "\n--- 白天行动 ---\n";
                cout << "1. 寻找食物     (饱食度 +20~40)\n";
                cout << "2. 收集淡水     (饮水量 +20~40)\n";
                cout << "3. 修建庇护所   (庇护所等级 +1, 最高3级)\n";
                cout << "4. 探索岛屿     (可能找到无线电零件)\n";
                cout << "5. 休息恢复     (生命 +10~20, 士气 +5~10)\n";
                cout << "请选择行动 (1-5): ";

                cin >> choice;

                // 如果输入的不是数字，清理错误状态并丢弃坏输入
                if (cin.fail()) {
                    cin.clear();
                    clean_char = cin.get();
                    while (clean_char != '\n' && clean_char != EOF) {
                        clean_char = cin.get();
                    }
                    cout << "[!] 请输入数字 1-5！\n";
                    choice = -1;    // 强制重新循环
                } else if (choice < 1 || choice > 5) {
                    cout << "[!] 请输入 1-5 之间的数字！\n";
                }
            } while (choice < 1 || choice > 5);

            // ===== switch：执行玩家选择的行动 =====
            switch (choice) {
                case 1: {
                    // 寻找食物：随机获得不同数量的食物
                    loot_roll = rand() % 4;
                    if (loot_roll == 0) {
                        find_amount = 40;
                        cout << "\n[+] 你找到了一棵结满果实的椰子树！饱食度 +40\n";
                    } else if (loot_roll == 1) {
                        find_amount = 30;
                        cout << "\n[+] 你采集了一大捧野果。饱食度 +30\n";
                    } else if (loot_roll == 2) {
                        find_amount = 25;
                        cout << "\n[+] 你在礁石上挖到了一些贝类。饱食度 +25\n";
                    } else {
                        find_amount = 20;
                        cout << "\n[+] 你找到了一些可食用的植物根茎。饱食度 +20\n";
                    }
                    hunger = hunger + find_amount;
                    if (hunger > 100) {
                        hunger = 100;
                    }
                    break;
                }

                case 2: {
                    // 收集淡水：随机获得不同量的淡水
                    loot_roll = rand() % 4;
                    if (loot_roll == 0) {
                        find_amount = 40;
                        cout << "\n[+] 你发现了一处清澈的山泉！饮水量 +40\n";
                    } else if (loot_roll == 1) {
                        find_amount = 30;
                        cout << "\n[+] 昨晚的雨水积在岩石凹坑中，你收集了不少。饮水量 +30\n";
                    } else if (loot_roll == 2) {
                        find_amount = 25;
                        cout << "\n[+] 你找到了一条小溪。饮水量 +25\n";
                    } else {
                        find_amount = 20;
                        cout << "\n[+] 你用椰子壳收集了些许淡水。饮水量 +20\n";
                    }
                    thirst = thirst + find_amount;
                    if (thirst > 100) {
                        thirst = 100;
                    }
                    break;
                }

                case 3: {
                    // 修建庇护所：每次提升1级，最高3级
                    if (shelter_level >= 3) {
                        cout << "\n[!] 你的庇护所已经是最牢固的了！无需继续修建。\n";
                    } else {
                        shelter_level = shelter_level + 1;
                        if (shelter_level == 1) {
                            cout << "\n[+] 你用棕榈叶和树枝搭了一个简易棚屋。庇护所 → 1级\n";
                        } else if (shelter_level == 2) {
                            cout << "\n[+] 你加固了墙壁，添加了地板。庇护所 → 2级（满足求救条件！）\n";
                        } else {
                            cout << "\n[+] 你用石头加固了地基，庇护所固若金汤！庇护所 → 3级\n";
                        }
                        // 庇护所升级带来士气加成
                        morale = morale + 5;
                        if (morale > 100) {
                            morale = 100;
                        }
                        cout << "   (建造让人安心，士气 +5)\n";
                    }
                    break;
                }

                case 4: {
                    // 探索岛屿：有概率找到无线电零件或额外资源
                    explore_roll = rand() % 3;
                    if (explore_roll == 0) {
                        if (radio_parts < 3) {
                            radio_parts = radio_parts + 1;
                            cout << "\n[!] 在一处岩石缝隙中，你发现了一个无线电零件！(";
                            cout << radio_parts << "/3)\n";
                            if (radio_parts == 3) {
                                cout << "   >>> 零件已集齐！现在需要庇护所 ≥ 2 级来组装信号装置 <<<\n";
                            }
                            morale = morale + 10;
                            if (morale > 100) {
                                morale = 100;
                            }
                        } else {
                            cout << "\n[-] 你仔细搜索了岛屿，但无线电零件已经集齐了。\n";
                            cout << "   你捡到了一些有用的物资聊以自慰。\n";
                            // 即使零件已齐，探索仍可获得小奖励
                            hunger = hunger + 10;
                            if (hunger > 100) {
                                hunger = 100;
                            }
                            thirst = thirst + 10;
                            if (thirst > 100) {
                                thirst = 100;
                            }
                        }
                    } else if (explore_roll == 1) {
                        find_amount = rand() % 15 + 10;
                        hunger = hunger + find_amount;
                        if (hunger > 100) {
                            hunger = 100;
                        }
                        cout << "\n[+] 探索中你偶然发现了一片野生果园。饱食度 +";
                        cout << find_amount << "\n";
                    } else {
                        cout << "\n[-] 你探索了一整天，除了蚊子包什么都没找到...\n";
                    }
                    break;
                }

                case 5: {
                    // 休息恢复：回复生命值和士气
                    find_amount = rand() % 11 + 10;
                    health = health + find_amount;
                    if (health > 100) {
                        health = 100;
                    }
                    cout << "\n[+] 你在树荫下休息了一天，感觉精力恢复了。生命值 +";
                    cout << find_amount << "\n";

                    find_amount = rand() % 6 + 5;
                    morale = morale + find_amount;
                    if (morale > 100) {
                        morale = 100;
                    }
                    cout << "   望着大海放空自己，士气 +" << find_amount << "\n";
                    break;
                }

                default:
                    // 理论上不会到这里（已有输入验证），但保留分支以展示 continue
                    cout << "\n[!] 无效选择，跳过本日行动。\n";
                    break;
            }

            // ===== 每日被动衰减 =====
            hunger = hunger - 10;
            thirst = thirst - 15;
            morale = morale - 3;

            cout << "\n--- 每日消耗 ---\n";
            cout << "饱食度 -10 | 饮水量 -15 | 士气 -3\n";

            // 衰减后钳制下限（防止显示负数）
            if (hunger < 0) { hunger = 0; }
            if (thirst < 0) { thirst = 0; }
            if (morale < 0) { morale = 0; }

            // ===== 检测死亡：if-else 链判断各项数值 =====
            if (health <= 0) {
                gameOver = 1;
                won = 0;
                cout << "\n";
                cout << "╔══════════════════════════════════════════╗\n";
                cout << "║                                          ║\n";
                cout << "║  你因伤势过重，倒在了荒岛的海滩上...     ║\n";
                cout << "║  海浪冲走了你最后的足迹。               ║\n";
                cout << "║  也许，这座岛将成为你永恒的归宿。       ║\n";
                cout << "║                                          ║\n";
                cout << "║           †  安  息  †                 ║\n";
                cout << "║                                          ║\n";
                cout << "╚══════════════════════════════════════════╝\n";
                continue;  // 跳过夜间阶段和胜利检测，直接结算
            } else if (hunger <= 0) {
                gameOver = 1;
                won = 0;
                cout << "\n";
                cout << "╔══════════════════════════════════════════╗\n";
                cout << "║                                          ║\n";
                cout << "║  饥饿感越来越强烈，你终于撑不住了...     ║\n";
                cout << "║  你找遍了整座岛，再也没有可以吃的东西。 ║\n";
                cout << "║  你在荒岛上活活饿死了。                 ║\n";
                cout << "║                                          ║\n";
                cout << "║           †  安  息  †                 ║\n";
                cout << "║                                          ║\n";
                cout << "╚══════════════════════════════════════════╝\n";
                continue;
            } else if (thirst <= 0) {
                gameOver = 1;
                won = 0;
                cout << "\n";
                cout << "╔══════════════════════════════════════════╗\n";
                cout << "║                                          ║\n";
                cout << "║  嘴唇干裂，意识模糊，你因脱水倒下了...   ║\n";
                cout << "║  在这片被海水环绕的岛上渴死，           ║\n";
                cout << "║  这是多么残酷的讽刺。                   ║\n";
                cout << "║                                          ║\n";
                cout << "║           †  安  息  †                 ║\n";
                cout << "║                                          ║\n";
                cout << "╚══════════════════════════════════════════╝\n";
                continue;
            } else if (morale <= 0) {
                gameOver = 1;
                won = 0;
                cout << "\n";
                cout << "╔══════════════════════════════════════════╗\n";
                cout << "║                                          ║\n";
                cout << "║  孤独和绝望将你吞噬...                   ║\n";
                cout << "║  一天又一天，只有海浪声为伴。           ║\n";
                cout << "║  你失去了求生的意志，放弃了挣扎。       ║\n";
                cout << "║                                          ║\n";
                cout << "║           †  安  息  †                 ║\n";
                cout << "║                                          ║\n";
                cout << "╚══════════════════════════════════════════╝\n";
                continue;
            }

            // ===== 夜间阶段：随机事件 =====
            cout << "\n--- 夜幕降临 ---\n";
            event_roll = rand() % 4;

            // 暴风雨事件
            if (event_roll == 0) {
                cout << "天空电闪雷鸣，暴风雨席卷全岛！\n";
                if (shelter_level == 0) {
                    storm_damage = rand() % 21 + 15;
                    health = health - storm_damage;
                    cout << "你没有庇护所，在暴雨中无处可躲，浑身湿透、\n";
                    cout << "体温骤降。生命值 -" << storm_damage << "\n";
                } else if (shelter_level == 1) {
                    storm_damage = rand() % 11 + 5;
                    health = health - storm_damage;
                    cout << "简易棚屋挡了一些风雨，但还是漏了不少水。生命值 -";
                    cout << storm_damage << "\n";
                } else if (shelter_level == 2) {
                    storm_damage = rand() % 6;
                    health = health - storm_damage;
                    cout << "牢固的木屋经受住了暴风雨的考验，只是轻微漏水。生命值 -";
                    cout << storm_damage << "\n";
                } else {
                    cout << "你的庇护所固若金汤，暴风雨？就当是免费的白噪音吧。\n";
                }

            // 野兽袭击事件
            } else if (event_roll == 1) {
                cout << "不远处传来低沉的咆哮声...有野兽！\n";
                if (shelter_level == 0) {
                    storm_damage = rand() % 21 + 10;
                    health = health - storm_damage;
                    cout << "你暴露在空旷处，与野兽正面遭遇！一番搏斗后...生命值 -";
                    cout << storm_damage << "\n";
                } else if (shelter_level == 1) {
                    storm_damage = rand() % 11 + 5;
                    health = health - storm_damage;
                    cout << "野兽在棚屋外徘徊了很久，你躲在里面大气不敢出。生命值 -";
                    cout << storm_damage << " (惊吓)\n";
                } else {
                    cout << "野兽在远处嚎叫，但你的庇护所非常安全。它最终悻悻离去。\n";
                }
                morale = morale - 5;
                cout << "野兽的威胁让你感到不安，士气 -5\n";

            // 星空之夜事件
            } else if (event_roll == 2) {
                cout << "今晚万里无云，银河横跨夜空，壮观极了。\n";
                find_amount = rand() % 11 + 10;
                morale = morale + find_amount;
                if (morale > 100) {
                    morale = 100;
                }
                cout << "凝望星空让你忘却了困境，内心重新燃起希望。士气 +";
                cout << find_amount << "\n";

            // 平安夜事件
            } else {
                cout << "今夜风平浪静，只有海浪温柔地拍打着沙滩。\n";
                cout << "你安稳地度过了一夜。\n";
            }

            // 夜间事件后钳制所有数值
            if (health > 100) { health = 100; }
            if (health < 0) { health = 0; }
            if (hunger < 0) { hunger = 0; }
            if (thirst < 0) { thirst = 0; }
            if (morale > 100) { morale = 100; }
            if (morale < 0) { morale = 0; }

            // ===== 夜间后再次检测死亡 =====
            if (health <= 0) {
                gameOver = 1;
                won = 0;
                cout << "\n";
                cout << "╔══════════════════════════════════════════╗\n";
                cout << "║                                          ║\n";
                cout << "║  夜晚的遭遇让你伤重不治...               ║\n";
                cout << "║  黑暗吞噬了最后的生命之光。             ║\n";
                cout << "║  明天太阳照常升起，但你再也看不到了。   ║\n";
                cout << "║                                          ║\n";
                cout << "║           †  安  息  †                 ║\n";
                cout << "║                                          ║\n";
                cout << "╚══════════════════════════════════════════╝\n";
                continue;
            }
            if (morale <= 0) {
                gameOver = 1;
                won = 0;
                cout << "\n";
                cout << "╔══════════════════════════════════════════╗\n";
                cout << "║                                          ║\n";
                cout << "║  连续的打击让你彻底绝望...               ║\n";
                cout << "║  你坐在海滩上，望着无尽的大海。         ║\n";
                cout << "║  求生之火在心中彻底熄灭了。             ║\n";
                cout << "║                                          ║\n";
                cout << "║           †  安  息  †                 ║\n";
                cout << "║                                          ║\n";
                cout << "╚══════════════════════════════════════════╝\n";
                continue;
            }

            // ===== 胜利条件检测 =====
            if (radio_parts >= 3 && shelter_level >= 2) {
                gameOver = 1;
                won = 1;

                // 第一阶段：宣布条件达成
                cout << "\n";
                cout << "╔══════════════════════════════════════════╗\n";
                cout << "║                                          ║\n";
                cout << "║  你集齐了所有无线电零件！                ║\n";
                cout << "║  你的庇护所足够牢固，可以安全组装设备！  ║\n";
                cout << "║                                          ║\n";
                cout << "╚══════════════════════════════════════════╝\n\n";

                // 第二阶段：点燃信号火（逐步描述）
                cout << "你在庇护所旁清出一块空地，将干燥的枯枝堆成锥形...\n";
                cout << "从无线电零件中拆下电池，用导线打出火花...\n\n";
                for (clean_char = 0; clean_char < 120000000; clean_char = clean_char + 1) { }

                cout << "   .    .    .    .    .\n";
                cout << "    火花溅落在枯叶上，一缕青烟升起...\n\n";
                for (clean_char = 0; clean_char < 120000000; clean_char = clean_char + 1) { }

                cout << "            🔥\n";
                cout << "          🔥🔥🔥\n";
                cout << "        🔥🔥🔥🔥🔥\n";
                cout << "      信号火堆燃起来了！火焰直冲云霄！\n\n";
                for (clean_char = 0; clean_char < 120000000; clean_char = clean_char + 1) { }

                cout << "      浓烟升上高空，在蓝天映衬下格外显眼...\n\n";
                for (clean_char = 0; clean_char < 120000000; clean_char = clean_char + 1) { }

                cout << "============================================\n\n";

                // 第三阶段：等待救援 — for 循环倒计时
                cout << "你坐在火堆旁，望着地平线，静静等待...\n";
                cout << "时间一分一秒过去...\n\n";

                for (countdown = 3; countdown >= 1; countdown = countdown - 1) {
                    cout << "      " << countdown << " ...\n";
                    for (clean_char = 0; clean_char < 150000000; clean_char = clean_char + 1) { }
                }

                // 第四阶段：发现救援
                cout << "\n";
                cout << "轰隆隆————\n";
                cout << "远处的地平线上，出现了一个小黑点...\n\n";
                for (clean_char = 0; clean_char < 120000000; clean_char = clean_char + 1) { }

                cout << "黑点越来越大，越来越清晰...\n";
                cout << "那是一架直升机！\n\n";
                for (clean_char = 0; clean_char < 120000000; clean_char = clean_char + 1) { }

                cout << "╔══════════════════════════════════════════╗\n";
                cout << "║                                          ║\n";
                cout << "║    🚁  直升机发现了你的信号火堆！        ║\n";
                cout << "║                                          ║\n";
                cout << "║    它缓缓下降，悬停在沙滩上空...         ║\n";
                cout << "║    救援人员向你伸出大拇指：              ║\n";
                cout << "║    「干得漂亮，幸存者！我们带你回家！」  ║\n";
                cout << "║                                          ║\n";
                cout << "║          ★  你 得 救 了！ ★             ║\n";
                cout << "║                                          ║\n";
                cout << "╚══════════════════════════════════════════╝\n";
                break;
            }

            // 天数推进
            day = day + 1;

        }   // 结束内层 while (gameOver == 0)

        // ========== 结算界面 ==========
        totalScore = day * 100;
        totalScore = totalScore + health;
        totalScore = totalScore + hunger;
        totalScore = totalScore + thirst;
        totalScore = totalScore + morale;

        if (won == 1) {
            totalScore = totalScore + 500;
        }

        cout << "\n";
        cout << "╔══════════════════════════════════════════╗\n";
        if (won == 1) {
            cout << "║          ★  游  戏  结  束  ★           ║\n";
        } else {
            cout << "║            游  戏  结  束                ║\n";
        }
        cout << "╚══════════════════════════════════════════╝\n";
        if (won == 1) {
            cout << "         结  果:  🎉  胜  利 ！\n\n";
        } else {
            cout << "         结  果:  失  败  ...\n\n";
        }
        cout << "  ┌──────────────────────────────────────────┐\n";
        cout << "  │              最  终  状  态              │\n";
        cout << "  ├──────────────────────────────────────────┤\n";
        cout << "  │  存活天数:      " << day;
        // 对齐天数
        if (day < 10) { cout << " "; }
        if (day < 100) { cout << " "; }
        cout << "                       │\n";
        cout << "  │  最终生命值:    " << health << "/100";
        if (health < 10) { cout << " "; }
        if (health < 100) { cout << " "; }
        cout << "                       │\n";
        cout << "  │  最终饱食度:    " << hunger << "/100";
        if (hunger < 10) { cout << " "; }
        if (hunger < 100) { cout << " "; }
        cout << "                       │\n";
        cout << "  │  最终饮水量:    " << thirst << "/100";
        if (thirst < 10) { cout << " "; }
        if (thirst < 100) { cout << " "; }
        cout << "                       │\n";
        cout << "  │  最终士气值:    " << morale << "/100";
        if (morale < 10) { cout << " "; }
        if (morale < 100) { cout << " "; }
        cout << "                       │\n";
        cout << "  │  庇护所等级:    " << shelter_level << "/3                          │\n";
        cout << "  │  无线电零件:    " << radio_parts << "/3                          │\n";
        cout << "  ├──────────────────────────────────────────┤\n";
        cout << "  │              得  分  明  细              │\n";
        cout << "  ├──────────────────────────────────────────┤\n";
        cout << "  │  基础分 (天数×100):  " << day << "×100 = " << day * 100;
        if (day * 100 < 1000) { cout << " "; }
        cout << "               │\n";
        cout << "  │  状态分:             + " << health + hunger + thirst + morale;
        if (health + hunger + thirst + morale < 100) { cout << " "; }
        cout << "                  │\n";
        if (won == 1) {
            cout << "  │  获救奖励:           + 500                       │\n";
        }
        cout << "  │  ───────────────────────────────────  │\n";
        cout << "  │  总  分:              " << totalScore;
        if (totalScore < 1000) { cout << " "; }
        if (totalScore < 100) { cout << " "; }
        cout << "                       │\n";
        cout << "  └──────────────────────────────────────────┘\n";

        // 评级
        cout << "\n  ";
        if (won == 1) {
            if (totalScore >= 2500) {
                cout << "🏆 评级: S — 传奇幸存者！你简直是贝爷转世！";
            } else if (totalScore >= 2000) {
                cout << "🌟 评级: A — 优秀！你成功活了下来！";
            } else if (totalScore >= 1500) {
                cout << "👍 评级: B — 不错！获救了，虽然过程艰辛。";
            } else {
                cout << "✅ 评级: C — 勉强获救，下次争取更好！";
            }
        } else {
            if (day >= 15) {
                cout << "💀 评级: B — 坚持了很久，可惜功亏一篑...";
            } else if (day >= 8) {
                cout << "💀 评级: C — 表现一般，多注意资源平衡。";
            } else if (day >= 4) {
                cout << "💀 评级: D — 还需努力，优先保证食物和水。";
            } else {
                cout << "💀 评级: E — 荒岛求生不是你的强项...";
            }
        }
        cout << "\n";

        // ===== 重玩询问（do-while 输入验证）=====
        do {
            cout << "\n再来一局？ (1 = 再来一次, 0 = 退出): ";
            cin >> playAgain;

            if (cin.fail()) {
                cin.clear();
                clean_char = cin.get();
                while (clean_char != '\n' && clean_char != EOF) {
                    clean_char = cin.get();
                }
                playAgain = -1;
            }

            if (playAgain != 0 && playAgain != 1) {
                cout << "[!] 请输入 0 或 1！\n";
            }
        } while (playAgain != 0 && playAgain != 1);

    }   // 结束外层 while (playAgain == 1)

    // ========== 结束语 ==========
    cout << "\n";
    cout << "╔══════════════════════════════════════════╗\n";
    cout << "║                                          ║\n";
    cout << "║    感谢游玩「海岛求生」！               ║\n";
    cout << "║                                          ║\n";
    cout << "║    无论你是否获救，这段经历都属于你。   ║\n";
    cout << "║    希望你在真实生活中永远不会流落荒岛！ ║\n";
    cout << "║                                          ║\n";
    cout << "║        🏝  See you next time!  🏝        ║\n";
    cout << "║                                          ║\n";
    cout << "╚══════════════════════════════════════════╝\n";
    cout << "\n";

    return 0;
}
