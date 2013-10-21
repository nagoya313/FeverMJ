#ifndef FEVERMJ_UTILITY_LOG_HPP_
#define FEVERMJ_UTILITY_LOG_HPP_
#include <DxLib.h>

#ifndef NDEBUGLOG
#define FEVERMJ_LOG(...) printfDx(__VA_ARGS__)
#else
#define FEVERMJ_LOG(...)
#endif

#endif
