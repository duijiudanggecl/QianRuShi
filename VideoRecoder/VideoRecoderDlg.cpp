
// VideoRecoderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoRecoder.h"
#include "VideoRecoderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include<math.h>//数学函数库的头文件

#include<Vfw.h>//增加AVI视频处理函数的头文件

#pragma comment(lib,"Vfw32.lib")//连接库Vfw32.lib

#pragma comment(lib,"Winmm.lib")//链接库

//定义AVI视频处理函数所需的结构体变量

AVISTREAMINFO strhdr;

PAVIFILE pfile;

PAVISTREAM ps;

PAVISTREAM pComStream;

AVICOMPRESSOPTIONS pCompressOption;

AVICOMPRESSOPTIONS FAR * opts[1] = { &pCompressOption };

HRESULT hr;

int nFrames = 0; //定义视频中帧的个数

BOOL m_timer = TRUE;//定时器运行状态标志

UINT timer_num = 0;//标记定时器的名称

BOOL m_IsPause = FALSE;//是否暂停标志



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


// CVideoRecoderDlg 对话框



CVideoRecoderDlg::CVideoRecoderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VIDEORECODER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideoRecoderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Path);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CVideoRecoderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HOTKEY()
	ON_BN_CLICKED(IDC_BUTTON1, &CVideoRecoderDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CVideoRecoderDlg::OnBnClickedButton2)
	ON_MESSAGE(WM_ONTRAY, &CVideoRecoderDlg::OnOntray)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON3, &CVideoRecoderDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CVideoRecoderDlg::OnBnClickedButton4)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON5, &CVideoRecoderDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON7, &CVideoRecoderDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON6, &CVideoRecoderDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CVideoRecoderDlg 消息处理程序

BOOL CVideoRecoderDlg::OnInitDialog()
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	::RegisterHotKey(m_hWnd, 199, MOD_ALT, 'U');//注意要大写，注册的热键id为199（自己定义），Alt+U

	::RegisterHotKey(m_hWnd, 200, NULL, 'Q');//注意要大写，注册的热键id为199（自己定义），Alt+U

	::RegisterHotKey(m_hWnd, 201, NULL, 'W');//注意要大写，注册的热键id为199（自己定义），Alt+U

	m_icon1 = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_icon2 = AfxGetApp()->LoadIcon(IDI_ICON1);


	//Create(1);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}



void CVideoRecoderDlg::Create(int state)
{
	LPCWSTR  lpszTip = L"test";
	NOTIFYICONDATA data;
	data.cbSize = sizeof(NOTIFYICONDATA);
	data.hWnd = m_hWnd;
	lstrcpyn(data.szTip, lpszTip, sizeof(lpszTip));
	data.uCallbackMessage = WM_ONTRAY;
	data.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	//data.hIcon = m_hIcon;
	/*
	if (state==1)
	data.uID = IDR_MAINFRAME;
	else
		data.uID = IDI_ICON1;
		*/

	Shell_NotifyIcon(NIM_ADD, &data);

	if (state == 1)

	{
		data.hIcon = m_icon1;

	Shell_NotifyIcon(NIM_MODIFY, &data);
	}
	else

	{
		data.hIcon = m_icon2;

		Shell_NotifyIcon(NIM_MODIFY, &data);
	}

	ShowWindow(SW_HIDE);


}


void CVideoRecoderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CVideoRecoderDlg::OnPaint()
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
HCURSOR CVideoRecoderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVideoRecoderDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nHotKeyId == 199)

	{
		AfxMessageBox(_T("使用了热键"));

		return;

	}

	if (nHotKeyId == 200)

	{
		Create(1);

		return;


	}

	if (nHotKeyId == 201)

	{
		Create(2);

		return;


	}


	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}


void CVideoRecoderDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	ULARGE_INTEGER  totalsize;
	ULARGE_INTEGER  freesize;
	ULARGE_INTEGER  availablesize;
	CString  str, temp, path;

	BOOL isOpen = FALSE;		//是否打开(否则为保存)
	CString defaultDir = L"E:\\FileTest";	//默认打开的文件路径
	CString fileName = L"1.avi";			//默认打开的文件名
	CString filter = L"文件 (*.avi; *.*)|*.avi; *.*||";	//文件过虑的类型
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);


	if (openFileDlg.DoModal() == IDOK)
	{
		path = openFileDlg.GetPathName();
	}


