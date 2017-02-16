
// SDKExampleDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "SDKExample.h"
#include "SDKExampleDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define TIMER_GET_SYS_INFO    0x10001
#define MAX_COLOR_VALUE		  255
#define EFFECT_NUM			  12
#define DEVICE_NUM			  5

CString g_DeviceName[DEVICE_NUM] = {_T("MasterKeys Pro L"), _T("MasterKeys Pro S"), _T("MasterKeys Pro L White"), _T("MasterKeys Pro M White"), _T("MasterMouse Pro L")};


CString g_EffectName[EFFECT_NUM] = {_T("Full ON"), _T("Breathing"), _T("Color Cycle"), _T("Single Key"), _T("Wave Color"), _T("Ripple"),
							 _T("Cross Mode"), _T("Rain Drop"), _T("Star Effect"), _T("Game Snake"), _T("Customization"), _T("Indicator")};

EFF_INDEX  g_EffectMap[EFFECT_NUM] = {EFF_FULL_ON, EFF_BREATH, EFF_BREATH_CYCLE, EFF_SINGLE, EFF_WAVE,
									 EFF_RIPPLE, EFF_CROSS, EFF_RAIN, EFF_STAR, EFF_SNAKE, EFF_REC, EFF_INDICATOR};

CSDKExampleDlg * g_expDlg = NULL;


// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 對話方塊資料
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSDKExampleDlg 對話方塊




CSDKExampleDlg::CSDKExampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSDKExampleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bChange = false;
	m_bLedControl = false;
	g_expDlg = this;
}

void CSDKExampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ROW, m_cbRow);
	DDX_Control(pDX, IDC_COMBO_COLUMN, m_cbColumn);
	DDX_Control(pDX, IDC_EDIT_R, m_edR);
	DDX_Control(pDX, IDC_EDIT_G, m_edG);
	DDX_Control(pDX, IDC_EDIT_B, m_edB);
	DDX_Control(pDX, IDC_COMBO_EFFECT, m_cbEffectList);
	DDX_Control(pDX, IDC_EDIT_R2, m_edR2);
	DDX_Control(pDX, IDC_EDIT_G2, m_edG2);
	DDX_Control(pDX, IDC_EDIT_B2, m_edB2);
	DDX_Control(pDX, IDC_COMBO_DEVICE, m_cbDevList);
	DDX_Control(pDX, IDC_BUTTON_DEVICE_SET, m_DevSet);
}

BEGIN_MESSAGE_MAP(CSDKExampleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_ROW, &CSDKExampleDlg::OnCbnSelchangeKey)
	ON_CBN_SELCHANGE(IDC_COMBO_COLUMN, &CSDKExampleDlg::OnCbnSelchangeKey)

	ON_EN_CHANGE(IDC_EDIT_R, &CSDKExampleDlg::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_G, &CSDKExampleDlg::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT_B, &CSDKExampleDlg::OnEnChangeEdit)
	ON_BN_CLICKED(IDC_BUTTON_SINGLE_KEY, &CSDKExampleDlg::OnBnClickedButtonSingleKey)
	ON_BN_CLICKED(IDC_BUTTON_ALL_KEY, &CSDKExampleDlg::OnBnClickedButtonAllKey)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_LED_CONTROL, &CSDKExampleDlg::OnBnClickedButtonLedControl)
	ON_BN_CLICKED(IDC_BUTTON_EFFECT_SET, &CSDKExampleDlg::OnBnClickedButtonEffectSet)
	ON_BN_CLICKED(IDC_BUTTON_SET_ONE_COLOR, &CSDKExampleDlg::OnBnClickedButtonSetOneColor)
	ON_BN_CLICKED(IDC_BUTTON_DEVICE_SET, &CSDKExampleDlg::OnBnClickedButtonDeviceSet)
	ON_BN_CLICKED(IDC_BUTTON_GET_LAYOUT, &CSDKExampleDlg::OnBnClickedButtonGetLayout)
	ON_BN_CLICKED(IDC_CHECK_KEY_EFF, &CSDKExampleDlg::OnBnClickedCheckKeyEff)
END_MESSAGE_MAP()


// CSDKExampleDlg 訊息處理常式

