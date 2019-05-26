#ifndef _CHANNEL_H__
#define _CHANNEL_H__

#include "Common.h"
#include "Coroutine.h"

template<typename T>
class Channel
{
public:
	Channel() :_taker(0) {}
	Channel(size_t t) :_taker(t) {}
	~Channel() {}

	inline void consumer(size_t id)
	{
		_taker = id;
	}

	inline void push(const T &obj)
	{
		_list.push_back(obj);
		if (_taker && _taker != current())
			Coroutine::resume(_taker);
	}

	inline void push(T &&obj)
	{
		_list.push_back(std::move(obj));
		if (_taker && _taker != current())
			Coroutine::resume(_taker);
	}

	inline T pop()
	{
		if (!_taker)
			_taker = current();

		while (_list.empty())
			Coroutine::yield();

		T obj = std::move(_list.front());
		_list.pop_front();
		return std::move(obj);
	}

	inline void clear()
	{
		_list.clear();
	}

	inline void touch()
	{
		if (_taker && _taker != current())
			Coroutine::resume(_taker);
	}

	inline size_t size()
	{
		return _list.size();
	}

	inline bool empty()
	{
		return _list.empty();
	}

private:
	std::list<T> _list;
	size_t _taker;

};

#endif