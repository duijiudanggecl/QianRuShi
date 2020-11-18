
// VideoRecoderDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VideoRecoder.h"
#include "VideoRecoderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include<math.h>//��ѧ�������ͷ�ļ�

#include<Vfw.h>//����AVI��Ƶ��������ͷ�ļ�

#pragma comment(lib,"Vfw32.lib")//���ӿ�Vfw32.lib

#pragma comment(lib,"Winmm.lib")//���ӿ�

//����AVI��Ƶ����������Ľṹ�����

AVISTREAMINFO strhdr;

PAVIFILE pfile;

PAVISTREAM ps;

PAVISTREAM pComStream;

AVICOMPRESSOPTIONS pCompressOption;

AVICOMPRESSOPTIONS FAR * opts[1] = { &pCompressOption };

HRESULT hr;

int nFrames = 0; //������Ƶ��֡�ĸ���

BOOL m_timer = TRUE;//��ʱ������״̬��־

UINT timer_num = 0;//��Ƕ�ʱ��������

BOOL m_IsPause = FALSE;//�Ƿ���ͣ��־



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


// CVideoRecoderDlg �Ի���



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


// CVideoRecoderDlg ��Ϣ�������

BOOL CVideoRecoderDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	::RegisterHotKey(m_hWnd, 199, MOD_ALT, 'U');//ע��Ҫ��д��ע����ȼ�idΪ199���Լ����壩��Alt+U

	::RegisterHotKey(m_hWnd, 200, NULL, 'Q');//ע��Ҫ��д��ע����ȼ�idΪ199���Լ����壩��Alt+U

	::RegisterHotKey(m_hWnd, 201, NULL, 'W');//ע��Ҫ��д��ע����ȼ�idΪ199���Լ����壩��Alt+U

	m_icon1 = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_icon2 = AfxGetApp()->LoadIcon(IDI_ICON1);


	//Create(1);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CVideoRecoderDlg::OnPaint()
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
HCURSOR CVideoRecoderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVideoRecoderDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nHotKeyId == 199)

	{
		AfxMessageBox(_T("ʹ�����ȼ�"));

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ULARGE_INTEGER  totalsize;
	ULARGE_INTEGER  freesize;
	ULARGE_INTEGER  availablesize;
	CString  str, temp, path;

	BOOL isOpen = FALSE;		//�Ƿ��(����Ϊ����)
	CString defaultDir = L"E:\\FileTest";	//Ĭ�ϴ򿪵��ļ�·��
	CString fileName = L"1.avi";			//Ĭ�ϴ򿪵��ļ���
	CString filter = L"�ļ� (*.avi; *.*)|*.avi; *.*||";	//�ļ����ǵ�����
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);


	if (openFileDlg.DoModal() == IDOK)
	{
		path = openFileDlg.GetPathName();
	}


//	m_Path.GetWindowText(path);
	str = path.Left(3);
	::GetDiskFreeSpaceEx(str, &totalsize, &freesize, &availablesize);   //��ȡ���̿ռ���Ϣ
	temp.Format(L"%ldM", totalsize.QuadPart / 1024 / 1024);

	m_Path.SetWindowText(L"ʣ��ռ䣺"+ str+temp);

	//m_Path.SetWindowText(str);

}

//�ļ�������
void CVideoRecoderDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString path;
	m_Path.GetWindowText(path);
	CFileFind file;
	//if (path.Right(1) != "\\")
	//	path = +"\\*.txt";
	path = L"C:\\Users\\Administrator\\Desktop\\path\\*.txt";

	BOOL bf;
	bf = file.FindFile(path);  //Ѱ�Һ�׺Ϊ.txt���ļ�
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