//	m_Path.GetWindowText(path);
	str = path.Left(3);
	::GetDiskFreeSpaceEx(str, &totalsize, &freesize, &availablesize);   //获取磁盘空间信息
	temp.Format(L"%ldM", totalsize.QuadPart / 1024 / 1024);

	m_Path.SetWindowText(L"剩余空间："+ str+temp);

	//m_Path.SetWindowText(str);

}

//文件名查找
void CVideoRecoderDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path;
	m_Path.GetWindowText(path);
	CFileFind file;
	//if (path.Right(1) != "\\")
	//	path = +"\\*.txt";
	path = L"C:\\Users\\Administrator\\Desktop\\path\\*.txt";

	BOOL bf;
	bf = file.FindFile(path);  //寻找后缀为.txt的文件
	int i = 0;
	while (bf)
	{
		bf = file.FindNextFile();
	    CString fileName;
		fileName = file.GetFileName();
		m_list.InsertItem(i,fileName.Left(fileName.GetLength()-4));
		i++;
	}

	SetFileName();
//	CString str;
	//str.Format(L"%d", i);
	//m_Path.SetWindowText(str);

}

//设置文件名
void CVideoRecoderDlg::SetFileName()
{
	int count = m_list.GetItemCount();

	int flag = 0;
	CString strName;
	int i = 1;
	while (1)
	{
		strName.Format(L"录像%d", i);

		for (int j = 0; j < count; j++)
		{

			if (strName != m_list.GetItemText(j, 0))
				flag++;

		}
		if (flag == count)
		{
			goto end;
		}
		i++;
		flag = 0;
	}

    end:strName.Format(L"录像%d", i);

	m_Path.SetWindowText(strName);

}

afx_msg LRESULT CVideoRecoderDlg::OnOntray(WPARAM wParam, LPARAM lParam)
{


	//	if (wParam != IDR_MAINFRAME)
	//		return   1;



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



void CVideoRecoderDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	NOTIFYICONDATA data;
	data.cbSize = sizeof(NOTIFYICONDATA);
	data.hWnd = m_hWnd;

	data.hIcon = m_icon1;
	data.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	Shell_NotifyIcon(NIM_DELETE, &data);
	CDialogEx::OnClose();
}


void CVideoRecoderDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString oname;

	::ShellExecute(GetSafeHwnd(), TEXT("OPEN"), TEXT("D:\\BaiduNetdiskDownload\\1.wmv"), TEXT(""), NULL, SW_SHOWNORMAL);//播放音频
	//播放视频

}

