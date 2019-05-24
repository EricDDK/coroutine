#ifndef _COROUTINE_COMMON_H__
#define _COROUTINE_COMMON_H__

#ifndef COROUTINE_SIZE_T
typedef unsigned int size_t;
#endif

#define COROUTINE_NAMESPACE_START namespace coroutine {
#define COROUTINE_NAMESPACE_END }
#define COROUTINE_NAMESPACE_USE using namespace coroutine;

#ifndef STACK_DEFAULT
#define STACK_DEFAULT 1024
#endif // STACK_DEFAULT

#include <assert.h>
#include <vector>
#include <list>
#include <thread>
#include <future>

#endif