#ifndef _COROUTINE_H__
#define _COROUTINE_H__

#include "Common.h"
#include "Routine.h"
#include "Ordinator.h"

COROUTINE_NAMESPACE_START

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

inline void entry()
{
    size_t id = _ordinator._current;
    Routine *routine = _ordinator._routines[id];
    routine->_func();
    _ordinator._current = 0;
    _ordinator._indexes.push(id);
    routine->_status = kCoroutineStatus::Dead;
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
            getcontext(&routine->_context);
            routine->_stack = new char[_ordinator._stackSize];
            routine->_context.uc_stack.ss_sp = routine->_stack;
            routine->_context.uc_stack.ss_size = _ordinator._stackSize;
            routine->_context.uc_link = &_ordinator._main;
            _ordinator._current = id;
            makecontext(&routine->_context, reinterpret_cast<void (*)(void)>(entry), 0);
            swapcontext(&_ordinator._main, &routine->_context);
            break;
        case kCoroutineStatus::Suspend:
            _ordinator._current = id;
            swapcontext(&_ordinator._main, &routine->_context);
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
    char *stackTop = routine->_stack + _ordinator._stackSize;
    char stackBottom = 0;
    assert(size_t(stackTop - &stackBottom) <= _ordinator._stackSize);
    _ordinator._current = 0;
    swapcontext(&routine->_context , &_ordinator._main);
}

COROUTINE_NAMESPACE_END

#endif