/*
//把截图的文件保存到桌面
 BOOL CaptureLolToDesktop(HWND hWnd)
{
	Sleep(1 * 1000);
	LPRECT lprc = new RECT;
	GetWindowRect(hWnd, lprc);
	CString strTime;
	DWORD llocaltime = GetTickCount();
	char chDesktopPath[MAX_PATH] = { 0 };
	SHGetSpecialFolderPathA(NULL, chDesktopPath, CSIDL_DESKTOP, 0);//获取当前用户桌面路径
	strTime.Format(L"%s\\截图%ld.jpg", chDesktopPath, llocaltime);
	logs->WriteFormat("截图保存路径为%s", strTime.c_str());
	ScreenCapture(LPSTR(strTime.c_str()), 16, lprc);
	delete lprc;
	return TRUE;
}

//屏幕截图功能实现
 HBITMAP ScreenCapture(LPSTR filename, WORD bitCount, LPRECT lpRect)
{
	HBITMAP hBitmap;
	HDC hScreenDC = CreateDCA("DISPLAY", NULL, NULL, NULL);	 //通过指定DISPLAY来获取一个显示设备上下文环境
	HDC hmemDC = CreateCompatibleDC(hScreenDC);			 //该函数创建一个与指定设备兼容的内存设备上下文环境（DC）
	int ScreenWidth = GetDeviceCaps(hScreenDC, HORZRES);	 //获取指定设备的性能参数（此处获取屏幕宽度）
	int ScreenHeight = GetDeviceCaps(hScreenDC, VERTRES);	 //获取指定设备的性能参数（此处获取屏幕高度）
	HBITMAP hOldBM;
	PVOID lpData;	//内存分配成功返回的指向所分配内存块的首地址指针
	int startX;		//截图x位置
	int startY;		//截图y位置
	int width;		//截图宽度
	int height;		//截图高度
	long BitmapSize;
	DWORD BitsOffset;
	DWORD ImageSize;
	DWORD FileSize;
	BITMAPINFOHEADER bmInfo; //BITMAPINFOHEADER结构所包含的成员表明了图像的尺寸、原始设备的颜色格式、以及数据压缩方案
	BITMAPFILEHEADER bmFileHeader;
	HANDLE bmFile, hpal, holdpal = NULL;;
	DWORD dwWritten;
	if (lpRect == NULL)
	{
		startX = startY = 0;
		width = ScreenWidth;
		height = ScreenHeight;
	}
	else
	{
		startX = lpRect->left;
		startY = lpRect->top;
		width = lpRect->right - lpRect->left;
		height = lpRect->bottom - lpRect->top;
	}
	//创建一张长width，宽height的画布，用于后面绘制图形
	hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
	//该函数选择一对象到指定的设备上下文环境中，该新对象替换先前的相同类型的对象。
	hOldBM = (HBITMAP)SelectObject(hmemDC, hBitmap);
	//该函数对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。
	BitBlt(hmemDC, 0, 0, width, height, hScreenDC, startX, startY, SRCCOPY);
	hBitmap = (HBITMAP)SelectObject(hmemDC, hOldBM);
	if (filename == NULL)
	{
		DeleteDC(hScreenDC);
		DeleteDC(hmemDC);
		return hBitmap;
	}
	BitmapSize = ((((width * 32) + 32) / 32) * 4)*height;
	//用来在指定的堆上分配内存，并且分配后的内存不可移动(HEAP_NO_SERIALIZE 不使用连续存取)
	lpData = HeapAlloc(GetProcessHeap(), HEAP_NO_SERIALIZE, BitmapSize);
	ZeroMemory(lpData, BitmapSize);
	ZeroMemory(&bmInfo, sizeof(BITMAPINFOHEADER));
	bmInfo.biSize = sizeof(BITMAPINFOHEADER); //位图信息结构长度 ,必须为40
	bmInfo.biWidth = width;					//图像宽度 单位是像素
	bmInfo.biHeight = height;					//图像高度 单位是像素
	bmInfo.biPlanes = 1;						//必须为1
	bmInfo.biBitCount = bitCount;				//设置图像的位数。比如8位，16位，32位位数越高分辨率越高
	bmInfo.biCompression = BI_RGB;			//位图是否压缩 BI_RGB为不压缩
	ZeroMemory(&bmFileHeader, sizeof(BITMAPFILEHEADER));
	BitsOffset = sizeof(BITMAPFILEHEADER) + bmInfo.biSize;
	ImageSize = ((((bmInfo.biWidth*bmInfo.biBitCount) + 31) / 32) * 4)*bmInfo.biHeight;
	FileSize = BitsOffset + ImageSize;
	bmFileHeader.bfType = 0x4d42;//'B'+('M'<<8);
	bmFileHeader.bfSize = FileSize;
	bmFileHeader.bfOffBits = BitsOffset;
	hpal = GetStockObject(DEFAULT_PALETTE);
	if (hpal)
	{
		holdpal = SelectPalette(hmemDC, (HPALETTE)hpal, false);
		RealizePalette(hmemDC);
	}
	GetDIBits(hmemDC, hBitmap, 0, bmInfo.biHeight, lpData, (BITMAPINFO *)&bmInfo, DIB_RGB_COLORS);
	if (holdpal)
	{
		SelectPalette(hmemDC, (HPALETTE)holdpal, true);
		RealizePalette(hmemDC);
	}
	bmFile = CreateFileA(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (bmFile == INVALID_HANDLE_VALUE)
	{
		MessageBoxA(NULL, "Create File Error!", "提示", MB_OK | MB_ICONWARNING);
	}
	WriteFile(bmFile, &bmFileHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	WriteFile(bmFile, &bmInfo, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
	WriteFile(bmFile, lpData, ImageSize, &dwWritten, NULL);
	CloseHandle(bmFile);
	HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, lpData);
	::ReleaseDC(0, hScreenDC);
	DeleteDC(hmemDC);
	//logs->WriteFormat("【截图保存成功】");
	//MessageBoxA(NULL,"玩家您好，你的本场游戏战绩已经通过截图保存在桌面","提示",MB_OK);
	return hBitmap;
}
*/









