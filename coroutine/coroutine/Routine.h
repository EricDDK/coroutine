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

#ifdef _MSC_VER

COROUTINE_NAMESPACE_START

class Routine
{
	friend class Coroutine;

public:
	Routine(std::function<void()> f)
	{
		_func = f;
		_finished = false;
		_fiber = nullptr;
	}

	~Routine()
	{
		DeleteFiber(_fiber);
	}

private:
	std::function<void()> _func;
	bool _finished;
	LPVOID _fiber;

};

#else



#endif

COROUTINE_NAMESPACE_END

#endif