// Main_Dialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SuperMarket.h"
#include "Main_Dialog.h"
#include "afxdialogex.h"


// Main_Dialog 对话框

IMPLEMENT_DYNAMIC(Main_Dialog, CDialogEx)

Main_Dialog::Main_Dialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAIN_DIALOG1, pParent)
{

	m_font.CreatePointFont(150, _T("宋体"));//代表15号字体，华文行楷
	EnableAutomation();

}

Main_Dialog::~Main_Dialog()
{
}

void Main_Dialog::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。  基类将自动
	// 删除该对象。  在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CDialogEx::OnFinalRelease();
}

void Main_Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Main_Dialog, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(Main_Dialog, CDialogEx)
END_DISPATCH_MAP()

// 注意: 我们添加 IID_IMain_Dialog 支持
//  以支持来自 VBA 的类型安全绑定。  此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {F5B0C755-84F3-42F1-870A-0D6EB6AF18DE}
static const IID IID_IMain_Dialog =
{ 0xF5B0C755, 0x84F3, 0x42F1, { 0x87, 0xA, 0xD, 0x6E, 0xB6, 0xAF, 0x18, 0xDE } };

BEGIN_INTERFACE_MAP(Main_Dialog, CDialogEx)
	INTERFACE_PART(Main_Dialog, IID_IMain_Dialog, Dispatch)
END_INTERFACE_MAP()


// Main_Dialog 消息处理程序


void Main_Dialog::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Main_Dialog *pDlg = (Main_Dialog *)GetParent();
	pDlg->ShowWindow(SW_SHOW);                     //显示主对话框
	CDialogEx::OnClose();
}


void Main_Dialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()

	CRect   rect;
	//CPaintDC   dc(this);
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(128, 128, 128));     //设置为绿色背景   

}


HBRUSH Main_Dialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_DLG)      //对话框颜色  
		return   m_brush;       //返加绿色刷子    
	if (nCtlColor == CTLCOLOR_STATIC && pWnd->GetDlgCtrlID() == IDC_STATIC)  //静态文本颜色，并且指定控件ID号  
	{
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->SetBkColor(RGB(128, 128, 128));     //静态控件的背景颜色设置，这里设置于背景颜色一致，则可以免设置透明  
		//pDC->SetBkMode(TRANSPARENT); 										 //pDC->SetBkMode(TRANSPARENT);    //模式设置透明的话，则忽略静态控件的背景颜色设置，与对话框颜色融合  
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
