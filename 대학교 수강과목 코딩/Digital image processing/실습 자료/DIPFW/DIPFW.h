// DIPFW.h : DIPFW ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CDIPFWApp:
// �� Ŭ������ ������ ���ؼ��� DIPFW.cpp�� �����Ͻʽÿ�.
//

class CDIPFWApp : public CWinApp
{
public:
	CDIPFWApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDIPFWApp theApp;