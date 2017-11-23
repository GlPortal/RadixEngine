
#ifndef RADIX_UTIL_PROFILING_HPP
#define RADIX_UTIL_PROFILING_HPP

#include <easy/profiler.h>

#define PROFILER_BLOCK(...) EASY_BLOCK(__VA_ARGS__)
#define PROFILER_NONSCOPED_BLOCK(...) EASY_NONSCOPED_BLOCK(__VA_ARGS__)
#define PROFILER_FUNCTION(...) EASY_FUNCTION(__VA_ARGS__)
#define PROFILER_END_BLOCK EASY_END_BLOCK
#define PROFILER_EVENT(...) EASY_EVENT(__VA_ARGS__)
#define PROFILER_PROFILER_ENABLE EASY_PROFILER_ENABLE
#define PROFILER_PROFILER_DISABLE EASY_PROFILER_DISABLE
#define PROFILER_THREAD_SCOPE(...) EASY_THREAD_SCOPE(__VA_ARGS__)
#define PROFILER_MAIN_THREAD EASY_MAIN_THREAD

#endif /* RADIX_UTIL_PROFILING_HPP */

