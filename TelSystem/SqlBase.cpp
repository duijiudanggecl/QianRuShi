#include "stdafx.h"
#include "SqlBase.h"
#include "afxdialogex.h"


SqlBase::SqlBase()
{

	if (S_OK != CoInitialize(NULL))
	{
		AfxMessageBox(L"��ʼ��COM�����!");
	}
	else
	{
		AfxMessageBox(L"��ʼ��COM��ɹ�!");
	}

}


SqlBase::~SqlBase()
{
}


bool SqlBase::ConnectSql()
{



	HRESULT hr; //���ؽ������

	try
	{
		hr = m_pConnection.CreateInstance(__uuidof(Connection)); //����Connection����
		if (SUCCEEDED(hr))

		{
			m_pConnection->ConnectionString = "Provider='sqloledb';Data Source='DE2-PC\\SQLEXPRESS';"
				"Initial Catalog='chen';Integrated Security='SSPI';";
			m_pConnection->ConnectionTimeout = 30;
			m_pConnection->Open("", "", "", adConnectUnspecified);

		}
	}

    catch (_com_error   e)
		{
			CString   errorMsg;
			errorMsg.Format(L"�������ݿ�ʧ��!\r\n������Ϣ:%s", e.ErrorMessage());			
			return false;

		}
		return true;
}



bool SqlBase::ExecuteSql(CString exesql)
{
	//"SELECT NAME from Table5"
	try
	{ 
	_variant_t RecordsAffected;
	m_pRecordset = m_pConnection->Execute((_bstr_t)exesql, &RecordsAffected, adCmdText);
	}
     catch (_com_error   e)
	{	
		return false;
	}
	return true;



}
