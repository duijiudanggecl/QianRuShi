
// TelSystemDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "SqlBase.h"
#include "find_dialog.h"
#include "InfoDialog.h"

// CTelSystemDlg �Ի���
class CTelSystemDlg : public CDialogEx
{
// ����
public:
	CTelSystemDlg(CWnd* pParent = NULL);	// ��׼���캯��


	SqlBase SqlServer;


// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TELSYSTEM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	CMenu m_Menu;
	CMenu m_MenuSys;

	CImageList m_LagerIcon;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CEdit m_Edit;
	CEdit m_Edit2;
	CEdit m_Edit3;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
	CComboBox m_Combox;
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void On32773();
	afx_msg void On32776();
	afx_msg void On32777();
protected:
	afx_msg LRESULT OnOntray(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnShow();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton7();
};
