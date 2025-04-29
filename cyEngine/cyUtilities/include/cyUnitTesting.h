/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file   	cyUnitTesting.h
 * @author 	Marco "Swampy" Millan
 * @date 	  2024/11/14
 * @brief 	This is to have all doctest related systems in an 
 *          include file
 *
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
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



