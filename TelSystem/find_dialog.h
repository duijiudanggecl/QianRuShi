#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// find_dialog �Ի���

class find_dialog : public CDialogEx
{
	DECLARE_DYNAMIC(find_dialog)

public:
	find_dialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~find_dialog();


	BOOL find_dialog::OnInitDialog();
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FIND_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	CEdit m_Edit;
	afx_msg void OnBnClickedPlay();
	CListCtrl m_List;
};
