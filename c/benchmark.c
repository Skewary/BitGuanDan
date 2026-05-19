#include <stdint.h>
#include <stdio.h>
#include <time.h>

#if defined(__GNUC__) || defined(__clang__)
#define ALWAYS_INLINE __attribute__((always_inline)) inline
#else
#define ALWAYS_INLINE inline
#endif

static ALWAYS_INLINE uint8_t can_beat_easy_fast(uint8_t a, uint8_t b) {
    uint8_t gt = (uint8_t)(a > b);
    uint8_t same_type = (uint8_t)((((uint8_t)(a ^ b)) & 0xF0u) == 0u);
    uint8_t high_type = (uint8_t)((a & 0x80u) != 0u);
    return (uint8_t)(gt & (same_type | high_type));
}

static void build_table(uint8_t table[65536]) {
    for (uint32_t a = 0; a < 256; ++a) {
        uint32_t base = a << 8;
        for (uint32_t b = 0; b < 256; ++b) {
            table[base | b] = can_beat_easy_fast((uint8_t)a, (uint8_t)b);
        }
    }
}

static uint64_t bench_direct(uint32_t rounds) {
    uint64_t c = 0;
    for (uint32_t r = 0; r < rounds; ++r) {
        for (uint32_t a = 0; a < 256; ++a) {
            for (uint32_t b = 0; b < 256; ++b) {
                c += can_beat_easy_fast((uint8_t)a, (uint8_t)b);
            }
        }
    }
    return c;
}

static uint64_t bench_lookup(const uint8_t table[65536], uint32_t rounds) {
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

static double elapsed_sec(struct timespec s, struct timespec e) {
    return (e.tv_sec - s.tv_sec) + (e.tv_nsec - s.tv_nsec) / 1e9;
}

int main(void) {
    const uint32_t rounds = 4000;
    uint8_t table[65536];
    build_table(table);

    uint64_t checksum_direct = bench_direct(1);
    uint64_t checksum_lookup = bench_lookup(table, 1);
    printf("correct=%s\n", checksum_direct == checksum_lookup ? "true" : "false");
    printf("true_count=%llu\n", (unsigned long long)checksum_direct);

    struct timespec s, e;
    clock_gettime(CLOCK_MONOTONIC, &s);
    uint64_t d = bench_direct(rounds);
    clock_gettime(CLOCK_MONOTONIC, &e);
    double t_direct = elapsed_sec(s, e);

    clock_gettime(CLOCK_MONOTONIC, &s);
    uint64_t l = bench_lookup(table, rounds);
    clock_gettime(CLOCK_MONOTONIC, &e);
    double t_lookup = elapsed_sec(s, e);

    printf("direct_checksum=%llu\n", (unsigned long long)d);
    printf("lookup_checksum=%llu\n", (unsigned long long)l);
    printf("direct_seconds=%.6f\n", t_direct);
    printf("lookup_seconds=%.6f\n", t_lookup);
    printf("ratio_direct_over_lookup=%.6f\n", t_direct / t_lookup);
    return 0;
}
