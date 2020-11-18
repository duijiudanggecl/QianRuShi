#pragma once
#include "stdafx.h"
#import  "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF", "adoEOF")

//数据库的操作类
class SqlBase
{
public:
	SqlBase();
	~SqlBase();
	void SqlInit();
	//bool m_init;
	_ConnectionPtr   m_pConnection;
	_RecordsetPtr m_pRecordset;
	bool ConnectSql();
	bool ExecuteSql(_bstr_t exesql);
	_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);

	void CloseSql();

	void Release();


};

