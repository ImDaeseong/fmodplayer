#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"	

class CfmodplayerApp : public CWinAppEx
{
protected:
	ULONG_PTR	m_gdiplusToken;

public:
	CfmodplayerApp();

	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual int ExitInstance();
	DECLARE_MESSAGE_MAP()
};

extern CfmodplayerApp theApp;