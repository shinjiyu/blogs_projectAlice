#pragma once
#include <windows.h>
#ifdef TestLocker
#include <iostream>
using namespace std;
#define Test(m) m
#else
#define Test(m) 
#endif

class Locker
{
public:
	Locker()
	{
		m_mutex = CreateMutex(NULL,FALSE,NULL);
		Test(cout << "create" << endl);
		m_autoClose = true;
	}

	Locker(HANDLE mutex) :m_mutex(mutex), m_autoClose(false){}

	Locker(const Locker& l) :m_mutex(l.m_mutex), m_autoClose(false){}

	
	virtual ~Locker()
	{
		if (m_autoClose)
		{
			CloseHandle(m_mutex);
			Test(cout << "close" << endl);
		}
	}
	void Lock()
	{
		WaitForSingleObject(m_mutex, INFINITE);
	}

	void Unlock()
	{
		ReleaseMutex(m_mutex);
	}

private:
	HANDLE m_mutex;
	bool m_autoClose;
};

class AutoLocker:public Locker
{
public:
	AutoLocker(const Locker& l) :Locker(l)
	{
		Lock();
	}
	~AutoLocker()
	{
		Unlock();
	}
};