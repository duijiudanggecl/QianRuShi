#pragma once
#include "afxwin.h"
class BaseButton :
	public CButton
{
public:
	BaseButton();
	~BaseButton();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedButton3();
	afx_msg void On32777();
};

