#!/usr/bin/env python3
import re
import statistics
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent

PAT = {
    "direct": re.compile(r"direct_seconds=([0-9]+\.[0-9]+)"),
    "lookup": re.compile(r"lookup_seconds=([0-9]+\.[0-9]+)"),
    "ratio": re.compile(r"ratio_direct_over_lookup=([0-9]+\.[0-9]+)"),
}

LANGS = [
    {
        "name": "C",
        "build": "gcc -O3 -march=native c/benchmark.c -o c/benchmark",
        "run": "./c/benchmark",
    },
    {
        "name": "C++",
        "build": "g++ -O3 -march=native cpp/benchmark.cpp -o cpp/benchmark",
        "run": "./cpp/benchmark",
    },
    {
        "name": "Java",
        "build": "javac java/Benchmark.java",
        "run": "java -cp java Benchmark",
    },
]


def parse_value(pattern, text, key, lang):
    m = pattern.search(text)
    if not m:
        raise RuntimeError(f"{lang}: cannot parse {key}")
    return float(m.group(1))


def run_cmd(cmd):
    return subprocess.check_output(cmd, shell=True, text=True, cwd=ROOT)


def main():
    repeats = 5
    for cfg in LANGS:
        print(f"== {cfg['name']} ==")
        print(run_cmd(cfg["build"]).strip())
        directs, lookups, ratios = [], [], []
        for _ in range(repeats):
            out = run_cmd(cfg["run"])
            directs.append(parse_value(PAT["direct"], out, "direct_seconds", cfg["name"]))
            lookups.append(parse_value(PAT["lookup"], out, "lookup_seconds", cfg["name"]))
            ratios.append(parse_value(PAT["ratio"], out, "ratio_direct_over_lookup", cfg["name"]))

        print(f"direct  best={min(directs):.6f} mean={statistics.mean(directs):.6f} p95={sorted(directs)[-1]:.6f}")
        print(f"lookup  best={min(lookups):.6f} mean={statistics.mean(lookups):.6f} p95={sorted(lookups)[-1]:.6f}")
        print(f"ratio   best={min(ratios):.6f} mean={statistics.mean(ratios):.6f} p95={sorted(ratios)[-1]:.6f}")
        print()


if __name__ == "__main__":
    main()
