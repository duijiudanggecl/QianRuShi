// Product_Dialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SuperMarket.h"
#include "Product_Dialog.h"
#include "afxdialogex.h"


// Product_Dialog 对话框

IMPLEMENT_DYNAMIC(Product_Dialog, CDialogEx)

Product_Dialog::Product_Dialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RPRDUCT_DIALOG, pParent)
{

	EnableAutomation();

	//m_Grid.InsertColumn(0, L"类型"); //添加列名
	//m_Grid.InsertColumn(1, L"线路");
	//m_Grid.InsertColumn(2, L"电话号码");
	//m_Grid.InsertColumn(3, L"备注");

}

Product_Dialog::~Product_Dialog()
{
}


BOOL Product_Dialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_Grid.InsertColumn(0, L"商品编号",LVCFMT_LEFT,80,0); //添加列名
	m_Grid.InsertColumn(1, L"商品名称",LVCFMT_LEFT, 80, 1);
	m_Grid.InsertColumn(2, L"电话号码", LVCFMT_LEFT, 80, 2);
	m_Grid.InsertColumn(3, L"商品类别", LVCFMT_LEFT, 80, 3);
	m_Grid.InsertColumn(4, L"商品单位", LVCFMT_LEFT, 80, 4);
	m_Grid.InsertColumn(5, L"条形码", LVCFMT_LEFT, 80, 5);
	m_Grid.InsertColumn(6, L"进货价格", LVCFMT_LEFT, 80, 6);
	m_Grid.InsertColumn(7, L"销售价格", LVCFMT_LEFT, 80, 7);


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

	//m_Grid.SetColumnWidth(0, 120); //设置列宽
	//m_Grid.SetColumnWidth(1, 120);
	//m_Grid.SetColumnWidth(2, 180);
	//m_Grid.SetColumnWidth(3, 120);

	m_Grid.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_HEADERDRAGDROP | LVS_EX_GRIDLINES);//设置扩展格式，整行选择，网格线

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
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。  基类将自动
	// 删除该对象。  在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

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

// 注意: 我们添加 IID_IProduct_Dialog 支持
//  以支持来自 VBA 的类型安全绑定。  此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {9CBF6F31-1EB6-433D-8EFA-CB41EB94B5BF}
static const IID IID_IProduct_Dialog =
{ 0x9CBF6F31, 0x1EB6, 0x433D, { 0x8E, 0xFA, 0xCB, 0x41, 0xEB, 0x94, 0xB5, 0xBF } };

BEGIN_INTERFACE_MAP(Product_Dialog, CDialogEx)
	INTERFACE_PART(Product_Dialog, IID_IProduct_Dialog, Dispatch)
END_INTERFACE_MAP()


// Product_Dialog 消息处理程序

//添加
void Product_Dialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

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
