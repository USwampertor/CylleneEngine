/**
 * @file cyUnitTesting.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for UnitTesting.
 */

#pragma once

#include "cyUtilitiesPrerequisites.h"

#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench/nanobench.h>

namespace CYLLENE_SDK {

using Benchmark = ankerl::nanobench::Bench;

// Making Catch2-like Benchmark tests
#define DONOTOPTIMIZE(...) ankerl::nanobench::doNotOptimizeAway(__VA_ARGS__);

#define BENCHMARK(name, ...) Benchmark().run(name, __VA_ARGS__);

#define BENCHMARKEPOCHS(name, times, ...) \
Benchmark().epochs(times).run(name, __VA_ARGS__);

}

