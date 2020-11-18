// find_dialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TelSystem.h"
#include "find_dialog.h"
#include "afxdialogex.h"


#include "mmsystem.h"
#pragma comment (lib,"WINMM.LIB")


// find_dialog 对话框

IMPLEMENT_DYNAMIC(find_dialog, CDialogEx)

find_dialog::find_dialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FIND_DIALOG, pParent)
{






}

find_dialog::~find_dialog()
{
}



BOOL find_dialog::OnInitDialog()
{



	//m_ImageList.Create(32, 32, ILC_COLOR24 || ILC_MASK, 1, 0);
	//m_ImageList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	//m_List.SetImageList(&m_ImageList, LVSIL_NORMAL);

	m_List.InsertColumn(0, L"类型"); //添加列名
	m_List.InsertColumn(1, L"线路");
	m_List.InsertColumn(2, L"电话号码");
	m_List.InsertColumn(3, L"备注");

	m_List.SetColumnWidth(0, 120); //设置列宽
	m_List.SetColumnWidth(1, 120);
	m_List.SetColumnWidth(2, 180);
	m_List.SetColumnWidth(3, 120);

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);//设置扩展格式，整行选择，网格线

	m_List.SetBkColor(RGB(0, 128, 128));
	m_List.SetTextBkColor(RGB(0, 128, 128));
	m_List.SetTextColor(RGB(0, 25, 250));

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



	return TRUE;


}




void find_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Edit);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(find_dialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &find_dialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &find_dialog::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_Play, &find_dialog::OnBnClickedPlay)
END_MESSAGE_MAP()


// find_dialog 消息处理程序


void find_dialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString ReturnPath, strText;
	TCHAR szPath[_MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.lpszTitle = _T("请选择备份文件夹");
	bi.pszDisplayName = szPath;
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	bi.lpfn = NULL;
	bi.lParam = NULL;
	LPITEMIDLIST pltemlDList = SHBrowseForFolder(&bi);    //文件夹对话框

	if (pltemlDList)
	{
		if (SHGetPathFromIDList(pltemlDList, szPath))
		    	ReturnPath = szPath;
	}

	else {
		ReturnPath = "";
	}
	strText = ReturnPath;
	
	if(strText.Right(1)!="\\")
		strText += "\\";

	m_Edit.SetWindowText(strText);
}


void find_dialog::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码


}



void find_dialog::OnBnClickedPlay()
{
	// TODO: 在此添加控件通知处理程序代码
	//MessageBox(L"---PLAY---");
	
	UINT wMIDIDeviceID;
	MCI_OPEN_PARMS mciOpenParams;
	MCI_PLAY_PARMS mciPlayParams;
	DWORD dwError;

	mciOpenParams.lpstrDeviceType = L"sequencer";
	mciOpenParams.lpstrElementName = L"D:\\main.mid"; //要播放的MIDI
	if (dwError = mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&mciOpenParams))
		; // 播放
	else
		wMIDIDeviceID = mciOpenParams.wDeviceID;

	if (dwError = mciSendCommand(wMIDIDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlayParams))
	{
		mciSendCommand(wMIDIDeviceID, MCI_CLOSE, 0, NULL); //关闭
	}



/*

	UINT wMIDIDeviceID;
	MCI_OPEN_PARMS mciOpenParams;
	MCI_PLAY_PARMS mciPlayParams;
	DWORD dwError;

//	mciOpenParams.lpstrDeviceType = L"sequencer";


	mciOpenParams.lpstrElementName = L"D:\\main.mid"; //要播放的MIDI
	if (dwError = mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&mciOpenParams))
		; // 播放
	else
		wMIDIDeviceID = mciOpenParams.wDeviceID;

	if (dwError = mciSendCommand(wMIDIDeviceID, MCI_PLAY, 0, (DWORD)(LPVOID)&mciPlayParams))
	{
		mciSendCommand(wMIDIDeviceID, MCI_CLOSE, 0, NULL); //关闭
	}

	*/

//	sndPlaySound(L"D:\\1.wav", SND_ASYNC );  可播放WAV文件 

}