BOOL CSDKExampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示


	// 初始化基本元件與設定
	memset(&m_colorMatrix, 0x00, sizeof(m_colorMatrix));   // 將所有按鍵顏色設為黑色

	CString csTemp;

	for (int iIndex = 0; iIndex < MAX_LED_ROW; iIndex++)
	{
		csTemp.Format(_T("%d"), iIndex );
		m_cbRow.AddString(csTemp);
	}
	m_cbRow.SetCurSel(0);

	for (int iIndex = 0; iIndex < MAX_LED_COLUMN; iIndex++)
	{
		csTemp.Format(_T("%d"), iIndex );
		m_cbColumn.AddString(csTemp);
	}
	m_cbColumn.SetCurSel(0);

	m_edR.SetLimitText(3);
	m_edG.SetLimitText(3);
	m_edB.SetLimitText(3);

	m_edR2.SetLimitText(3);
	m_edG2.SetLimitText(3);
	m_edB2.SetLimitText(3);

	m_edR2.SetWindowText(_T("0"));
	m_edG2.SetWindowText(_T("0"));
	m_edB2.SetWindowText(_T("0"));

	for (int iDevice = 0; iDevice < DEVICE_NUM; iDevice++)
	{
		m_cbDevList.AddString(g_DeviceName[iDevice]);
	}
	m_cbDevList.SetCurSel(0);


	for (int iEffect = 0; iEffect < EFFECT_NUM; iEffect++)
	{
		m_cbEffectList.AddString(g_EffectName[iEffect]);
	}
	m_cbEffectList.SetCurSel(0);

	OnCbnSelchangeKey();
	OnBnClickedButtonLedControl();

	SetTimer(TIMER_GET_SYS_INFO, 500, NULL);   // 半秒抓取一次 系統資訊

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}


void CSDKExampleDlg::OnDestroy()
{
	CDialog::OnDestroy();
	EnableLedControl(false);
	// TODO: 在此加入您的訊息處理常式程式碼
}


void CSDKExampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CSDKExampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CSDKExampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSDKExampleDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	switch(nIDEvent)
	{
	case TIMER_GET_SYS_INFO:
		GetSysInfo();
		break;
	}
	CDialog::OnTimer(nIDEvent);
}


void CSDKExampleDlg::GetSysInfo()
{
	//  取得系統相關資訊並設到控制項上
	CString csInfo = GetNowTime();
	GetDlgItem(IDC_EDIT_SYS_TIME)->SetWindowText(csInfo);

	LONG lCPUUsage =  GetNowCPUUsage();
	csInfo.Format(_T("%d"), lCPUUsage);
	GetDlgItem(IDC_EDIT_CPU_USAGE)->SetWindowText(csInfo);

	DWORD dwRamUsage =  GetRamUsage();
	csInfo.Format(_T("%d"), dwRamUsage);
	GetDlgItem(IDC_EDIT_RAM_USGAE)->SetWindowText(csInfo);

	float fPeekValue = GetNowVolumePeekValue();
	csInfo.Format(_T("%d"), int (fPeekValue * 100));
	GetDlgItem(IDC_EDIT_VOLUME_PEEK)->SetWindowText(csInfo);
}


void CSDKExampleDlg::OnBnClickedButtonDeviceSet()
{
	// TODO: 在此加入控制項告知處理常式程式碼

	//清除按鍵回呼
	CButton * pCheck = (CButton*) GetDlgItem(IDC_CHECK_KEY_EFF);
	pCheck->SetCheck(FALSE); 
	OnBnClickedCheckKeyEff();


	DEVICE_INDEX devIndex = (DEVICE_INDEX)m_cbDevList.GetCurSel();
	SetControlDevice(devIndex);
	EnableLedControl(m_bLedControl);
	if (m_bLedControl)
		RefreshLed(true);
}


void CSDKExampleDlg::OnBnClickedButtonLedControl()
{
	m_bLedControl = !m_bLedControl;
	EnableLedControl(m_bLedControl);
	if (m_bLedControl)
		RefreshLed(true);

	if (m_bLedControl == true)
		GetDlgItem(IDC_BUTTON_LED_CONTROL)->SetWindowText(_T("Disable"));
	else
		GetDlgItem(IDC_BUTTON_LED_CONTROL)->SetWindowText(_T("Enable"));



	GetDlgItem(IDC_BUTTON_EFFECT_SET)->EnableWindow(!m_bLedControl);

	GetDlgItem(IDC_BUTTON_SINGLE_KEY)->EnableWindow(m_bLedControl);
	GetDlgItem(IDC_BUTTON_ALL_KEY)->EnableWindow(m_bLedControl);
	GetDlgItem(IDC_BUTTON_SET_ONE_COLOR)->EnableWindow(m_bLedControl);


}

void CSDKExampleDlg::OnBnClickedButtonEffectSet()
{
	int iCurSel = m_cbEffectList.GetCurSel();

	EFF_INDEX effIndex = g_EffectMap[iCurSel];
	BOOL bSuccess = SwitchLedEffect(effIndex);
	if (bSuccess == FALSE)
	{
		AfxMessageBox(_T("No Effect or Fail"));
	}

}


