
// SuperMarketDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SuperMarket.h"
#include "SuperMarketDlg.h"
#include "afxdialogex.h"
#include "Main_Dialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CSuperMarketDlg �Ի���



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


// CSuperMarketDlg ��Ϣ�������

BOOL CSuperMarketDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_Sql.SqlInit();

	m_Sql.ConnectSql();   //�������ݿ�

//	CFont font;
//	font.CreatePointFont(2, _T("����"));
//	GetDlgItem(IDC_STATIC)->SetFont(&font);
//	SetDlgItemTextW(IDC_STATIC, L"Account");


//	CWnd *cWnd = GetDlgItem(IDC_STATIC);
//	cWnd->SetFont(&font);
//	cWnd->SetWindowTextW(L"Account");
	m_font.CreatePointFont(150, _T("����"));//����15�����壬�����п�

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
	//LPCTSTR str = L"��Ʒ��Ϣ";

	 m_ToolBar.SetButtonText(0, L"��Ʒ��Ϣ");
     m_ToolBar.SetButtonText(1,L"���۲�ѯ");
	 m_ToolBar.SetButtonText(3, L"���Ǽ�");
	 m_ToolBar.SetButtonText(4, L"�����Ϣ");
	 m_ToolBar.SetButtonText(5, L"�ս��ѯ");
	 m_ToolBar.SetButtonText(7, L"�û�����");
	

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSuperMarketDlg::OnPaint()
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
		CRect   rect;
		CPaintDC   dc(this);
		GetClientRect(rect);
		//dc.FillSolidRect(rect, RGB(0, 128, 128));     //����Ϊ��ɫ����    
		//CDialog::OnPaint();

		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSuperMarketDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSuperMarketDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
//	m_Sql.ConnectSql();
	m_Sql.m_pRecordset =m_Sql.GetRecordSet( L"SELECT * FROM   ProductInformation");

	CString str1, str2;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str1);
	GetDlgItem(IDC_EDIT2)->GetWindowText(str2);

	CString  Str1, Str2;
	_variant_t RecordsAffected, Name, Password, TempName, TempPW;


	USES_CONVERSION;
	//���ú�����T2A��W2A��֧��ATL��MFC�е��ַ�ת��
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
	ShowWindow(SW_HIDE); //�������Ի���
	Main_Dialog  m_MainDialog;
	m_MainDialog.DoModal();

	//m_Sql.CloseSql(); 
	}
	else
		AfxMessageBox(L"������˻�������");
}


HBRUSH CSuperMarketDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (nCtlColor == CTLCOLOR_DLG)      //�Ի�����ɫ  
		return   m_brush;       //������ɫˢ��    
	if (nCtlColor == CTLCOLOR_STATIC && pWnd->GetDlgCtrlID() == IDC_STATIC)  //��̬�ı���ɫ������ָ���ؼ�ID��  
	{
		

		pDC->SetTextColor(RGB(255, 0, 0));
		//pDC->SetBkColor(RGB(0, 128, 128));     //��̬�ؼ��ı�����ɫ���ã����������ڱ�����ɫһ�£������������͸��  
		pDC->SetBkMode(TRANSPARENT); 										 //pDC->SetBkMode(TRANSPARENT);    //ģʽ����͸���Ļ�������Ծ�̬�ؼ��ı�����ɫ���ã���Ի�����ɫ�ں�  

	pDC->SelectObject(&m_font);//����Ϊ15������
		hbr = (HBRUSH)m_brush;
	}
	if (nCtlColor == CTLCOLOR_EDIT)    //�ı��༭����ɫ  
	{
		pDC->SetTextColor(RGB(0, 0, 255));
		//pDC->SetBkColor(RGB(191,219,255));     //��̬�ؼ��ı�����ɫ����  
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)m_brush;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CSuperMarketDlg::OnOK()
{}




BOOL CSuperMarketDlg::PreTranslateMessage(MSG* pMsg)
{
	/*
	if (pMsg->message == WM_LBUTTONDOWN)  //���������������Ϣ
	{
		//for (int i = 0; i < 8; i++)  //�������а�ť
		{
			 if (pMsg->hwnd == m_ToolBar.GetDlgItem(1001)->m_hWnd)
			{
				CString str;
				str.Format( 1);
				AfxMessageBox(_T("��") + str + _T("��ť�����£�"));  //��ʾ
				return TRUE;
			}
		}
	}
	*/
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) 
	{

		//������Ӧ�س�����Ķ���
		OnBnClickedButton1();
	}
	return __super::PreTranslateMessage(pMsg);
}


void CSuperMarketDlg::OnBnClickedButton1050()
{
	Product_Dialog  m_Product;
	m_Product.DoModal();
}