//�����ļ���
void CVideoRecoderDlg::SetFileName()
{
	int count = m_list.GetItemCount();

	int flag = 0;
	CString strName;
	int i = 1;
	while (1)
	{
		strName.Format(L"¼��%d", i);

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

    end:strName.Format(L"¼��%d", i);

	m_Path.SetWindowText(strName);

}

afx_msg LRESULT CVideoRecoderDlg::OnOntray(WPARAM wParam, LPARAM lParam)
{


	//	if (wParam != IDR_MAINFRAME)
	//		return   1;



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



void CVideoRecoderDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString oname;

	::ShellExecute(GetSafeHwnd(), TEXT("OPEN"), TEXT("D:\\BaiduNetdiskDownload\\1.wmv"), TEXT(""), NULL, SW_SHOWNORMAL);//������Ƶ
	//������Ƶ

}

/*
//�ѽ�ͼ���ļ����浽����
 BOOL CaptureLolToDesktop(HWND hWnd)
{
	Sleep(1 * 1000);
	LPRECT lprc = new RECT;
	GetWindowRect(hWnd, lprc);
	CString strTime;
	DWORD llocaltime = GetTickCount();
	char chDesktopPath[MAX_PATH] = { 0 };
	SHGetSpecialFolderPathA(NULL, chDesktopPath, CSIDL_DESKTOP, 0);//��ȡ��ǰ�û�����·��
	strTime.Format(L"%s\\��ͼ%ld.jpg", chDesktopPath, llocaltime);
	logs->WriteFormat("��ͼ����·��Ϊ%s", strTime.c_str());
	ScreenCapture(LPSTR(strTime.c_str()), 16, lprc);
	delete lprc;
	return TRUE;
}

//��Ļ��ͼ����ʵ��
 HBITMAP ScreenCapture(LPSTR filename, WORD bitCount, LPRECT lpRect)
{
	HBITMAP hBitmap;
	HDC hScreenDC = CreateDCA("DISPLAY", NULL, NULL, NULL);	 //ͨ��ָ��DISPLAY����ȡһ����ʾ�豸�����Ļ���
	HDC hmemDC = CreateCompatibleDC(hScreenDC);			 //�ú�������һ����ָ���豸���ݵ��ڴ��豸�����Ļ�����DC��
	int ScreenWidth = GetDeviceCaps(hScreenDC, HORZRES);	 //��ȡָ���豸�����ܲ������˴���ȡ��Ļ��ȣ�
	int ScreenHeight = GetDeviceCaps(hScreenDC, VERTRES);	 //��ȡָ���豸�����ܲ������˴���ȡ��Ļ�߶ȣ�
	HBITMAP hOldBM;
	PVOID lpData;	//�ڴ����ɹ����ص�ָ���������ڴ����׵�ַָ��
	int startX;		//��ͼxλ��
	int startY;		//��ͼyλ��
	int width;		//��ͼ���
	int height;		//��ͼ�߶�
	long BitmapSize;
	DWORD BitsOffset;
	DWORD ImageSize;
	DWORD FileSize;
	BITMAPINFOHEADER bmInfo; //BITMAPINFOHEADER�ṹ�������ĳ�Ա������ͼ��ĳߴ硢ԭʼ�豸����ɫ��ʽ���Լ�����ѹ������
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
	//����һ�ų�width����height�Ļ��������ں������ͼ��
	hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
	//�ú���ѡ��һ����ָ�����豸�����Ļ����У����¶����滻��ǰ����ͬ���͵Ķ���
	hOldBM = (HBITMAP)SelectObject(hmemDC, hBitmap);
	//�ú�����ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸������
	BitBlt(hmemDC, 0, 0, width, height, hScreenDC, startX, startY, SRCCOPY);
	hBitmap = (HBITMAP)SelectObject(hmemDC, hOldBM);
	if (filename == NULL)
	{
		DeleteDC(hScreenDC);
		DeleteDC(hmemDC);
		return hBitmap;
	}
	BitmapSize = ((((width * 32) + 32) / 32) * 4)*height;
	//������ָ���Ķ��Ϸ����ڴ棬���ҷ������ڴ治���ƶ�(HEAP_NO_SERIALIZE ��ʹ��������ȡ)
	lpData = HeapAlloc(GetProcessHeap(), HEAP_NO_SERIALIZE, BitmapSize);
	ZeroMemory(lpData, BitmapSize);
	ZeroMemory(&bmInfo, sizeof(BITMAPINFOHEADER));
	bmInfo.biSize = sizeof(BITMAPINFOHEADER); //λͼ��Ϣ�ṹ���� ,����Ϊ40
	bmInfo.biWidth = width;					//ͼ���� ��λ������
	bmInfo.biHeight = height;					//ͼ��߶� ��λ������
	bmInfo.biPlanes = 1;						//����Ϊ1
	bmInfo.biBitCount = bitCount;				//����ͼ���λ��������8λ��16λ��32λλ��Խ�߷ֱ���Խ��
	bmInfo.biCompression = BI_RGB;			//λͼ�Ƿ�ѹ�� BI_RGBΪ��ѹ��
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
		MessageBoxA(NULL, "Create File Error!", "��ʾ", MB_OK | MB_ICONWARNING);
	}
	WriteFile(bmFile, &bmFileHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	WriteFile(bmFile, &bmInfo, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
	WriteFile(bmFile, lpData, ImageSize, &dwWritten, NULL);
	CloseHandle(bmFile);
	HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, lpData);
	::ReleaseDC(0, hScreenDC);
	DeleteDC(hmemDC);
	//logs->WriteFormat("����ͼ����ɹ���");
	//MessageBoxA(NULL,"������ã���ı�����Ϸս���Ѿ�ͨ����ͼ����������","��ʾ",MB_OK);
	return hBitmap;
}
*/









//1 ��ȡͼ��  2.����BMP   3.д���ļ�
void CVideoRecoderDlg::OnBnClickedButton4()   //��ͼ����

	{
		CWnd *pDesktop = GetDesktopWindow();
		CDC *pdeskdc = pDesktop->GetDC();
		CRect re;
		//��ȡ���ڵĴ�С
		pDesktop->GetClientRect(&re);
		CBitmap bmp;
		bmp.CreateCompatibleBitmap(pdeskdc, re.Width(), re.Height());
		//����һ�����ݵ��ڴ滭��
		CDC memorydc;
		memorydc.CreateCompatibleDC(pdeskdc);
		//ѡ�л���
		CBitmap *pold = memorydc.SelectObject(&bmp);
		//����ͼ��
		memorydc.BitBlt(0, 0, re.Width(), re.Height(), pdeskdc, 0, 0, SRCCOPY);
		//��ȡ���λ�ã�Ȼ��������ͼ��
		CPoint po;
		GetCursorPos(&po);
		HICON hinco = (HICON)GetCursor();
		memorydc.DrawIcon(po.x - 10, po.y - 10, hinco);
		//ѡ��ԭ���Ļ���
		memorydc.SelectObject(pold);
		BITMAP bit;
		bmp.GetBitmap(&bit);
		//���� ͼ���С����λ��byte��
		DWORD size = bit.bmWidthBytes * bit.bmHeight;
		LPSTR lpdata = (LPSTR)GlobalAlloc(GPTR, size);



		//�����Ǵ���һ��bmp�ļ��ı����ļ�ͷ
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



		//д���ļ�
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_timer)//�����һ�εĶ�ʱ�������Ѿ������꣬��ִ������Ĵ���

	{
		m_timer = FALSE;//���ζ�ʱ���������У�δ����ǰ�����ý����´�����

		CDC* pDeskDC = GetDesktopWindow()->GetDC();//��ȡ���滭������

		CRect rc;

		GetDesktopWindow()->GetClientRect(rc);//��ȡ��Ļ�Ŀͻ�����

		CDC memDC;//����һ���ڴ滭��

		memDC.CreateCompatibleDC(pDeskDC);//����һ�����ݵĻ���

		CBitmap bmp;

		bmp.CreateCompatibleBitmap(pDeskDC, rc.Width(), rc.Height());//��������λͼ

		memDC.SelectObject(&bmp);//ѡ��λͼ����

		BITMAP bitmap;

		bmp.GetBitmap(&bitmap);

		memDC.BitBlt(0, 0, bitmap.bmWidth, bitmap.bmHeight, pDeskDC, 0, 0, SRCCOPY);

		DWORD size = bitmap.bmWidthBytes*bitmap.bmHeight;//��Ļͼ�����������

		BYTE* lpData = new BYTE[size];//����һ����СΪsize��BYTE�����飬���ڴ������ͼ�������

		int panelsize = 0; //��¼��ɫ���С

		if (bitmap.bmBitsPixel<16)//�ж��Ƿ�Ϊ���ɫλͼ

			panelsize = (int)pow(2, bitmap.bmBitsPixel * sizeof(RGBQUAD));

		BITMAPINFOHEADER *pBInfo = new BITMAPINFOHEADER;//����λͼ��Ϣͷ�ṹ��

														//��ʼ��λͼ��Ϣͷ

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

		BITMAPINFO bInfo;//����λͼ��Ϣ�ṹ��

		bInfo.bmiHeader = *pBInfo;//��ʼ��

		GetDIBits(memDC.m_hDC, bmp, 0, pBInfo->biHeight, lpData, &bInfo, DIB_RGB_COLORS);

		//��ͼ�����ݱ��浽pdata


		//---------------------------------

		if (nFrames == 0)//����ǵ�һ֡

		{
			//CString rate = L"3";//3֡
			char * rate = "3";
			CString filename = L"C:\\Users\\Administrator\\Desktop\\path\\chen.avi";//�ļ���

			AVIFileOpen(&pfile, filename, OF_WRITE | OF_CREATE, NULL);//�򿪻򴴽�AVI�ļ�

			memset(&strhdr, 0, sizeof(strhdr));//��ʼ����ϢͷΪ0

											   //��ʼ��AVI��Ƶ����Ϣ�ṹ��

			strhdr.fccType = streamtypeVIDEO;

			strhdr.fccHandler = 0;

			strhdr.dwScale = 1;

			strhdr.dwRate = atoi(rate);

			strhdr.dwSuggestedBufferSize = pBInfo->biSizeImage;

			SetRect(&strhdr.rcFrame, 0, 0, pBInfo->biWidth, pBInfo->biHeight);

			hr = AVIFileCreateStream(pfile, &ps, &strhdr);//����AVI�ļ���

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

			pBInfo->biSizeImage, AVIIF_KEYFRAME, NULL, NULL);//������ͼ������д���ļ�

		nFrames++;//����һ֡

		delete[]lpData;//�ͷ��ڴ�

		delete pBInfo; //�ͷ��ڴ�

		m_timer = TRUE;//��ʱ��������Լ�����Ӧ

	}


	CDialogEx::OnTimer(nIDEvent);
}


void CVideoRecoderDlg::OnBnClickedButton5()  //��ʼ
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	timer_num = SetTimer(1, 100, 0);//������ʱ��

	m_timer = TRUE;//���ö�ʱ������

	AVIFileInit(); //��ʼ��AVI�ļ�

}


void CVideoRecoderDlg::OnBnClickedButton7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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
