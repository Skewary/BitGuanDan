# compare_with_lookup 思路跨语言对比（direct vs lookup，优化版）

测试日期：2026-05-19（UTC）

对齐 `compare_with_lookup.py`：
- `direct`：直接执行 `can_beat_easy` 逻辑。
- `lookup`：预先构建 `256*256` 查表，运行时 `(a<<8)|b` 索引。

## 本次额外优化
- direct 改为 branch-minimized 的位运算组合。
- C/C++ 使用 `-O3 -march=native`。
- Java 增加 warmup 阶段，降低 JIT 冷启动影响。
- 新增 `benchmark_runner.py` 统一执行 5 次并输出 best/mean/p95。

## 5 次统计汇总

| 语言 | direct best | direct mean | lookup best | lookup mean | ratio mean | 更快方法 |
|---|---:|---:|---:|---:|---:|---|
| C | 0.086480 | 0.090064 | 0.185478 | 0.193536 | 0.465889 | direct |
| C++ | 0.085889 | 0.088052 | 0.184318 | 0.191057 | 0.460900 | direct |
| Java | 0.332394 | 0.346107 | 0.153697 | 0.158897 | 2.178592 | lookup |

## 结论
- “lookup 一定更快”不成立：在本机 C/C++ 的极致位运算版本下，direct 更优；Java 仍是 lookup 更优。
- 是否采用查表，建议在目标机型上按相同编译/JVM 参数做统计后再定。
