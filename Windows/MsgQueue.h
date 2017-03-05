#pragma once
#include "AtomLocker.h"
#include <Queue>
using namespace std;

template<class T>
class MsgQueue
{
public:
	MsgQueue(){};
	virtual ~MsgQueue(){
		AutoLocker _(m_locker);
		if (m_queue.size() > 0)
		{
			//waring T may be  a pointer ,so this  my cause a memory leak 
		}
		while (!m_queue.empty())
		{
			m_queue.pop();
		}
	}

	bool Empty()
	{
		return m_queue.empty();
	}

	void Push(const T& t)
	{
		AutoLocker _(m_locker);
		m_queue.push(t);
	}

	T Front()
	{
		AutoLocker _(m_locker);
		return m_queue.front();
	}
	void Pop()
	{
		AutoLocker _(m_locker);
		m_queue.pop();
	}

	T PopWithValue()
	{
		AutoLocker _(m_locker);
		T t = Front();
		Pop();
		return t;
	}

private:
	std::queue<T>  m_queue;
	Locker m_locker;
};