#pragma once

#include "stdafx.h"
#import  "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF", "adoEOF")
class SqlBase
{
public:
	SqlBase();
	~SqlBase();


	_ConnectionPtr   m_pConnection;
	_RecordsetPtr m_pRecordset; 

	bool ConnectSql();

	bool ExecuteSql(CString exesql);

};

