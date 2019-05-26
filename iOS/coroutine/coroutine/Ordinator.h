#ifndef _ORDINATOR_H__
#define _ORDINATOR_H__

#include "Common.h"
#include "Routine.h"

COROUTINE_NAMESPACE_START

class Ordinator
{
public:
	Ordinator(size_t stackSize = STACK_DEFAULT)
		:_current(-1)
		,_stackSize(stackSize)
	{

	}

	~Ordinator()
	{
		for (auto &r : _routines)
			delete r;
		_routines.clear();
	}

	std::vector<coroutine::Routine*> _routines;
	std::queue<size_t> _indexes;
	size_t _current;
	size_t _stackSize;
    ucontext_t _main;
};

COROUTINE_NAMESPACE_END



#endif
