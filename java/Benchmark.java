public class Benchmark {
    static int canBeatEasyFast(int a, int b) {
        int gt = (a > b) ? 1 : 0;
        int sameType = ((((a ^ b) & 0xF0) == 0) ? 1 : 0);
        int highType = ((a & 0x80) >>> 7);
        return gt & (sameType | highType);
    }

    static byte[] buildTable() {
        byte[] table = new byte[256 * 256];
        for (int a = 0; a < 256; a++) {
            int base = a << 8;
            for (int b = 0; b < 256; b++) {
                table[base | b] = (byte) canBeatEasyFast(a, b);
            }
        }
        return table;
    }

    static long benchDirect(int rounds) {
        long c = 0L;
        for (int r = 0; r < rounds; r++) {
            for (int a = 0; a < 256; a++) {
                for (int b = 0; b < 256; b++) {
                    c += canBeatEasyFast(a, b);
                }
            }
        }
        return c;
    }

    static long benchLookup(byte[] table, int rounds) {
        long c = 0L;
        for (int r = 0; r < rounds; r++) {
            for (int a = 0; a < 256; a++) {
                int base = a << 8;
                for (int b = 0; b < 256; b++) {
                    c += table[base | b];
                }
            }
        }
        return c;
    }

    public static void main(String[] args) {
        final int warmupRounds = 800;
        final int rounds = 4000;
        byte[] table = buildTable();

        benchDirect(warmupRounds);
        benchLookup(table, warmupRounds);

        long checksumDirect = benchDirect(1);
        long checksumLookup = benchLookup(table, 1);
        System.out.println("correct=" + (checksumDirect == checksumLookup));
        System.out.println("true_count=" + checksumDirect);

        long start = System.nanoTime();
        long d = benchDirect(rounds);
        long end = System.nanoTime();
        double directSeconds = (end - start) / 1_000_000_000.0;

        start = System.nanoTime();
        long l = benchLookup(table, rounds);
        end = System.nanoTime();
        double lookupSeconds = (end - start) / 1_000_000_000.0;

        System.out.println("direct_checksum=" + d);
        System.out.println("lookup_checksum=" + l);
        System.out.printf("direct_seconds=%.6f%n", directSeconds);
        System.out.printf("lookup_seconds=%.6f%n", lookupSeconds);
        System.out.printf("ratio_direct_over_lookup=%.6f%n", directSeconds / lookupSeconds);
    }
}
