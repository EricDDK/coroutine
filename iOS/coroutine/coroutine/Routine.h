#ifndef _ROUTINE_H__
#define _ROUTINE_H__

#include "Common.h"

#ifdef _MSC_VER
#include <Windows.h>
#else
#if defined(__APPLE__) && defined(__MACH__)
#define _XOPEN_SOURCE
#include <ucontext.h>
#else
#include <ucontext.h>
#endif
#endif

COROUTINE_NAMESPACE_START

class Routine
{
public:
	Routine(std::function<void()> f)
    :_func(f)
    ,_stack(nullptr)
    ,_status(kCoroutineStatus::Ready)
	{
		
	}

	~Routine()
	{
        delete[] _stack;
	}

	std::function<void()> _func;
    char *_stack;
    kCoroutineStatus _status;
    ucontext_t _context;
};

COROUTINE_NAMESPACE_END

#endif
