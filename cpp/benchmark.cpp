#include <array>
#include <chrono>
#include <cstdint>
#include <iostream>

#if defined(__GNUC__) || defined(__clang__)
#define ALWAYS_INLINE __attribute__((always_inline)) inline
#else
#define ALWAYS_INLINE inline
#endif

static ALWAYS_INLINE uint8_t can_beat_easy_fast(uint8_t a, uint8_t b) {
    uint8_t gt = static_cast<uint8_t>(a > b);
    uint8_t same_type = static_cast<uint8_t>((((a ^ b) & 0xF0u) == 0u));
    uint8_t high_type = static_cast<uint8_t>((a & 0x80u) != 0u);
    return static_cast<uint8_t>(gt & (same_type | high_type));
}

static std::array<uint8_t, 65536> build_table() {
    std::array<uint8_t, 65536> table{};
    for (uint32_t a = 0; a < 256; ++a) {
        uint32_t base = a << 8;
        for (uint32_t b = 0; b < 256; ++b) {
            table[base | b] = can_beat_easy_fast(static_cast<uint8_t>(a), static_cast<uint8_t>(b));
        }
    }
    return table;
}

static uint64_t bench_direct(uint32_t rounds) {
    uint64_t c = 0;
    for (uint32_t r = 0; r < rounds; ++r) {
        for (uint32_t a = 0; a < 256; ++a) {
            for (uint32_t b = 0; b < 256; ++b) {
                c += can_beat_easy_fast(static_cast<uint8_t>(a), static_cast<uint8_t>(b));
            }
        }
    }
    return c;
}

static uint64_t bench_lookup(const std::array<uint8_t, 65536>& table, uint32_t rounds) {
    uint64_t c = 0;
    for (uint32_t r = 0; r < rounds; ++r) {
        for (uint32_t a = 0; a < 256; ++a) {
            uint32_t base = a << 8;
            for (uint32_t b = 0; b < 256; ++b) {
                c += table[base | b];
            }
        }
    }
    return c;
}

int main() {
    constexpr uint32_t rounds = 4000;
    auto table = build_table();

    auto checksum_direct = bench_direct(1);
    auto checksum_lookup = bench_lookup(table, 1);
    std::cout << "correct=" << (checksum_direct == checksum_lookup ? "true" : "false") << "\n";
    std::cout << "true_count=" << checksum_direct << "\n";

    auto s = std::chrono::steady_clock::now();
    auto d = bench_direct(rounds);
    auto e = std::chrono::steady_clock::now();
    double t_direct = std::chrono::duration<double>(e - s).count();

    s = std::chrono::steady_clock::now();
    auto l = bench_lookup(table, rounds);
    e = std::chrono::steady_clock::now();
    double t_lookup = std::chrono::duration<double>(e - s).count();

    std::cout << "direct_checksum=" << d << "\n";
    std::cout << "lookup_checksum=" << l << "\n";
    std::cout << "direct_seconds=" << t_direct << "\n";
    std::cout << "lookup_seconds=" << t_lookup << "\n";
    std::cout << "ratio_direct_over_lookup=" << (t_direct / t_lookup) << "\n";
    return 0;
}
