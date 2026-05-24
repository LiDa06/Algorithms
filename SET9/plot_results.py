#!/usr/bin/env python3
"""Build plots for string_sort_lab.cpp benchmark CSV.

Usage:
    python3 plot_results.py results.csv plots
"""
from __future__ import annotations

import sys
from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd


def plot_metric(df: pd.DataFrame, dataset: str, metric: str, ylabel: str, out_dir: Path) -> None:
    part = df[df["dataset"] == dataset].copy()
    if part.empty:
        return

    plt.figure(figsize=(10, 6))
    for algorithm, group in part.groupby("algorithm"):
        group = group.sort_values("n")
        plt.plot(group["n"], group[metric], marker="o", linewidth=1.5, markersize=3, label=algorithm)

    plt.title(f"{ylabel}: {dataset}")
    plt.xlabel("Количество строк, n")
    plt.ylabel(ylabel)
    plt.grid(True, alpha=0.3)
    plt.legend(fontsize=8)
    plt.tight_layout()
    filename = out_dir / f"{dataset}_{metric}.png"
    plt.savefig(filename, dpi=180)
    plt.close()


def main() -> int:
    csv_path = Path(sys.argv[1]) if len(sys.argv) >= 2 else Path("results.csv")
    out_dir = Path(sys.argv[2]) if len(sys.argv) >= 3 else Path("plots")
    out_dir.mkdir(parents=True, exist_ok=True)

    df = pd.read_csv(csv_path)
    required = {
        "dataset", "algorithm", "n", "avg_us", "median_us",
        "avg_char_comparisons", "avg_char_inspections",
    }
    missing = required - set(df.columns)
    if missing:
        raise ValueError(f"CSV does not contain required columns: {sorted(missing)}")

    for dataset in sorted(df["dataset"].unique()):
        plot_metric(df, dataset, "avg_us", "Среднее время, мкс", out_dir)
        plot_metric(df, dataset, "avg_char_comparisons", "Посимвольные сравнения", out_dir)
        plot_metric(df, dataset, "avg_char_inspections", "Обращения к символам", out_dir)

    summary = (
        df.groupby(["dataset", "algorithm"], as_index=False)
          .agg(avg_time_us=("avg_us", "mean"),
               avg_cmp=("avg_char_comparisons", "mean"),
               avg_inspections=("avg_char_inspections", "mean"))
          .sort_values(["dataset", "avg_time_us"])
    )
    summary_path = out_dir / "summary_by_dataset_algorithm.csv"
    summary.to_csv(summary_path, index=False)
    print(f"Saved plots and summary to: {out_dir}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
