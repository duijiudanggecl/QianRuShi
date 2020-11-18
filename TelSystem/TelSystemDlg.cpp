
// TelSystemDlg.cpp : 实现文件
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CTelSystemDlg 对话框



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


// CTelSystemDlg 消息处理程序

BOOL CTelSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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
	  // char lpszTip[] = "企业电话录音管理系统";


		  





	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码


	m_Combox.AddString(L"1");//添加dao一个选项dao
	m_Combox.AddString(L"2");//添加dao一个选项dao
	m_Combox.AddString(L"3");//添加dao一个选项dao
	m_Combox.AddString(L"4");//添加dao一个选项dao
	m_Combox.AddString(L"5");//添加dao一个选项dao
	m_Combox.AddString(L"6");//添加dao一个选项dao
	m_Combox.AddString(L"7");//添加dao一个选项dao
	m_Combox.AddString(L"8");//添加dao一个选项dao
	m_Combox.AddString(L"9");//添加dao一个选项dao
	m_Combox.SetCurSel(0);


	
	if (SqlServer.ConnectSql())
		MessageBox(L"连接数据库成功！");
	else
		MessageBox(L"连接数据库失败！");

	m_List.InsertColumn(0, L"类型"); //添加列名
	m_List.InsertColumn(1, L"线路");
	m_List.InsertColumn(2, L"电话号码");
	m_List.InsertColumn(3, L"备注");

	m_List.SetColumnWidth(0, 120); //设置列宽
	m_List.SetColumnWidth(1, 120);
	m_List.SetColumnWidth(2, 180);
	m_List.SetColumnWidth(3, 120);

	m_List.SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_HEADERDRAGDROP | LVS_EX_GRIDLINES);//设置扩展格式，整行选择，网格线

	m_List.SetBkColor(RGB(0, 128, 128));
	m_List.SetTextBkColor(RGB(0, 128,128));
	m_List.SetTextColor(RGB(0, 25, 250));



//	m_LagerIcon.Create(32, 32, ILC_COLOR24 || ILC_MASK, 1, 0);
//	m_LagerIcon.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));
//	m_List.SetImageList(&m_LagerIcon, LVSIL_NORMAL);


	//m_LagerIcon.Create(IDI_ICON1, 32, 1, RGB(255, 0, 0));
	//m_List.SetImageList(&m_LagerIcon, LVSIL_NORMAL);//显示正常图标

		int n = m_List.InsertItem(0, L"来电");
		m_List.SetItemText(n, 1, L"外线");
		m_List.SetItemText(n, 2, L"10086");
		m_List.SetItemText(n, 3, L"无");


		n = m_List.InsertItem(0, L"来电");
		m_List.SetItemText(n, 1, L"外线");
		m_List.SetItemText(n, 2, L"10010");
		m_List.SetItemText(n, 3, L"无");



		

		CString strPath;
		int nPos;
		GetModuleFileName(NULL, strPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
		strPath.ReleaseBuffer();
		nPos = strPath.ReverseFind(_T('\\'));
		strPath = strPath.Left(nPos);
		m_Edit2.SetWindowText(strPath);   //当前路径上一级

	


		SYSTEMTIME time;
		CString NowTime;
		GetLocalTime(&time);  //Windows API 函数，用来获取当地的当前系统日期和时间。
		NowTime.Format(L"%d-%d-%d %2d:%2d:%2d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
		//str2.Format(L"%2d:%2d:%2d", time.wHour, time.wMinute, time.wSecond)


		m_Edit3.SetWindowText(NowTime);

/*
	CString strWarn;
	int nItem = 1;
	strWarn = m_List.GetItemText(nItem, 0);
	m_List.SetItemText(2, 0, strWarn);
	*/
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTelSystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTelSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//添加
void CTelSystemDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	int iCount = m_List.GetItemCount();//获取列表中有多少行
	CString strText;
	strText.Format(_T("第 %d 行,第 1 列"), iCount + 1);
	m_List.InsertItem(iCount, strText); //第一列的行要用插入
	strText.Format(_T("第 %d 行,第 2 列"), iCount + 1);
	//第二列以后就要用设置Item文本,第一个参数是行,第二个是列,第三个是插入的字符串
	m_List.SetItemText(iCount, 1, strText);
}


void CTelSystemDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	int iCount = m_List.GetItemCount();
	//可以多选然后删除
	for (int i = iCount; i >= 0; i--)
	{
		if (m_List.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			m_List.DeleteItem(i);//选中了就删除
		}
	}

}


void CTelSystemDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_List.ModifyStyle(LVS_TYPEMASK, LVS_SMALLICON);


}


void CTelSystemDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码

	//MessageBox(L"ok");
}


void CTelSystemDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码

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
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CString TelNumber = m_List.GetItemText(n, 2);
	//m_Edit.SetWindowText(TelNumber);



	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void CTelSystemDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	CString str;
	int nId;
	//首先得到点击的位置
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox(L"请至少选择一项");
		return;
	}
	//得到行号，通过POSITION转化
	nId = (int)m_List.GetNextSelectedItem(pos);
	//str.Format("%d", nId);
	//MessageBox("您选中第" + str + "行");
	//得到列中的内容（0表示第一列，同理1,2,3...表示第二，三，四...列）

	str = m_List.GetItemText(nId, 2);
	m_Edit.SetWindowText(str);


	*pResult = 0;
}


void CTelSystemDlg::On32773()
{
	// TODO: 在此添加命令处理程序代码
	PostQuitMessage(0);
	
}


void CTelSystemDlg::On32776()   //查找功能
{
	// TODO: 在此添加命令处理程序代码

	find_dialog m_find_dialog;

	m_find_dialog.DoModal();

}


void CTelSystemDlg::On32777()
{
	// TODO: 在此添加命令处理程序代码


	InfoDialog info_dialog;
	info_dialog.DoModal();

}


afx_msg LRESULT CTelSystemDlg::OnOntray(WPARAM wParam, LPARAM lParam)
{

	
	if (wParam != IDR_MAINFRAME)
		return   1;



	switch (lParam)
	{
	case WM_RBUTTONUP://右键起来时弹出快捷菜单，这里只有一个“关闭”
	{
		LPPOINT   lpoint = new   tagPOINT;
		::GetCursorPos(lpoint);//得到鼠标位置
		CMenu   menu;
		menu.CreatePopupMenu();//声明一个弹出式菜单
							   //增加菜单项“关闭”，点击则发送消息WM_DESTROY给主窗口（已
							   //隐藏），将程序结束。
		menu.AppendMenu(MF_STRING, WM_DESTROY, L"关闭 ");
		//确定弹出式菜单的位置
		menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);
		//资源回收
		HMENU   hmenu = menu.Detach();
		menu.DestroyMenu();
		delete   lpoint;
	}
	break;
	case WM_LBUTTONDBLCLK://双击左键的处理
	{
		this->ShowWindow(SW_SHOW);//简单的显示主窗口完事儿
	}
	break;
	}
	return   0;

}



void CTelSystemDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码

	LPCWSTR  lpszTip = L"企业电话录音管理系统";
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
	// TODO: 在此添加控件通知处理程序代码
	BOOL  ret = DeleteFile(L"D:\\AAA.wmv");
	if (ret)
		MessageBox(L"SUCCESS !");
	else
		MessageBox(L"FAIL !");



}
