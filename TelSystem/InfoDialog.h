#pragma once
#include "afxcmn.h"


// InfoDialog �Ի���

class InfoDialog : public CDialogEx
{
	DECLARE_DYNAMIC(InfoDialog)

public:
	InfoDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~InfoDialog();


	BOOL OnInitDialog();

	CImageList m_ImageList;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	afx_msg void OnBnClickedButton1();
};
