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

enum class kCoroutineStatus : int
{
	Dead = 0,
    Ready = 1,
    Running = 2,
    Suspend = 3,
};

COROUTINE_NAMESPACE_END

#endif
