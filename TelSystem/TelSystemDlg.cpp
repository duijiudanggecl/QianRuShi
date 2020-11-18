
// TelSystemDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TelSystem.h"
#include "TelSystemDlg.h"
#include "afxdialogex.h"
#include "BaseButton.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_ONTRAY (WM_USER+10)

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTelSystemDlg �Ի���



CTelSystemDlg::CTelSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TELSYSTEM_DIALOG, pParent)

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTelSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);

	DDX_Control(pDX, IDC_EDIT1, m_Edit);
	DDX_Control(pDX, IDC_EDIT2, m_Edit2);
	DDX_Control(pDX, IDC_EDIT3, m_Edit3);
	DDX_Control(pDX, IDC_COMBO1, m_Combox);
}

BEGIN_MESSAGE_MAP(CTelSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTelSystemDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTelSystemDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTelSystemDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTelSystemDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &CTelSystemDlg::OnBnClickedButton6)
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CTelSystemDlg::OnNMClickList1)
	ON_COMMAND(ID_32773, &CTelSystemDlg::On32773)
	ON_COMMAND(ID_32776, &CTelSystemDlg::On32776)
	ON_COMMAND(ID_32777, &CTelSystemDlg::On32777)
	ON_MESSAGE(WM_ONTRAY, &CTelSystemDlg::OnOntray)
//	ON_COMMAND(ID_32778, &CTelSystemDlg::OnShow)
ON_BN_CLICKED(IDC_BUTTON5, &CTelSystemDlg::OnBnClickedButton5)
ON_BN_CLICKED(IDC_BUTTON7, &CTelSystemDlg::OnBnClickedButton7)
END_MESSAGE_MAP()


// CTelSystemDlg ��Ϣ�������

BOOL CTelSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	  m_Menu.LoadMenu(IDR_MENU1);
	 SetMenu(&m_Menu);

	  
	  // m_MenuSys.LoadMenu(IDR_MENU2);
	  // SetMenu(&m_MenuSys);
	   //SetMenu(&m_MenuSys);
	  // char lpszTip[] = "��ҵ�绰¼������ϵͳ";


		  





	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������


	m_Combox.AddString(L"1");//���daoһ��ѡ��dao
	m_Combox.AddString(L"2");//���daoһ��ѡ��dao
	m_Combox.AddString(L"3");//���daoһ��ѡ��dao
	m_Combox.AddString(L"4");//���daoһ��ѡ��dao
	m_Combox.AddString(L"5");//���daoһ��ѡ��dao
	m_Combox.AddString(L"6");//���daoһ��ѡ��dao
	m_Combox.AddString(L"7");//���daoһ��ѡ��dao
	m_Combox.AddString(L"8");//���daoһ��ѡ��dao
	m_Combox.AddString(L"9");//���daoһ��ѡ��dao
	m_Combox.SetCurSel(0);


	
	if (SqlServer.ConnectSql())
		MessageBox(L"�������ݿ�ɹ���");
	else
		MessageBox(L"�������ݿ�ʧ�ܣ�");

	m_List.InsertColumn(0, L"����"); //�������
	m_List.InsertColumn(1, L"��·");
	m_List.InsertColumn(2, L"�绰����");
	m_List.InsertColumn(3, L"��ע");

	m_List.SetColumnWidth(0, 120); //�����п�
	m_List.SetColumnWidth(1, 120);
	m_List.SetColumnWidth(2, 180);
	m_List.SetColumnWidth(3, 120);

	m_List.SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_HEADERDRAGDROP | LVS_EX_GRIDLINES);//������չ��ʽ������ѡ��������

	m_List.SetBkColor(RGB(0, 128, 128));
	m_List.SetTextBkColor(RGB(0, 128,128));
	m_List.SetTextColor(RGB(0, 25, 250));



