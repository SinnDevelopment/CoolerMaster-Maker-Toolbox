
// SDKExample.h : PROJECT_NAME ���ε{�����D�n���Y��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"		// �D�n�Ÿ�


// CSDKExampleApp:
// �аѾ\��@�����O�� SDKExample.cpp
//

class CSDKExampleApp : public CWinAppEx
{
public:
	CSDKExampleApp();

// �мg
	public:
	virtual BOOL InitInstance();

// �{���X��@

	DECLARE_MESSAGE_MAP()
};

extern CSDKExampleApp theApp;