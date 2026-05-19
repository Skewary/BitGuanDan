# Java 两种方法性能测试结果（direct vs lookup，优化版）

## 编译与运行
```bash
javac Benchmark.java
java Benchmark
```

## 5 次统计（由 `benchmark_runner.py` 汇总）
- direct: best=0.332394, mean=0.346107, p95=0.357987
- lookup: best=0.153697, mean=0.158897, p95=0.163807
- ratio_direct_over_lookup: best=2.106366, mean=2.178592, p95=2.242089

## 结论
- 在当前 Java 优化实现下，**lookup 更快**（约快 2.18x，按 mean 估算）。