//	m_LagerIcon.Create(32, 32, ILC_COLOR24 || ILC_MASK, 1, 0);
//	m_LagerIcon.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));
//	m_List.SetImageList(&m_LagerIcon, LVSIL_NORMAL);


	//m_LagerIcon.Create(IDI_ICON1, 32, 1, RGB(255, 0, 0));
	//m_List.SetImageList(&m_LagerIcon, LVSIL_NORMAL);//��ʾ����ͼ��

		int n = m_List.InsertItem(0, L"����");
		m_List.SetItemText(n, 1, L"����");
		m_List.SetItemText(n, 2, L"10086");
		m_List.SetItemText(n, 3, L"��");


		n = m_List.InsertItem(0, L"����");
		m_List.SetItemText(n, 1, L"����");
		m_List.SetItemText(n, 2, L"10010");
		m_List.SetItemText(n, 3, L"��");



		

		CString strPath;
		int nPos;
		GetModuleFileName(NULL, strPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
		strPath.ReleaseBuffer();
		nPos = strPath.ReverseFind(_T('\\'));
		strPath = strPath.Left(nPos);
		m_Edit2.SetWindowText(strPath);   //��ǰ·����һ��

	


		SYSTEMTIME time;
		CString NowTime;
		GetLocalTime(&time);  //Windows API ������������ȡ���صĵ�ǰϵͳ���ں�ʱ�䡣
		NowTime.Format(L"%d-%d-%d %2d:%2d:%2d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
		//str2.Format(L"%2d:%2d:%2d", time.wHour, time.wMinute, time.wSecond)


		m_Edit3.SetWindowText(NowTime);

/*
	CString strWarn;
	int nItem = 1;
	strWarn = m_List.GetItemText(nItem, 0);
	m_List.SetItemText(2, 0, strWarn);
	*/
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTelSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTelSystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTelSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//���
void CTelSystemDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int iCount = m_List.GetItemCount();//��ȡ�б����ж�����
	CString strText;
	strText.Format(_T("�� %d ��,�� 1 ��"), iCount + 1);
	m_List.InsertItem(iCount, strText); //��һ�е���Ҫ�ò���
	strText.Format(_T("�� %d ��,�� 2 ��"), iCount + 1);
	//�ڶ����Ժ��Ҫ������Item�ı�,��һ����������,�ڶ�������,�������ǲ�����ַ���
	m_List.SetItemText(iCount, 1, strText);
}


void CTelSystemDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int iCount = m_List.GetItemCount();
	//���Զ�ѡȻ��ɾ��
	for (int i = iCount; i >= 0; i--)
	{
		if (m_List.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			m_List.DeleteItem(i);//ѡ���˾�ɾ��
		}
	}

}


void CTelSystemDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_List.ModifyStyle(LVS_TYPEMASK, LVS_SMALLICON);


}


void CTelSystemDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//MessageBox(L"ok");
}


void CTelSystemDlg::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int m_index = 0;
	m_index = m_Combox.GetCurSel();

	CString exesql;

	exesql.Format(_T("SELECT * from Table%d"), (m_index+1));


	if(SqlServer.ExecuteSql(exesql))
		MessageBox(L"ok");
	else
		MessageBox(L"fail");
}


void CTelSystemDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//CString TelNumber = m_List.GetItemText(n, 2);
	//m_Edit.SetWindowText(TelNumber);



	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void CTelSystemDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString str;
	int nId;
	//���ȵõ������λ��
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox(L"������ѡ��һ��");
		return;
	}
	//�õ��кţ�ͨ��POSITIONת��
	nId = (int)m_List.GetNextSelectedItem(pos);
	//str.Format("%d", nId);
	//MessageBox("��ѡ�е�" + str + "��");
	//�õ����е����ݣ�0��ʾ��һ�У�ͬ��1,2,3...��ʾ�ڶ���������...�У�

	str = m_List.GetItemText(nId, 2);
	m_Edit.SetWindowText(str);


	*pResult = 0;
}


void CTelSystemDlg::On32773()
{
	// TODO: �ڴ���������������
	PostQuitMessage(0);
	
}


void CTelSystemDlg::On32776()   //���ҹ���
{
	// TODO: �ڴ���������������

	find_dialog m_find_dialog;

	m_find_dialog.DoModal();

}


void CTelSystemDlg::On32777()
{
	// TODO: �ڴ���������������


	InfoDialog info_dialog;
	info_dialog.DoModal();

}


afx_msg LRESULT CTelSystemDlg::OnOntray(WPARAM wParam, LPARAM lParam)
{

	
	if (wParam != IDR_MAINFRAME)
		return   1;



	switch (lParam)
	{
	case WM_RBUTTONUP://�Ҽ�����ʱ������ݲ˵�������ֻ��һ�����رա�
	{
		LPPOINT   lpoint = new   tagPOINT;
		::GetCursorPos(lpoint);//�õ����λ��
		CMenu   menu;
		menu.CreatePopupMenu();//����һ������ʽ�˵�
							   //���Ӳ˵���رա������������ϢWM_DESTROY�������ڣ���
							   //���أ��������������
		menu.AppendMenu(MF_STRING, WM_DESTROY, L"�ر� ");
		//ȷ������ʽ�˵���λ��
		menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);
		//��Դ����
		HMENU   hmenu = menu.Detach();
		menu.DestroyMenu();
		delete   lpoint;
	}
	break;
	case WM_LBUTTONDBLCLK://˫������Ĵ���
	{
		this->ShowWindow(SW_SHOW);//�򵥵���ʾ���������¶�
	}
	break;
	}
	return   0;

}



void CTelSystemDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	LPCWSTR  lpszTip = L"��ҵ�绰¼������ϵͳ";
	NOTIFYICONDATA data;
	data.cbSize = sizeof(NOTIFYICONDATA);
	data.hWnd = m_hWnd;
	lstrcpyn(data.szTip, lpszTip, sizeof(lpszTip));
	data.uCallbackMessage = WM_ONTRAY;
	data.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	data.hIcon = m_hIcon;
	data.uID = IDR_MAINFRAME;
	Shell_NotifyIcon(NIM_ADD, &data);
	ShowWindow(SW_HIDE);



}


void CTelSystemDlg::OnBnClickedButton7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL  ret = DeleteFile(L"D:\\AAA.wmv");
	if (ret)
		MessageBox(L"SUCCESS !");
	else
		MessageBox(L"FAIL !");



}
