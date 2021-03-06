#pragma once

#include <stdio.h>
#include <tchar.h>
#include <atlbase.h>
#include <atlwin.h>
#include <windows.h>
#include "MsgQueue.h"

class MsgWnd :public CWindowImpl < MsgWnd >, public MsgQueue<COPYDATASTRUCT>
{
public:
	MsgWnd();
	~MsgWnd();
	DECLARE_WND_CLASS(_T("MsgWnd"));
	BEGIN_MSG_MAP(MsgWnd)
		MESSAGE_HANDLER(WM_CLOSE, OnClose);
		MESSAGE_HANDLER(WM_COPYDATA, OnCopyData);
		MESSAGE_RANGE_HANDLER(WM_USER, WM_APP, OnMsg);
	END_MSG_MAP()

	bool Run(const TCHAR* szName);
	bool Stop();

	void FreeItem(COPYDATASTRUCT& st);

	HANDLE GetCreateWndOKEvent(){ return m_CreateWndOK; };
	const TCHAR* GetWindowName() {
		return m_WindowName.c_str();
	}
private:
	LRESULT OnMsg(UINT nMsg, WPARAM wParam,
		LPARAM lParam, BOOL& bHandled);

	LRESULT OnClose(UINT nMsg, WPARAM wParam,
		LPARAM lParam, BOOL& bHandled);

	LRESULT OnCopyData(UINT nMsg, WPARAM wParam,
		LPARAM lParam, BOOL& bHandled);
	HANDLE m_thread;
	HANDLE m_CreateWndOK;

	std::wstring m_WindowName;
};


