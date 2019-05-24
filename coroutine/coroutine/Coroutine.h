#ifndef _COROUTINE_H__
#define _COROUTINE_H__

#include "Common.h"
#include "Routine.h"
#include "Ordinator.h"

COROUTINE_NAMESPACE_START

class Coroutine
{
public:
	size_t create(std::function<void()> func)
	{
		Routine *routine = new Routine(func);
		if (ordinator._routines.empty())
		{
			ordinator._routines.push_back(routine);
			return 1;
		}
		else
		{
			size_t id = ordinator._indexes.front();
			ordinator._indexes.pop_front();
			assert(ordinator._routines[id - 1] == nullptr);
			ordinator._routines[id - 1] = routine;
			return id;
		}
	}

	void destroy(size_t id)
	{
		Routine *routine = ordinator._routines[id - 1];
		assert(routine != nullptr);
		delete routine;
		ordinator._routines[id - 1] = nullptr;
		ordinator._indexes.push_back(id);
	}

	void yield()
	{
		size_t id = ordinator._current;
		Routine *routine = ordinator._routines[id - 1];
		assert(routine != nullptr);
		ordinator._current = 0;
		SwitchToFiber(ordinator._fiber);
	}

	inline static void __stdcall entry(LPVOID lpParameter)
	{
		size_t id = ordinator._current;
		Routine *routine = ordinator._routines[id - 1];
		assert(routine != nullptr);
		routine->_func();
		routine->_finished = true;
		ordinator._current = 0;
		SwitchToFiber(ordinator._fiber);
	}

	int resume(size_t id)
	{
		assert(ordinator._current == 0);
		Routine *routine = ordinator._routines[id - 1];
		if (routine == nullptr)
			return -1;
		if (routine->_finished)
			return -2;
		if (routine->_fiber == nullptr)
		{
			routine->_fiber = CreateFiber(ordinator._stackSize, this->entry, 0);
			ordinator._current = id;
			SwitchToFiber(routine->_fiber);
		}
		else
		{
			ordinator._current = id;
			SwitchToFiber(routine->_fiber);
		}
		return 0;
	}

private:
	thread_local static Ordinator ordinator;

};

COROUTINE_NAMESPACE_END

#endif
