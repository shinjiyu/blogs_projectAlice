#include "stdafx.h"
#include "MsgWindow.h"
#define CLOSE_WINDOW_TIMEOUT 5000

#ifdef TestMsgWnd
#include <iostream>
using namespace std;
#define Test(m) m
#else
#define Test(m) 
#endif



DWORD WINAPI ThreadMethod(LPVOID lpParameter)//ִ���߳�����ĺ��� 
{
	MsgWnd* pWnd = (MsgWnd*)lpParameter;

	HWND hwnd = pWnd->Create(NULL, NULL, _T("Test_Shinjiyu"), WS_POPUP);
	DWORD dwResult = GetLastError();
	
	MSG msg;
	while (::GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return 0;
}


MsgWnd::MsgWnd()
{
}

MsgWnd::~MsgWnd()
{
	while (!Empty())
	{
		FreeItem(PopWithValue());
	}
}

bool MsgWnd::Run()
{
	DWORD dwThreadID = 0;//�����߳�ID 

	m_thread = CreateThread(0, 0, ThreadMethod, (LPVOID)this, 0, &dwThreadID);//�����߳� 

	return m_thread != NULL;
}

bool MsgWnd::Stop()
{
	if (m_thread != NULL)
	{
		
		BOOL B = PostMessage(WM_CLOSE,0,0);
		DWORD dwResult = WaitForSingleObject(m_thread, CLOSE_WINDOW_TIMEOUT);
		if (dwResult == WAIT_TIMEOUT)
		{
			CloseHandle(m_thread);
		}
		m_thread = NULL;
	}
	return TRUE;
}

void MsgWnd::FreeItem(COPYDATASTRUCT& st)
{
	delete st.lpData;
	st.lpData = NULL;
	st.cbData = 0;
}


LRESULT MsgWnd::OnMsg(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	
	return 0;
}

LRESULT MsgWnd::OnClose(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DestroyWindow();
	PostQuitMessage(0);
	return 0;
}

LRESULT MsgWnd::OnCopyData(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	COPYDATASTRUCT* p = (COPYDATASTRUCT*)lParam;
	if (!p) return 0;
	COPYDATASTRUCT st;
	st.dwData = p->dwData;
	st.cbData = p->cbData;
	st.lpData = new char[st.cbData];
	memcpy(st.lpData, p->lpData, st.cbData);
	Push(st);
	return 0;
}
