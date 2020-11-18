// Product_Dialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SuperMarket.h"
#include "Product_Dialog.h"
#include "afxdialogex.h"


// Product_Dialog �Ի���

IMPLEMENT_DYNAMIC(Product_Dialog, CDialogEx)

Product_Dialog::Product_Dialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RPRDUCT_DIALOG, pParent)
{

	EnableAutomation();

	//m_Grid.InsertColumn(0, L"����"); //�������
	//m_Grid.InsertColumn(1, L"��·");
	//m_Grid.InsertColumn(2, L"�绰����");
	//m_Grid.InsertColumn(3, L"��ע");

}

Product_Dialog::~Product_Dialog()
{
}


BOOL Product_Dialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_Grid.InsertColumn(0, L"��Ʒ���",LVCFMT_LEFT,80,0); //�������
	m_Grid.InsertColumn(1, L"��Ʒ����",LVCFMT_LEFT, 80, 1);
	m_Grid.InsertColumn(2, L"�绰����", LVCFMT_LEFT, 80, 2);
	m_Grid.InsertColumn(3, L"��Ʒ���", LVCFMT_LEFT, 80, 3);
	m_Grid.InsertColumn(4, L"��Ʒ��λ", LVCFMT_LEFT, 80, 4);
	m_Grid.InsertColumn(5, L"������", LVCFMT_LEFT, 80, 5);
	m_Grid.InsertColumn(6, L"�����۸�", LVCFMT_LEFT, 80, 6);
	m_Grid.InsertColumn(7, L"���ۼ۸�", LVCFMT_LEFT, 80, 7);


	/**/
	//m_Grid.InsertItem(0, L"");
	//m_Grid.SetItemText(0, 0, L"001");
	//m_Grid.SetItemText(0, 1, L"001");
	//m_Grid.SetItemText(0, 2, L"001");
	//m_Grid.SetItemText(0, 3, L"001");
	//m_Grid.SetItemText(0, 4, L"001");
	//m_Grid.SetItemText(0, 5, L"001");
	//m_Grid.SetItemText(0, 6, L"001");
	//m_Grid.SetItemText(0, 7, L"001");

	//m_Grid.SetColumnWidth(0, 120); //�����п�
	//m_Grid.SetColumnWidth(1, 120);
	//m_Grid.SetColumnWidth(2, 180);
	//m_Grid.SetColumnWidth(3, 120);

	m_Grid.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_HEADERDRAGDROP | LVS_EX_GRIDLINES);//������չ��ʽ������ѡ��������

	AddToGrid();

	return TRUE;
}

void Product_Dialog::AddToGrid()
{
	SqlBase Product_Sql;
	Product_Sql.ConnectSql();
	CString sql;
	int i = 0;
	sql.Format(L"select *from ProductInformation");
	Product_Sql.m_pRecordset = Product_Sql.GetRecordSet((_bstr_t)sql);
	while (!Product_Sql.m_pRecordset->adoEOF)
	{
		m_Grid.InsertItem(i, L"");
		m_Grid.SetItemText(i, 0,(_bstr_t)(_variant_t)Product_Sql.m_pRecordset->GetCollect(L"NAME"));
		m_Grid.SetItemText(i, 1, (_bstr_t)(_variant_t)Product_Sql.m_pRecordset->GetCollect(L"PASSWORD"));
		//m_Grid.SetItemText(i, 1, (_bstr_t)(_variant_t)Product_Sql.m_pRecordset->GetCollect(L"SPBH"));
		Product_Sql.m_pRecordset->MoveNext();
		i++;
	}
	Product_Sql.Release();
	Product_Sql.CloseSql();



}

void Product_Dialog::OnFinalRelease()
{
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease��  ���ཫ�Զ�
	// ɾ���ö���  �ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

	CDialogEx::OnFinalRelease();
}

void Product_Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_Grid);
}


BEGIN_MESSAGE_MAP(Product_Dialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Product_Dialog::OnBnClickedButton1)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(Product_Dialog, CDialogEx)
END_DISPATCH_MAP()

// ע��: ������� IID_IProduct_Dialog ֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡�  �� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {9CBF6F31-1EB6-433D-8EFA-CB41EB94B5BF}
static const IID IID_IProduct_Dialog =
{ 0x9CBF6F31, 0x1EB6, 0x433D, { 0x8E, 0xFA, 0xCB, 0x41, 0xEB, 0x94, 0xB5, 0xBF } };

BEGIN_INTERFACE_MAP(Product_Dialog, CDialogEx)
	INTERFACE_PART(Product_Dialog, IID_IProduct_Dialog, Dispatch)
END_INTERFACE_MAP()


// Product_Dialog ��Ϣ�������

//���
void Product_Dialog::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString m_name,m_password;
	GetDlgItem(IDC_EDIT2)->GetWindowText(m_name);
	GetDlgItem(IDC_EDIT3)->GetWindowText(m_password);

	SqlBase Product_Sql;
	Product_Sql.ConnectSql();
	CString sql;
	int i = 0;
	sql.Format(L"insert into   ProductInformation(NAME,PASSWORD) values ('%s','%s')", m_name, m_password);

	Product_Sql.ExecuteSql((_bstr_t)sql);
	m_Grid.DeleteAllItems();
	AddToGrid();



	Product_Sql.Release();
	Product_Sql.CloseSql();

}
