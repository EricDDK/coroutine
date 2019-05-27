#ifndef _COROUTINE_H__
#define _COROUTINE_H__

#include "Common.h"
#include "Routine.h"
#include "Ordinator.h"

COROUTINE_NAMESPACE_START

#ifdef _MSC_VER

#else

#endif

#ifdef _MSC_VER
#ifndef COROUTINE_CALL
#define COROUTINE_CALL __stdcall
#endif
#else
#ifndef COROUTINE_CALL
#define COROUTINE_CALL 
#endif
#endif

thread_local static Ordinator _ordinator;

size_t create(std::function<void()> func)
{
    Routine *routine = new Routine(func);
    if (_ordinator._indexes.empty())
    {
        _ordinator._routines.push_back(routine);
        return _ordinator._routines.size() - 1;
    }
    else
    {
        size_t id = _ordinator._indexes.front();
        _ordinator._indexes.pop();
        _ordinator._routines[id] = routine;
        return id;
    }
}

void destroy(size_t id)
{
    Routine *routine = _ordinator._routines[id];
    assert(routine != nullptr);
    delete routine;
    _ordinator._routines[id] = nullptr;
}

#ifdef _MSC_VER
#ifndef COROUTINE_ENTRY_PARAMS
#define COROUTINE_ENTRY_PARAMS LPVOID lpParameter
#endif
#else
#ifndef COROUTINE_ENTRY_PARAMS
#define COROUTINE_ENTRY_PARAMS
#endif
#endif

inline void COROUTINE_CALL entry(COROUTINE_ENTRY_PARAMS)
{
	size_t id = _ordinator._current;
	Routine *routine = _ordinator._routines[id];
	routine->_func();
	_ordinator._current = 0;
	routine->_status = kCoroutineStatus::Dead;
#ifdef _MSC_VER
	SwitchToFiber(_ordinator._main);
#else
	_ordinator._indexes.push(id);
#endif
}

int resume(size_t id)
{
    Routine *routine = _ordinator._routines[id];
    if (routine == nullptr)
        return -1;
    switch (routine->_status) {
        case kCoroutineStatus::Dead:
            return -1;
        case kCoroutineStatus::Ready:
#ifdef _MSC_VER
			routine->_fiber = CreateFiber(_ordinator._stackSize, entry, 0);
			_ordinator._current = id;
			SwitchToFiber(routine->_fiber);
#else
			getcontext(&routine->_context);
			routine->_stack = new char[_ordinator._stackSize];
			routine->_context.uc_stack.ss_sp = routine->_stack;
			routine->_context.uc_stack.ss_size = _ordinator._stackSize;
			routine->_context.uc_link = &_ordinator._main;
			_ordinator._current = id;
			makecontext(&routine->_context, reinterpret_cast<void(*)(void)>(entry), 0);
			swapcontext(&_ordinator._main, &routine->_context);
#endif      
            break;
        case kCoroutineStatus::Suspend:
#ifdef _MSC_VER
			_ordinator._current = id;
			SwitchToFiber(routine->_fiber);
#else
			_ordinator._current = id;
			swapcontext(&_ordinator._main, &routine->_context);
#endif
            break;
        default:
            assert(0);
            break;
    }
    return 0;
}

void yield()
{
    size_t id = _ordinator._current;
    Routine *routine = _ordinator._routines[id];
    assert(routine != nullptr);
#ifdef _MSC_VER
	_ordinator._current = 0;
	SwitchToFiber(_ordinator._main);
#else
	char *stackTop = routine->_stack + _ordinator._stackSize;
	char stackBottom = 0;
	assert(size_t(stackTop - &stackBottom) <= _ordinator._stackSize);
	_ordinator._current = 0;
	swapcontext(&routine->_context, &_ordinator._main);
#endif
}

COROUTINE_NAMESPACE_END

#endif