void CSDKExampleDlg::OnCbnSelchangeKey()
{
	//  將當前選擇的按鍵顏色 設到 編輯方塊上

	m_bChange = true;
	int iRow = m_cbRow.GetCurSel();
	int iColumn = m_cbColumn.GetCurSel();
	const KEY_COLOR & keyColor = m_colorMatrix.KeyColor[iRow][iColumn];

	CString csColor;
	csColor.Format(_T("%d"), keyColor.r);
	m_edR.SetWindowText(csColor);

	csColor.Format(_T("%d"), keyColor.g);
	m_edG.SetWindowText(csColor);

	csColor.Format(_T("%d"), keyColor.b);
	m_edB.SetWindowText(csColor);

	m_bChange = false;

}

void CSDKExampleDlg::OnEnChangeEdit()
{
	//  將按鍵顏色 有變更時 將值寫道顏色陣列結構中
	if (m_bChange == true)
		return ;

	int iRow = m_cbRow.GetCurSel();
	int iColumn = m_cbColumn.GetCurSel();
	KEY_COLOR & keyColor = m_colorMatrix.KeyColor[iRow][iColumn];


	keyColor.r = GetColorValue(&m_edR);
	keyColor.g = GetColorValue(&m_edG);
	keyColor.b = GetColorValue(&m_edB);

}

BYTE CSDKExampleDlg::GetColorValue(CEdit * pEdit)
{
	BYTE byColor = 0x00;

	CString csColor;
	pEdit->GetWindowText(csColor);

	if (_ttoi(csColor) > MAX_COLOR_VALUE)
		byColor = MAX_COLOR_VALUE;
	else
		byColor = _ttoi(csColor);
	return byColor;
}

void CSDKExampleDlg::OnBnClickedButtonSingleKey()
{

	int iRow = m_cbRow.GetCurSel() ;
	int iColumn = m_cbColumn.GetCurSel() ;
	KEY_COLOR & keyColor = m_colorMatrix.KeyColor[iRow][iColumn];

	bool bSuccess = SetLedColor(iRow , iColumn , keyColor.r, keyColor.g, keyColor.b);
	if (bSuccess == false)
		MessageBox(_T("Set Single Key Fail!!"), _T("Warn"), MB_OK);

}

void CSDKExampleDlg::OnBnClickedButtonAllKey()
{
	bool bSuccess = SetAllLedColor(m_colorMatrix);
	if (bSuccess == false)
		MessageBox(_T("Set All Key Fail!!"), _T("Warn"), MB_OK);
}




void CSDKExampleDlg::OnBnClickedButtonSetOneColor()
{
	CString csValue;
	BYTE r = GetColorValue(&m_edR2);
	BYTE g = GetColorValue(&m_edG2);
	BYTE b = GetColorValue(&m_edB2);

	bool bSuccess = SetFullLedColor( r, g, b);
	if (bSuccess == false)
		MessageBox(_T("SetFullLedColor Fail!!"), _T("Warn"), MB_OK);
}

void CSDKExampleDlg::OnBnClickedButtonGetLayout()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CEdit * pEditLayout = (CEdit *) GetDlgItem(IDC_EDIT_LAYOUT);

	LAYOUT_KEYBOARD layout = GetDeviceLayout();
	if (layout == LAYOUT_UNINIT)
		pEditLayout->SetWindowText(_T("Unplug"));
	else if (layout == LAYOUT_US)
		pEditLayout->SetWindowText(_T("US"));
	else if (layout == LAYOUT_EU)
		pEditLayout->SetWindowText(_T("EU"));

}


void CALLBACK KeyCallBack(int iRow, int iColumn, bool bPressed)
{
	if (g_expDlg->m_bLedControl == true)
	{

		DEVICE_INDEX devIndex = (DEVICE_INDEX)g_expDlg->m_cbDevList.GetCurSel();
		if (devIndex == DEV_MMouse_L)
			if (iColumn  > 3)
				iColumn = 3;
		if (bPressed)
			SetLedColor(iRow, iColumn, 255, 0 ,0);
		else
			SetLedColor(iRow, iColumn, 0, 0 ,0);
		//Sleep(1);
	}
}

void CSDKExampleDlg::OnBnClickedCheckKeyEff()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CButton * pCheck = (CButton*) GetDlgItem(IDC_CHECK_KEY_EFF);
	if (pCheck->GetCheck())
	{
		SetKeyCallBack(KeyCallBack);
		EnableKeyInterrupt(true);
	}
	else
	{
		SetKeyCallBack(NULL);
		EnableKeyInterrupt(false);
	}
}

BOOL CSDKExampleDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
    if (pMsg-> message == WM_KEYDOWN)
    {
		CButton * pCheck = (CButton*) GetDlgItem(IDC_CHECK_KEY_EFF);
		if (pCheck->GetCheck())
		{
			return TRUE;
		}
    }

	return CDialog::PreTranslateMessage(pMsg);
}
