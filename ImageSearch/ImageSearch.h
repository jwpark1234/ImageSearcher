
// ImageSearch.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CImageSearchApp:
// �� Ŭ������ ������ ���ؼ��� ImageSearch.cpp�� �����Ͻʽÿ�.
//

class CImageSearchApp : public CWinApp
{
public:
	CImageSearchApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CImageSearchApp theApp;