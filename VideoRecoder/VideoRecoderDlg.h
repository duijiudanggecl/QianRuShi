
// VideoRecoderDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#define WM_ONTRAY (WM_USER+10)

// CVideoRecoderDlg 对话框
class CVideoRecoderDlg : public CDialogEx
{
// 构造
public:
	CVideoRecoderDlg(CWnd* pParent = NULL);	// 标准构造函数
	void Create(int state);
	void SetFileName();

	BOOL m_Once;
	HICON m_icon1;
	HICON m_icon2;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIDEORECODER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
