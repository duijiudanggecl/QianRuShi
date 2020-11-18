
// SuperMarketDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SuperMarket.h"
#include "SuperMarketDlg.h"
#include "afxdialogex.h"
#include "Main_Dialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CSuperMarketDlg 对话框



CSuperMarketDlg::CSuperMarketDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SUPERMARKET_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSuperMarketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSuperMarketDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(IDC_BUTTON1, &CSuperMarketDlg::OnBnClickedButton1)
	ON_BN_CLICKED(1050, &CSuperMarketDlg::OnBnClickedButton1050)

	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CSuperMarketDlg 消息处理程序

BOOL CSuperMarketDlg::OnInitDialog()
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_Sql.SqlInit();

	m_Sql.ConnectSql();   //连接数据库

//	CFont font;
//	font.CreatePointFont(2, _T("宋体"));
//	GetDlgItem(IDC_STATIC)->SetFont(&font);
//	SetDlgItemTextW(IDC_STATIC, L"Account");


//	CWnd *cWnd = GetDlgItem(IDC_STATIC);
//	cWnd->SetFont(&font);
//	cWnd->SetWindowTextW(L"Account");
	m_font.CreatePointFont(150, _T("宋体"));//代表15号字体，华文行楷

//	CToolBar m_ToolBar;
//	CImageList m_ImageList;
	m_ImageList.Create(32, 32, ILC_COLOR24 | ILC_MASK, 1, 1);
	m_ImageList.Add(AfxGetApp()->LoadIconW(IDI_ICON1));
	m_ImageList.Add(AfxGetApp()->LoadIconW(IDR_MAINFRAME));
	m_ImageList.Add(AfxGetApp()->LoadIconW(IDI_ICON1));
	m_ImageList.Add(AfxGetApp()->LoadIconW(IDR_MAINFRAME));
	m_ImageList.Add(AfxGetApp()->LoadIconW(IDI_ICON1));
	m_ImageList.Add(AfxGetApp()->LoadIconW(IDR_MAINFRAME));
	UINT array[8];
	
	for (int i = 0; i < 8; i++)
		array[i] = i + 1050;
	array[2] = ID_SEPARATOR;
	array[6] = ID_SEPARATOR;

	m_ToolBar.Create(this);
	m_ToolBar.SetButtons(array, 8);
	m_ToolBar.GetToolBarCtrl().SetImageList(&m_ImageList);
	m_ToolBar.SetSizes(CSize(50,60), CSize(32,32));
	//LPCTSTR str = L"商品信息";

	 m_ToolBar.SetButtonText(0, L"商品信息");
     m_ToolBar.SetButtonText(1,L"销售查询");
	 m_ToolBar.SetButtonText(3, L"入库登记");
	 m_ToolBar.SetButtonText(4, L"库存信息");
	 m_ToolBar.SetButtonText(5, L"日结查询");
	 m_ToolBar.SetButtonText(7, L"用户管理");
	

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSuperMarketDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSuperMarketDlg::OnPaint()
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
		CRect   rect;
		CPaintDC   dc(this);
		GetClientRect(rect);
		//dc.FillSolidRect(rect, RGB(0, 128, 128));     //设置为绿色背景    
		//CDialog::OnPaint();

		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSuperMarketDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSuperMarketDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	
//	m_Sql.ConnectSql();
	m_Sql.m_pRecordset =m_Sql.GetRecordSet( L"SELECT * FROM   ProductInformation");

	CString str1, str2;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str1);
	GetDlgItem(IDC_EDIT2)->GetWindowText(str2);

	CString  Str1, Str2;
	_variant_t RecordsAffected, Name, Password, TempName, TempPW;


	USES_CONVERSION;
	//调用函数，T2A和W2A均支持ATL和MFC中的字符转换
	TempName = T2A(str1);
	TempPW = T2A(str2);


	BOOL m_Land = FALSE;;
	while(!m_Sql.m_pRecordset->adoEOF)

	{
		Name = m_Sql.m_pRecordset->GetCollect(L"NAME");
		Password = m_Sql.m_pRecordset->GetCollect(L"PASSWORD");

		if ((Name == TempName) && (Password == TempPW))
		{ 
			m_Land = TRUE;
			//m_Sql.Release();
			//m_Sql.CloseSql();

			break;
		}
		m_Sql.m_pRecordset->MoveNext();

	}

    m_Sql.Release();
	m_Sql.CloseSql();


	if (m_Land)
	{ 
	ShowWindow(SW_HIDE); //隐藏主对话框
	Main_Dialog  m_MainDialog;
	m_MainDialog.DoModal();

	//m_Sql.CloseSql(); 
	}
	else
		AfxMessageBox(L"密码或账户名错误！");
}


HBRUSH CSuperMarketDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_DLG)      //对话框颜色  
		return   m_brush;       //返加绿色刷子    
	if (nCtlColor == CTLCOLOR_STATIC && pWnd->GetDlgCtrlID() == IDC_STATIC)  //静态文本颜色，并且指定控件ID号  
	{
		

		pDC->SetTextColor(RGB(255, 0, 0));
		//pDC->SetBkColor(RGB(0, 128, 128));     //静态控件的背景颜色设置，这里设置于背景颜色一致，则可以免设置透明  
		pDC->SetBkMode(TRANSPARENT); 										 //pDC->SetBkMode(TRANSPARENT);    //模式设置透明的话，则忽略静态控件的背景颜色设置，与对话框颜色融合  

	pDC->SelectObject(&m_font);//文字为15号字体
		hbr = (HBRUSH)m_brush;
	}
	if (nCtlColor == CTLCOLOR_EDIT)    //文本编辑框颜色  
	{
		pDC->SetTextColor(RGB(0, 0, 255));
		//pDC->SetBkColor(RGB(191,219,255));     //静态控件的背景颜色设置  
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)m_brush;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CSuperMarketDlg::OnOK()
{}




BOOL CSuperMarketDlg::PreTranslateMessage(MSG* pMsg)
{
	/*
	if (pMsg->message == WM_LBUTTONDOWN)  //如果发生鼠标左键消息
	{
		//for (int i = 0; i < 8; i++)  //遍历所有按钮
		{
			 if (pMsg->hwnd == m_ToolBar.GetDlgItem(1001)->m_hWnd)
			{
				CString str;
				str.Format( 1);
				AfxMessageBox(_T("第") + str + _T("按钮被按下！"));  //提示
				return TRUE;
			}
		}
	}
	*/
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) 
	{

		//处理响应回车键后的动作
		OnBnClickedButton1();
	}
	return __super::PreTranslateMessage(pMsg);
}


void CSuperMarketDlg::OnBnClickedButton1050()
{
	Product_Dialog  m_Product;
	m_Product.DoModal();
}