//1 获取图像  2.创建BMP   3.写入文件
void CVideoRecoderDlg::OnBnClickedButton4()   //截图功能

	{
		CWnd *pDesktop = GetDesktopWindow();
		CDC *pdeskdc = pDesktop->GetDC();
		CRect re;
		//获取窗口的大小
		pDesktop->GetClientRect(&re);
		CBitmap bmp;
		bmp.CreateCompatibleBitmap(pdeskdc, re.Width(), re.Height());
		//创建一个兼容的内存画板
		CDC memorydc;
		memorydc.CreateCompatibleDC(pdeskdc);
		//选中画笔
		CBitmap *pold = memorydc.SelectObject(&bmp);
		//绘制图像
		memorydc.BitBlt(0, 0, re.Width(), re.Height(), pdeskdc, 0, 0, SRCCOPY);
		//获取鼠标位置，然后添加鼠标图像
		CPoint po;
		GetCursorPos(&po);
		HICON hinco = (HICON)GetCursor();
		memorydc.DrawIcon(po.x - 10, po.y - 10, hinco);
		//选中原来的画笔
		memorydc.SelectObject(pold);
		BITMAP bit;
		bmp.GetBitmap(&bit);
		//定义 图像大小（单位：byte）
		DWORD size = bit.bmWidthBytes * bit.bmHeight;
		LPSTR lpdata = (LPSTR)GlobalAlloc(GPTR, size);



		//后面是创建一个bmp文件的必须文件头
		BITMAPINFOHEADER pbitinfo;
		pbitinfo.biBitCount = 24;
		pbitinfo.biClrImportant = 0;
		pbitinfo.biCompression = BI_RGB;
		pbitinfo.biHeight = bit.bmHeight;
		pbitinfo.biPlanes = 1;
		pbitinfo.biSize = sizeof(BITMAPINFOHEADER);
		pbitinfo.biSizeImage = size;
		pbitinfo.biWidth = bit.bmWidth;
		pbitinfo.biXPelsPerMeter = 0;
		pbitinfo.biYPelsPerMeter = 0;
		GetDIBits(pdeskdc->m_hDC, bmp, 0, pbitinfo.biHeight, lpdata, (BITMAPINFO*)
			&pbitinfo, DIB_RGB_COLORS);


		BITMAPFILEHEADER bfh;
		bfh.bfReserved1 = bfh.bfReserved2 = 0;
		bfh.bfType = ((WORD)('M' << 8) | 'B');
		bfh.bfSize = size + 54;
		bfh.bfOffBits = 54;



		//写入文件
		CFile file;
		CString strFileName;// (GetAppPathW().c_str());
		//strFileName += _T("ScreenShot\\");

		strFileName = L"C:\\Users\\Administrator\\Desktop\\path\\";  

		CreateDirectory((LPCTSTR)strFileName, NULL);
		CTime t = CTime::GetCurrentTime();
		CString tt = t.Format("%Y-%m-%d_%H-%M-%S");
		strFileName += tt;
		strFileName += _T(".bmp");
		if (file.Open((LPCTSTR)strFileName, CFile::modeCreate | CFile::modeWrite))
		{
			file.Write(&bfh, sizeof(BITMAPFILEHEADER));
			file.Write(&pbitinfo, sizeof(BITMAPINFOHEADER));
			file.Write(lpdata, size);
			file.Close();
		}
		GlobalFree(lpdata);
	

}


void CVideoRecoderDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_timer)//如果上一次的定时器程序已经运行完，才执行下面的代码

	{
		m_timer = FALSE;//本次定时，正在运行，未结束前，不得进行下次运行

		CDC* pDeskDC = GetDesktopWindow()->GetDC();//获取桌面画布对象

		CRect rc;

		GetDesktopWindow()->GetClientRect(rc);//获取屏幕的客户区域

		CDC memDC;//定义一个内存画布

		memDC.CreateCompatibleDC(pDeskDC);//创建一个兼容的画布

		CBitmap bmp;

		bmp.CreateCompatibleBitmap(pDeskDC, rc.Width(), rc.Height());//创建兼容位图

		memDC.SelectObject(&bmp);//选中位图对象

		BITMAP bitmap;

		bmp.GetBitmap(&bitmap);

		memDC.BitBlt(0, 0, bitmap.bmWidth, bitmap.bmHeight, pDeskDC, 0, 0, SRCCOPY);

		DWORD size = bitmap.bmWidthBytes*bitmap.bmHeight;//屏幕图像的总像素数

		BYTE* lpData = new BYTE[size];//申请一个大小为size的BYTE型数组，用于存放桌面图像的数据

		int panelsize = 0; //记录调色板大小

		if (bitmap.bmBitsPixel<16)//判断是否为真彩色位图

			panelsize = (int)pow(2, bitmap.bmBitsPixel * sizeof(RGBQUAD));

		BITMAPINFOHEADER *pBInfo = new BITMAPINFOHEADER;//定义位图信息头结构体

														//初始化位图信息头

		pBInfo->biBitCount = bitmap.bmBitsPixel;

		pBInfo->biClrImportant = 0;

		pBInfo->biCompression = 0;

		pBInfo->biHeight = bitmap.bmHeight;

		pBInfo->biPlanes = bitmap.bmPlanes;

		pBInfo->biSize = sizeof(BITMAPINFOHEADER);

		pBInfo->biSizeImage = bitmap.bmWidthBytes*bitmap.bmHeight;

		pBInfo->biWidth = bitmap.bmWidth;

		pBInfo->biXPelsPerMeter = 0;

		pBInfo->biYPelsPerMeter = 0;

		BITMAPINFO bInfo;//定义位图信息结构体

		bInfo.bmiHeader = *pBInfo;//初始化

		GetDIBits(memDC.m_hDC, bmp, 0, pBInfo->biHeight, lpData, &bInfo, DIB_RGB_COLORS);

		//将图像数据保存到pdata


		//---------------------------------

		if (nFrames == 0)//如果是第一帧

		{
			//CString rate = L"3";//3帧
			char * rate = "3";
			CString filename = L"C:\\Users\\Administrator\\Desktop\\path\\chen.avi";//文件名

			AVIFileOpen(&pfile, filename, OF_WRITE | OF_CREATE, NULL);//打开或创建AVI文件

			memset(&strhdr, 0, sizeof(strhdr));//初始化信息头为0

											   //初始化AVI视频流信息结构体

			strhdr.fccType = streamtypeVIDEO;

			strhdr.fccHandler = 0;

			strhdr.dwScale = 1;

			strhdr.dwRate = atoi(rate);

			strhdr.dwSuggestedBufferSize = pBInfo->biSizeImage;

			SetRect(&strhdr.rcFrame, 0, 0, pBInfo->biWidth, pBInfo->biHeight);

			hr = AVIFileCreateStream(pfile, &ps, &strhdr);//创建AVI文件流

			opts[0]->fccType = streamtypeVIDEO;

			opts[0]->fccHandler = mmioStringToFOURCC(L"MSVC", 0);

			opts[0]->dwQuality = 7500;

			opts[0]->dwBytesPerSecond = 0;

			opts[0]->dwFlags = AVICOMPRESSF_VALID || AVICOMPRESSF_KEYFRAMES;

			opts[0]->lpFormat = 0;

			opts[0]->cbFormat = 0;

			opts[0]->dwInterleaveEvery = 0;

			AVIMakeCompressedStream(&pComStream, ps, &pCompressOption, NULL);

			AVIStreamSetFormat(pComStream, 0, pBInfo, sizeof(BITMAPINFOHEADER));

		}

		hr = AVIStreamWrite(pComStream, nFrames, 1, (LPBYTE)lpData,

			pBInfo->biSizeImage, AVIIF_KEYFRAME, NULL, NULL);//将桌面图像数据写入文件

		nFrames++;//增加一帧

		delete[]lpData;//释放内存

		delete pBInfo; //释放内存

		m_timer = TRUE;//定时器程序可以继续响应

	}


	CDialogEx::OnTimer(nIDEvent);
}


void CVideoRecoderDlg::OnBnClickedButton5()  //开始
{
	// TODO: 在此添加控件通知处理程序代码

	timer_num = SetTimer(1, 100, 0);//启动定时器

	m_timer = TRUE;//设置定时器可用

	AVIFileInit(); //初始化AVI文件

}


void CVideoRecoderDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_IsPause == FALSE)

	{
		m_IsPause = !m_IsPause;

		if (timer_num)

		{
			KillTimer(timer_num);

		}

	}

	else

	{
		m_IsPause = !m_IsPause;

		timer_num = SetTimer(1, 100, NULL);

	}

}


void CVideoRecoderDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码

	if (timer_num)

	{
		KillTimer(timer_num);

	}

	m_timer = FALSE;

	if (pComStream)

	{
		AVIStreamClose(pComStream);

	}

	if (ps)

	{
		AVIStreamClose(ps);

	}

	if (pfile != NULL)

		AVIFileRelease(pfile);

	AVIFileExit();

	nFrames = 0;

}
