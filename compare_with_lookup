import timeit
from pathlib import Path

# ==============================
# 原函数
# ==============================

def can_beat_easy(a: int, b: int) -> bool:
    # ta = a >> 4
    # tb = b >> 4
    # return (ta == tb or ta >= 8) and a > b
    # return (a > b) and ((((a ^ b) & 0xF0) == 0) or ((a & 0x80) != 0)) # 完全bool
    return (a > b) and ((((a ^ b) & 0xF0) == 0) or (a & 0x80)) # hui


# ==============================
# 1. 生成查表
# ==============================

def build_table():
    table = bytearray(256 * 256)
    for a in range(256):
        base = a << 8
        for b in range(256):
            table[base | b] = 1 if can_beat_easy(a, b) else 0
    return table


# ==============================
# 2. 保存 / 加载
# ==============================

def save_table(table, path="can_beat_easy_table.bin"):
    Path(path).write_bytes(table)


def load_table(path="can_beat_easy_table.bin"):
    return Path(path).read_bytes()


# ==============================
# 3. 查表版本
# ==============================

def make_lookup(table_bytes):
    def can_beat_lookup(a: int, b: int) -> bool:
        return bool(table_bytes[(a << 8) | b])
    return can_beat_lookup


# ==============================
# 4. benchmark
# ==============================

def benchmark():
    table = build_table()
    save_table(table)

    TABLE = load_table()
    can_beat_lookup = make_lookup(TABLE)

    pairs = [(a, b) for a in range(256) for b in range(256)]

    def bench_direct():
        c = 0
        for a, b in pairs:
            c += can_beat_easy(a, b)
        return c

    def bench_lookup():
        c = 0
        for a, b in pairs:
            c += can_beat_lookup(a, b)
        return c

    # 正确性校验
    d = bench_direct()
    l = bench_lookup()
    print("correct:", d == l)
    print("true_count:", d)

    # 跑 benchmark
    direct_times = timeit.repeat(
        "bench_direct()",
        globals=locals(),
        number=50,
        repeat=5
    )

    lookup_times = timeit.repeat(
        "bench_lookup()",
        globals=locals(),
        number=50,
        repeat=5
    )

    best_direct = min(direct_times)
    best_lookup = min(lookup_times)

    print("\n=== benchmark ===")
    print("direct :", best_direct)
    print("lookup :", best_lookup)
    print("ratio (direct/lookup):", best_direct / best_lookup)

    print("\nraw direct:", direct_times)
    print("raw lookup:", lookup_times)



if __name__ == "__main__":
    benchmark()
