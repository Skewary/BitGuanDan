# BitGuanDan

This repository contains the core logic for representing and comparing cards in **GuanDan (掼蛋)** using compact 8-bit encoding.

<img width="1306" height="1204" alt="BitGuandan" src="https://github.com/user-attachments/assets/6fc84067-3ac2-43ef-aacf-4ca914b1ce1c" />

## 核心思路

1. **8-bit 编码结构**

   * 高 4 位 (`type_code`) 表示牌型
   * 低 4 位 (`core_rank`) 表示主牌点数
   * 这样一个字节即可表示一手牌（单牌、对子、炸弹、顺子等）

2. **牌型模式**
   为方便逻辑处理，每种牌型映射到一个模式（Mode），便于判断是否可拆或顺子处理：

| Type Code | 牌型               | 模式 (Mode)   |
| --------- | ---------------- | ----------- |
| 0x0       | PASS             | SAME        |
| 0x1       | SINGLE           | SAME        |
| 0x2       | PAIR             | SAME        |
| 0x3       | TRIPLE           | SAME        |
| 0x4       | TRIPLE_WITH_PAIR | TRIPLE_PAIR |
| 0x5       | STRAIGHT         | STRAIGHT    |
| 0x6       | PAIR_STRAIGHT    | PAIR_CHAIN  |
| 0x7       | STEEL_PLATE      | STEEL       |
| 0x8       | BOMB_4           | SAME        |
| 0x9       | BOMB_5           | SAME        |
| 0xA       | STRAIGHT_FLUSH   | STRAIGHT    |
| 0xB       | BOMB_6           | SAME        |
| 0xC       | BOMB_7           | SAME        |
| 0xD       | BOMB_8           | SAME        |
| 0xE       | BOMB_9           | SAME        |
| 0xF       | JOKER_BOMB       | SAME        |

3. **核心牌点 (core_rank)**

   * 牌点从 2 到 A，再到小王、大王
   * 值用 0x0~0xF 表示，便于位运算比较

| Core Rank | 点数    |
| --------- | ----- |
| 0x0       | 2     |
| 0x1       | 3     |
| 0x2       | 4     |
| 0x3       | 5     |
| 0x4       | 6     |
| 0x5       | 7     |
| 0x6       | 8     |
| 0x7       | 9     |
| 0x8       | 10    |
| 0x9       | J     |
| 0xA       | Q     |
| 0xB       | K     |
| 0xC       | A     |
| 0xD       | LEVEL |
| 0xE       | SJ    |
| 0xF       | BJ    |

4. **出牌比较逻辑**

   * `can_beat(a, b)` 判断 `a` 是否能压 `b`，遵循：

     * 同牌型按核心牌点比较
     * 炸弹可压任意非炸弹
     * 炸弹之间按炸弹等级比较
   * `can_beat_easy(a, b)` 是位运算优化版本，提高判断效率

5. **优势**

   * **紧凑**：一字节即可表示所有牌型信息
   * **高效**：位运算快速判断大小
   * **可扩展**：新牌型可直接扩展枚举，无需重写逻辑

6. **性能对比**

| 方法     | 时间 (s)    | 说明              |
| ------ | --------- | --------------- |
| direct | 0.3845893 | 直接计算            |
| lookup | 0.5370041 | 查表（预先生成比较结果）    |
| ratio  | 0.716     | direct / lookup |


---



