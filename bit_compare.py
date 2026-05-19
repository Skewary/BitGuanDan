# =========================================================
# 8bit 编码
# 高4bit: type_code
# 低4bit: core_rank
# =========================================================

# 非炸弹
TYPE_PASS = 0x0
TYPE_SINGLE = 0x1
TYPE_PAIR = 0x2
TYPE_TRIPLE = 0x3
TYPE_TRIPLE_WITH_PAIR = 0x4
TYPE_STRAIGHT = 0x5
TYPE_PAIR_STRAIGHT = 0x6
TYPE_STEEL_PLATE = 0x7

# 炸弹
TYPE_BOMB_4 = 0x8
TYPE_BOMB_5 = 0x9
TYPE_STRAIGHT_FLUSH = 0xA
TYPE_BOMB_6 = 0xB
TYPE_BOMB_7 = 0xC
TYPE_BOMB_8 = 0xD
TYPE_BOMB_9 = 0xE
TYPE_JOKER_BOMB = 0xF

# =========================================================
# core_rank 编码
# 2 < 3 < 4 < ... < A < LEVEL < SJ < BJ
# =========================================================
CORE_2 = 0x0
CORE_3 = 0x1
CORE_4 = 0x2
CORE_5 = 0x3
CORE_6 = 0x4
CORE_7 = 0x5
CORE_8 = 0x6
CORE_9 = 0x7
CORE_10 = 0x8
CORE_J = 0x9
CORE_Q = 0xA
CORE_K = 0xB
CORE_A = 0xC
CORE_LEVEL = 0xD
CORE_SJ = 0xE
CORE_BJ = 0xF

MODE_SAME = 0
MODE_TRIPLE_PAIR = 1
MODE_STRAIGHT = 2
MODE_PAIR_CHAIN = 3
MODE_STEEL = 4

TYPE_MODE = [
    MODE_SAME,  # 0x0 PASS
    MODE_SAME,  # 0x1 SINGLE
    MODE_SAME,  # 0x2 PAIR
    MODE_SAME,  # 0x3 TRIPLE
    MODE_TRIPLE_PAIR,  # 0x4 TRIPLE_WITH_PAIR
    MODE_STRAIGHT,  # 0x5 STRAIGHT
    MODE_PAIR_CHAIN,  # 0x6 PAIR_STRAIGHT
    MODE_STEEL,  # 0x7 STEEL_PLATE
    MODE_SAME,  # 0x8 BOMB_4
    MODE_SAME,  # 0x9 BOMB_5
    MODE_STRAIGHT,  # 0xA STRAIGHT_FLUSH
    MODE_SAME,  # 0xB BOMB_6
    MODE_SAME,  # 0xC BOMB_7
    MODE_SAME,  # 0xD BOMB_8
    MODE_SAME,  # 0xE BOMB_9
    MODE_SAME,  # 0xF JOKER_BOMB
]

def can_beat(a: int, b: int) -> bool:
    ta = a >> 4
    tb = b >> 4
    if ta == tb:
        return (a & 0xF) > (b & 0xF)
    if ta >= 8:
        return tb < 8 or ta > tb
    return False

def can_beat_easy(a: int, b: int) -> bool:
    # ta = a >> 4
    # tb = b >> 4
    # return (ta == tb or ta >= 8) and a > b
    return (a > b) and ((((a ^ b) & 0xF0) == 0) or (a & 0x80)) 
