# C++ 两种方法性能测试结果（direct vs lookup，优化版）

## 编译与运行
```bash
g++ -O3 -march=native benchmark.cpp -o benchmark
./benchmark
```

## 5 次统计（由 `benchmark_runner.py` 汇总）
- direct: best=0.085889, mean=0.088052, p95=0.090821
- lookup: best=0.184318, mean=0.191057, p95=0.194607
- ratio_direct_over_lookup: best=0.451241, mean=0.460900, p95=0.466689

## 结论
- 在当前 C++ 优化实现下，**direct 更快**（lookup 约慢 2.17x 左右，按 mean 估算）。
