#ifndef _COROUTINE_COMMON_H__
#define _COROUTINE_COMMON_H__

#define COROUTINE_NAMESPACE_START namespace coroutine {
#define COROUTINE_NAMESPACE_END }
#define COROUTINE_NAMESPACE_USE using namespace coroutine;

#ifndef STACK_DEFAULT
#define STACK_DEFAULT 1024 * 1024
#endif // STACK_DEFAULT

#include <assert.h>
#include <vector>
#include <queue>
#include <thread>
#include <future>

COROUTINE_NAMESPACE_START

enum class kCoroutineStatus
{
    Ready = 0,
    Running = 1,
    Suspend = 2,
    Dead = 3
};

COROUTINE_NAMESPACE_END

#endif
