#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// find_dialog 对话框

class find_dialog : public CDialogEx
{
	DECLARE_DYNAMIC(find_dialog)

public:
	find_dialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~find_dialog();


	BOOL find_dialog::OnInitDialog();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FIND_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	CEdit m_Edit;
	afx_msg void OnBnClickedPlay();
	CListCtrl m_List;
};
