#pragma once
#include "afxcmn.h"


// InfoDialog 对话框

class InfoDialog : public CDialogEx
{
	DECLARE_DYNAMIC(InfoDialog)

public:
	InfoDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~InfoDialog();


	BOOL OnInitDialog();

	CImageList m_ImageList;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	afx_msg void OnBnClickedButton1();
};
