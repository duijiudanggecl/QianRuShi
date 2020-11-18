// Main_Dialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SuperMarket.h"
#include "Main_Dialog.h"
#include "afxdialogex.h"


// Main_Dialog �Ի���

IMPLEMENT_DYNAMIC(Main_Dialog, CDialogEx)

Main_Dialog::Main_Dialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAIN_DIALOG1, pParent)
{

	m_font.CreatePointFont(150, _T("����"));//����15�����壬�����п�
	EnableAutomation();

}

Main_Dialog::~Main_Dialog()
{
}

void Main_Dialog::OnFinalRelease()
{
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease��  ���ཫ�Զ�
	// ɾ���ö���  �ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

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

// ע��: ������� IID_IMain_Dialog ֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡�  �� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {F5B0C755-84F3-42F1-870A-0D6EB6AF18DE}
static const IID IID_IMain_Dialog =
{ 0xF5B0C755, 0x84F3, 0x42F1, { 0x87, 0xA, 0xD, 0x6E, 0xB6, 0xAF, 0x18, 0xDE } };

BEGIN_INTERFACE_MAP(Main_Dialog, CDialogEx)
	INTERFACE_PART(Main_Dialog, IID_IMain_Dialog, Dispatch)
END_INTERFACE_MAP()


// Main_Dialog ��Ϣ�������


void Main_Dialog::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	Main_Dialog *pDlg = (Main_Dialog *)GetParent();
	pDlg->ShowWindow(SW_SHOW);                     //��ʾ���Ի���
	CDialogEx::OnClose();
}


void Main_Dialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

	CRect   rect;
	//CPaintDC   dc(this);
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(128, 128, 128));     //����Ϊ��ɫ����   

}


HBRUSH Main_Dialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (nCtlColor == CTLCOLOR_DLG)      //�Ի�����ɫ  
		return   m_brush;       //������ɫˢ��    
	if (nCtlColor == CTLCOLOR_STATIC && pWnd->GetDlgCtrlID() == IDC_STATIC)  //��̬�ı���ɫ������ָ���ؼ�ID��  
	{
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->SetBkColor(RGB(128, 128, 128));     //��̬�ؼ��ı�����ɫ���ã����������ڱ�����ɫһ�£������������͸��  
		//pDC->SetBkMode(TRANSPARENT); 										 //pDC->SetBkMode(TRANSPARENT);    //ģʽ����͸���Ļ�������Ծ�̬�ؼ��ı�����ɫ���ã���Ի�����ɫ�ں�  
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
