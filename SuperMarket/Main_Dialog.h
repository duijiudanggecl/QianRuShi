#pragma once


// Main_Dialog �Ի���

class Main_Dialog : public CDialogEx
{
	DECLARE_DYNAMIC(Main_Dialog)

public:
	Main_Dialog(CWnd* pParent = NULL);   // ��׼���캯��

	CBrush   m_brush;
	CFont    m_font;

	virtual ~Main_Dialog();

	virtual void OnFinalRelease();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
