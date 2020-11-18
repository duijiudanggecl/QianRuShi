// InfoDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TelSystem.h"
#include "InfoDialog.h"
#include "afxdialogex.h"


// InfoDialog 对话框

IMPLEMENT_DYNAMIC(InfoDialog, CDialogEx)

InfoDialog::InfoDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{


}

InfoDialog::~InfoDialog()
{
}


BOOL InfoDialog::OnInitDialog()
{


	

	//m_ImageList.Create(32, 32, ILC_COLOR24 || ILC_MASK, 1, 0);
	//m_ImageList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	//m_List.SetImageList(&m_ImageList, LVSIL_NORMAL);


	m_List.InsertItem(0, L"类型", 0);
/*
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

	*/

	return TRUE;


}


void InfoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(InfoDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &InfoDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// InfoDialog 消息处理程序


void InfoDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	MessageBox(L"OK");
}
