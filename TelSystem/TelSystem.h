
// TelSystem.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTelSystemApp: 
// �йش����ʵ�֣������ TelSystem.cpp
//

class CTelSystemApp : public CWinApp
{
public:
	CTelSystemApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	afx_msg void On32773();
};

extern CTelSystemApp theApp;