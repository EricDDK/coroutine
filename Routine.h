#ifndef _ROUTINE_H__
#define _ROUTINE_H__

#include "Common.h"

#ifdef _MSC_VER
#include <Windows.h>
#else
#if __APPLE__ && __MACH__
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
		, _status(kCoroutineStatus::Ready)
#ifdef _MSC_VER
		, _fiber(nullptr)
#else
		, _stack(nullptr)
#endif
	{

	}

	~Routine()
	{
#ifdef _MSC_VER
		DeleteFiber(_fiber);
#else
		delete[] _stack;
#endif
	}

	std::function<void()> _func;
	kCoroutineStatus _status;

#ifndef _MSC_VER
	char *_stack;
#endif

#ifdef _MSC_VER
	LPVOID _fiber;
#else
	ucontext_t _context;
#endif
};

COROUTINE_NAMESPACE_END

#endif
