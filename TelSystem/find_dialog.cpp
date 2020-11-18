// find_dialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TelSystem.h"
#include "find_dialog.h"
#include "afxdialogex.h"


#include "mmsystem.h"
#pragma comment (lib,"WINMM.LIB")


// find_dialog �Ի���

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

	m_List.InsertColumn(0, L"����"); //�������
	m_List.InsertColumn(1, L"��·");
	m_List.InsertColumn(2, L"�绰����");
	m_List.InsertColumn(3, L"��ע");

	m_List.SetColumnWidth(0, 120); //�����п�
	m_List.SetColumnWidth(1, 120);
	m_List.SetColumnWidth(2, 180);
	m_List.SetColumnWidth(3, 120);

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);//������չ��ʽ������ѡ��������

	m_List.SetBkColor(RGB(0, 128, 128));
	m_List.SetTextBkColor(RGB(0, 128, 128));
	m_List.SetTextColor(RGB(0, 25, 250));

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


// find_dialog ��Ϣ�������


void find_dialog::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString ReturnPath, strText;
	TCHAR szPath[_MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.lpszTitle = _T("��ѡ�񱸷��ļ���");
	bi.pszDisplayName = szPath;
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	bi.lpfn = NULL;
	bi.lParam = NULL;
	LPITEMIDLIST pltemlDList = SHBrowseForFolder(&bi);    //�ļ��жԻ���

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������


}



void find_dialog::OnBnClickedPlay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//MessageBox(L"---PLAY---");
	
	UINT wMIDIDeviceID;
	MCI_OPEN_PARMS mciOpenParams;
	MCI_PLAY_PARMS mciPlayParams;
	DWORD dwError;

	mciOpenParams.lpstrDeviceType = L"sequencer";
	mciOpenParams.lpstrElementName = L"D:\\main.mid"; //Ҫ���ŵ�MIDI
	if (dwError = mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&mciOpenParams))
		; // ����
	else
		wMIDIDeviceID = mciOpenParams.wDeviceID;

	if (dwError = mciSendCommand(wMIDIDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlayParams))
	{
		mciSendCommand(wMIDIDeviceID, MCI_CLOSE, 0, NULL); //�ر�
	}



/*

	UINT wMIDIDeviceID;
	MCI_OPEN_PARMS mciOpenParams;
	MCI_PLAY_PARMS mciPlayParams;
	DWORD dwError;

//	mciOpenParams.lpstrDeviceType = L"sequencer";


	mciOpenParams.lpstrElementName = L"D:\\main.mid"; //Ҫ���ŵ�MIDI
	if (dwError = mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&mciOpenParams))
		; // ����
	else
		wMIDIDeviceID = mciOpenParams.wDeviceID;

	if (dwError = mciSendCommand(wMIDIDeviceID, MCI_PLAY, 0, (DWORD)(LPVOID)&mciPlayParams))
	{
		mciSendCommand(wMIDIDeviceID, MCI_CLOSE, 0, NULL); //�ر�
	}

	*/

//	sndPlaySound(L"D:\\1.wav", SND_ASYNC );  �ɲ���WAV�ļ� 

}
