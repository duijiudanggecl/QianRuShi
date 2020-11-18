
// VideoRecoderDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#define WM_ONTRAY (WM_USER+10)

// CVideoRecoderDlg �Ի���
class CVideoRecoderDlg : public CDialogEx
{
// ����
public:
	CVideoRecoderDlg(CWnd* pParent = NULL);	// ��׼���캯��
	void Create(int state);
	void SetFileName();

	BOOL m_Once;
	HICON m_icon1;
	HICON m_icon2;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIDEORECODER_DIALOG };
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
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	CEdit m_Path;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CListCtrl m_list;
protected:
	afx_msg LRESULT OnOntray(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton6();
};
