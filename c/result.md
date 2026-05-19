# C 两种方法性能测试结果（direct vs lookup，优化版）

## 编译与运行
```bash
gcc -O3 -march=native benchmark.c -o benchmark
./benchmark
```

## 5 次统计（由 `benchmark_runner.py` 汇总）
- direct: best=0.086480, mean=0.090064, p95=0.096194
- lookup: best=0.185478, mean=0.193536, p95=0.215521
- ratio_direct_over_lookup: best=0.446332, mean=0.465889, p95=0.477826

## 结论
- 在当前 C 优化实现下，**direct 更快**（lookup 约慢 2.15x 左右，按 mean 估算）。
