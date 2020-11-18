#include "stdafx.h"
#include "SqlBase.h"


SqlBase::SqlBase()
{
	
	

	
}

void SqlBase::SqlInit()
{
	if (S_OK != CoInitialize(NULL))
	{
		AfxMessageBox(L"初始化COM库错误!");
	}
	else
	{
		AfxMessageBox(L"初始化COM库成功!");
	}


}

SqlBase::~SqlBase()
{
	::CoUninitialize();
}


bool SqlBase::ConnectSql()
{
	HRESULT hr; //返回结果变量
	try
	{
		hr = m_pConnection.CreateInstance(__uuidof(Connection)); //创建Connection对象
		if (SUCCEEDED(hr))
		{
			m_pConnection->ConnectionString = "Provider='sqloledb';Data Source='DE2-PC\\SQLEXPRESS';"
				"Initial Catalog='SuperMarket';Integrated Security='SSPI';";
			m_pConnection->ConnectionTimeout = 30;
			m_pConnection->Open("", "", "", adConnectUnspecified);

			//AfxMessageBox(L"连接数据库成功!");
		}
	}
	catch (_com_error   e)
	{
		CString   errorMsg;
		errorMsg.Format(L"连接数据库失败!\r\n错误信息:%s", e.ErrorMessage());
		return false;
	}
	return true;
}


bool SqlBase::ExecuteSql(_bstr_t exesql)   //执行SQL语句
{
	//"SELECT NAME from Table5"
	try
	{
		if (m_pConnection == NULL)
			ConnectSql();
		_variant_t RecordsAffected;
		//m_pRecordset = m_pConnection->Execute((_bstr_t)exesql, &RecordsAffected, adCmdText);
		m_pConnection->Execute((_bstr_t)exesql, &RecordsAffected, adCmdText);
		return true;
	}
	catch (_com_error  e)
	{
		return false;
	}
	

}


_RecordsetPtr& SqlBase::GetRecordSet(_bstr_t bstrSQL)  //返回记录集
{
	try
	{
		if (m_pConnection == NULL)
			ConnectSql();
		m_pRecordset.CreateInstance(_uuidof(Recordset));
		m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
		//m_pRecordset.CreateInstance(__uuidof(Recordset));
		//m_pRecordset->Open("SELECT * FROM STUDENT",
			//theApp.m_pConnection.GetInterfacePtr(),
			//adOpenDynamic,
			//adLockOptimistic,
			//adCmdText);
//  m_ListCtrl.SetItemText(i,0, (char *) (bstr_t)pRecordset->GetCollect("id"));
	 }
	catch (_com_error  e)
	{ 
		CString   errorMsg;
		errorMsg.Format(L"错误信息:%s", e.ErrorMessage());
	
	}
		return m_pRecordset;


}

 void SqlBase::CloseSql() //关闭连接
{

	 if (m_pConnection != NULL)
	m_pConnection->Close();
	 m_pConnection = NULL;
}

 void SqlBase::Release()  //释放资源
 {
	 if (m_pRecordset != NULL)
	 m_pRecordset->Close();
	// RstADOBind2->Release();
 
 }