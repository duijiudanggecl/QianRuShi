
// VideoRecoder.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVideoRecoderApp: 
// �йش����ʵ�֣������ VideoRecoder.cpp
//

class CVideoRecoderApp : public CWinApp
{
public:
	CVideoRecoderApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVideoRecoderApp theApp;