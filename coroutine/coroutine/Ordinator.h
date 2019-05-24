#ifndef _ORDINATOR_H__
#define _ORDINATOR_H__

#include "Common.h"
#include "Routine.h"

COROUTINE_NAMESPACE_START

class Ordinator
{
	friend class Coroutine;
	
public:
	Ordinator(size_t stackSize = STACK_DEFAULT)
		:_current(0)
		,_stackSize(stackSize)
		,_fiber(ConvertThreadToFiber(nullptr))
	{

	}

	~Ordinator()
	{
		for (auto &r : _routines)
			delete r;
		_routines.clear();
	}

private:
	std::vector<coroutine::Routine*> _routines;
	std::list<size_t> _indexes;
	size_t _current;
	size_t _stackSize;
	LPVOID _fiber;

};

COROUTINE_NAMESPACE_END



#endif