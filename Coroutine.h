#ifndef _COROUTINE_H__
#define _COROUTINE_H__

#include "Common.h"
#include "Routine.h"
#include "Schedule.h"

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

thread_local static Schedule _schedule;

size_t create(std::function<void()> func)
{
    Routine *routine = new Routine(func);
    if (_schedule._indexes.empty())
    {
        _schedule._routines.push_back(routine);
        return _schedule._routines.size() - 1;
    }
    else
    {
        size_t id = _schedule._indexes.front();
        _schedule._indexes.pop();
        _schedule._routines[id] = routine;
        return id;
    }
}

void destroy(size_t id)
{
    Routine *routine = _schedule._routines[id];
    assert(routine != nullptr);
    delete routine;
    _schedule._routines[id] = nullptr;
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
	size_t id = _schedule._current;
	Routine *routine = _schedule._routines[id];
	routine->_func();
	_schedule._current = 0;
	routine->_status = kCoroutineStatus::Dead;
#ifdef _MSC_VER
	SwitchToFiber(_schedule._main);
#else
	_schedule._indexes.push(id);
#endif
}

int resume(size_t id)
{
    Routine *routine = _schedule._routines[id];
    if (routine == nullptr)
        return -1;
    switch (routine->_status) {
        case kCoroutineStatus::Dead:
            return -1;
        case kCoroutineStatus::Ready:
#ifdef _MSC_VER
			routine->_fiber = CreateFiber(_schedule._stackSize, entry, 0);
			_schedule._current = id;
			SwitchToFiber(routine->_fiber);
#else
			getcontext(&routine->_context);
			routine->_stack = new char[_schedule._stackSize];
			routine->_context.uc_stack.ss_sp = routine->_stack;
			routine->_context.uc_stack.ss_size = _schedule._stackSize;
			routine->_context.uc_link = &_schedule._main;
			_schedule._current = id;
			makecontext(&routine->_context, reinterpret_cast<void(*)(void)>(entry), 0);
			swapcontext(&_schedule._main, &routine->_context);
#endif      
            break;
        case kCoroutineStatus::Suspend:
#ifdef _MSC_VER
			_schedule._current = id;
			SwitchToFiber(routine->_fiber);
#else
			_schedule._current = id;
			swapcontext(&_schedule._main, &routine->_context);
#endif
            break;
        default:
            //assert(0);
            break;
    }
    return 0;
}

void yield()
{
    size_t id = _schedule._current;
    Routine *routine = _schedule._routines[id];
    assert(routine != nullptr);
	routine->_status = kCoroutineStatus::Suspend;
#ifdef _MSC_VER
	_schedule._current = 0;
	SwitchToFiber(_schedule._main);
#else
	char *stackTop = routine->_stack + _schedule._stackSize;
	char stackBottom = 0;
	assert(size_t(stackTop - &stackBottom) <= _schedule._stackSize);
	_schedule._current = 0;
	swapcontext(&routine->_context, &_schedule._main);
#endif
}

kCoroutineStatus getStatus(size_t id)
{
	Routine *routine = _schedule._routines[id];
	return routine->_status;
}

COROUTINE_NAMESPACE_END

#endif
