
// SuperMarketDlg.h : ͷ�ļ�
//

#pragma once
#include "SqlBase.h"
#include "Product_Dialog.h"

// CSuperMarketDlg �Ի���
class CSuperMarketDlg : public CDialogEx
{
// ����
public:
	CSuperMarketDlg(CWnd* pParent = NULL);	// ��׼���캯��
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

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUPERMARKET_DIALOG };
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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton1050();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
