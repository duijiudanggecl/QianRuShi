
// SuperMarketDlg.h : 头文件
//

#pragma once
#include "SqlBase.h"
#include "Product_Dialog.h"

// CSuperMarketDlg 对话框
class CSuperMarketDlg : public CDialogEx
{
// 构造
public:
	CSuperMarketDlg(CWnd* pParent = NULL);	// 标准构造函数
	CBrush   m_brush;
	CFont m_font;
	CToolBar m_ToolBar;
	CImageList m_ImageList;
	CMenu m_Menu;
	SqlBase m_Sql;
	//_RecordsetPtr m_LandRecordset;
	//_RecordsetPtr m_pRecordset;

	virtual BOOL CSuperMarketDlg::PreTranslateMessage(MSG* pMsg);
	void CSuperMarketDlg::OnOK();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUPERMARKET_DIALOG };
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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton1050();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
