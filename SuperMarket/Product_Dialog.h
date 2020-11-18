#pragma once
#include "afxcmn.h"
#include "SqlBase.h"

// Product_Dialog 对话框

class Product_Dialog : public CDialogEx
{
	DECLARE_DYNAMIC(Product_Dialog)

public:
	Product_Dialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Product_Dialog();
	BOOL OnInitDialog();
	void AddToGrid();

	virtual void OnFinalRelease();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RPRDUCT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	CListCtrl m_Grid;
	afx_msg void OnBnClickedButton1();
};
