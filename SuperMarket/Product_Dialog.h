#pragma once
#include "afxcmn.h"
#include "SqlBase.h"

// Product_Dialog �Ի���

class Product_Dialog : public CDialogEx
{
	DECLARE_DYNAMIC(Product_Dialog)

public:
	Product_Dialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Product_Dialog();
	BOOL OnInitDialog();
	void AddToGrid();

	virtual void OnFinalRelease();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RPRDUCT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	CListCtrl m_Grid;
	afx_msg void OnBnClickedButton1();
};
