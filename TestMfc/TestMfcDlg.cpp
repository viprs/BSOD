
// TestMfcDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestMfc.h"
#include "TestMfcDlg.h"
#include "ToolHelp.h"
#include   <afxtempl.h>
#include <WinNT.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 自定义结构，保存打开句柄的的信息
typedef struct _UNFILE_INFO {
	int nFileType;
	DWORD dwHandle;
	char *strFileName;
} UNFILE_INFO, *PUNFILE_INFO;
typedef struct _SYSTEM_HANDLE_INFORMATION {
	ULONG   ProcessId;
	UCHAR   ObjectTypeNumber;
	UCHAR   Flags;
	USHORT   Handle;
	PVOID   Object;
	ACCESS_MASK   GrantedAccess;
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

//
// System Information Classes.
//

typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemBasicInformation,
	SystemProcessorInformation,             // obsolete...delete
	SystemPerformanceInformation,
	SystemTimeOfDayInformation,
	SystemPathInformation,
	SystemProcessInformation,
	SystemCallCountInformation,
	SystemDeviceInformation,
	SystemProcessorPerformanceInformation,
	SystemFlagsInformation,
	SystemCallTimeInformation,
	SystemModuleInformation,
	SystemLocksInformation,
	SystemStackTraceInformation,
	SystemPagedPoolInformation,
	SystemNonPagedPoolInformation,
	SystemHandleInformation,
	SystemObjectInformation,
	SystemPageFileInformation,
	SystemVdmInstemulInformation,
	SystemVdmBopInformation,
	SystemFileCacheInformation,
	SystemPoolTagInformation,
	SystemInterruptInformation,
	SystemDpcBehaviorInformation,
	SystemFullMemoryInformation,
	SystemLoadGdiDriverInformation,
	SystemUnloadGdiDriverInformation,
	SystemTimeAdjustmentInformation,
	SystemSummaryMemoryInformation,
	SystemMirrorMemoryInformation,
	SystemPerformanceTraceInformation,
	SystemObsolete0,
	SystemExceptionInformation,
	SystemCrashDumpStateInformation,
	SystemKernelDebuggerInformation,
	SystemContextSwitchInformation,
	SystemRegistryQuotaInformation,
	SystemExtendServiceTableInformation,
	SystemPrioritySeperation,
	SystemVerifierAddDriverInformation,
	SystemVerifierRemoveDriverInformation,
	SystemProcessorIdleInformation,
	SystemLegacyDriverInformation,
	SystemCurrentTimeZoneInformation,
	SystemLookasideInformation,
	SystemTimeSlipNotification,
	SystemSessionCreate,
	SystemSessionDetach,
	SystemSessionInformation,
	SystemRangeStartInformation,
	SystemVerifierInformation,
	SystemVerifierThunkExtend,
	SystemSessionProcessInformation,
	SystemLoadGdiDriverInSystemSpace,
	SystemNumaProcessorMap,
	SystemPrefetcherInformation,
	SystemExtendedProcessInformation,
	SystemRecommendedSharedDataAlignment,
	SystemComPlusPackage,
	SystemNumaAvailableMemory,
	SystemProcessorPowerInformation,
	SystemEmulationBasicInformation,
	SystemEmulationProcessorInformation,
	SystemExtendedHandleInformation,
	SystemLostDelayedWriteInformation,
	SystemBigPoolInformation,
	SystemSessionPoolTagInformation,
	SystemSessionMappedViewInformation,
	SystemHotpatchInformation,
	SystemObjectSecurityMode,
	SystemWatchdogTimerHandler,
	SystemWatchdogTimerInformation,
	SystemLogicalProcessorInformation,
	SystemWow64SharedInformation,
	SystemRegisterFirmwareTableInformationHandler,
	SystemFirmwareTableInformation,
	SystemModuleInformationEx,
	SystemVerifierTriageInformation,
	SystemSuperfetchInformation,
	SystemMemoryListInformation,
	SystemFileCacheInformationEx,
	MaxSystemInfoClass  // MaxSystemInfoClass should always be the last enum
} SYSTEM_INFORMATION_CLASS;

//
// System Information Structures.
//

//typedef
//LONG 
//(WINAPI *ZwQuerySystemInformation)(
//						 IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
//						 OUT PVOID SystemInformation,
//						 IN ULONG SystemInformationLength,
//						 OUT PULONG ReturnLength OPTIONAL
//						 );

//typedef
//LONG 
//(WINAPI *ZwQueryInformationFile)(
//					   IN HANDLE   FileHandle,
//					   OUT PIO_STATUS_BLOCK   IoStatusBlock,
//					   OUT PVOID   FileInformation,
//					   IN ULONG   Length,
//					   IN FILE_INFORMATION_CLASS   FileInformationClass
//					   );
LONG
ZwQuerySystemInformation(
						 IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
						 OUT PVOID SystemInformation,
						 IN ULONG SystemInformationLength,
						 OUT PULONG ReturnLength OPTIONAL
						 )
{
	HMODULE g_hNtDLL = LoadLibrary( "ntdll.dll" );
	if ( !g_hNtDLL )
	{
		return FALSE;
	}

	ZwQuerySystemInformation = 
		(ZwQueryInformationFile)GetProcAddress( g_hNtDLL, "ZwQuerySystemInformation");
	if( ZwQuerySystemInformation == NULL)
	{
		return FALSE;
	}
}

LONG
ZwQueryInformationFile(
					   IN HANDLE   FileHandle,
					   OUT PIO_STATUS_BLOCK   IoStatusBlock,
					   OUT PVOID   FileInformation,
					   IN ULONG   Length,
					   IN FILE_INFORMATION_CLASS   FileInformationClass
					   )
{
	HMODULE g_hNtDLL = LoadLibrary( "ntdll.dll" );
	if ( !g_hNtDLL )
	{
		return FALSE;
	}
	ZwQueryInformationFile =
		(ZwQueryInformationFile)GetProcAddress( g_hNtDLL, "ZwQueryInformationFile");
	if( ZwQueryInformationFile == NULL)
	{
		return FALSE;
	}
}

PVOID GetModulePreferredBaseAddr(DWORD dwProcessId, PVOID pvModuleRemote)
{
	PVOID pvModulePreferredBaseAddr = NULL;
	IMAGE_DOS_HEADER idh;
	IMAGE_NT_HEADERS inth;

	// Read the remote module's DOS header
	Toolhelp32ReadProcessMemory(dwProcessId,
	pvModuleRemote, &idh, sizeof(idh), NULL);


	// Verify the DOS image header
	//e_magic为DOS可执行文件标记
	//#define IMAGE_DOS_SIGNATURE                 0x5A4D      // MZ   DOS文件
	//#define IMAGE_OS2_SIGNATURE                 0x454E      // NE
	//#define IMAGE_OS2_SIGNATURE_LE              0x454C      // LE
	//#define IMAGE_VXD_SIGNATURE                 0x454C      // LE
	//#define IMAGE_NT_SIGNATURE                  0x00004550  // PE00 Windows文件
	if (idh.e_magic == IMAGE_DOS_SIGNATURE) {
				// Read the remote module's NT header
				// e_lfanew 是当前DOS头（pvModuleRemote）相对实际PE头标的相对偏移量，
				Toolhelp32ReadProcessMemory(dwProcessId,
				(PBYTE) pvModuleRemote + idh.e_lfanew, &inth, sizeof(inth), NULL);
				
				// Verify the NT image header
				if (inth.Signature == IMAGE_NT_SIGNATURE) {
				// This is valid NT header, get the image's preferred base address
				pvModulePreferredBaseAddr = (PVOID) inth.OptionalHeader.ImageBase;
			}
		}
	return(pvModulePreferredBaseAddr);
}
UCHAR GetFileHandleType()
{
	HANDLE                   hFile;
	PSYSTEM_HANDLE_INFORMATION Info;
	ULONG                   r;
	UCHAR                   Result = 0;

	hFile = CreateFile("NUL", GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, 0);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		Info = GetInfoTable(SystemHandleInformation);

		if (Info)
		{
			for (r = 0; r < Info->uCount; r++)
			{
				if (Info->aSH[r].Handle == (USHORT)hFile &&
					Info->aSH[r].uIdProcess == GetCurrentProcessId())
				{
					Result = Info->aSH[r].ObjectType;
					break;
				}
			}
			HeapFree(hHeap, 0, Info);
		}
		CloseHandle(hFile);
	}
	return Result;
}
//////////////////////////////////////////////////////////////////////////
// 通过PID号取得PID打开的文件句柄信息
//////////////////////////////////////////////////////////////////////////
void
GetModules(DWORD dwProcessID, CList<PUNFILE_INFO, PUNFILE_INFO> &plsUnFileInfo)
{
	CToolhelp::EnableDebugPrivilege(TRUE);
	CToolhelp th(TH32CS_SNAPALL, dwProcessID);
	// 显示进程的详细资料
	MODULEENTRY32 me = { sizeof(me) };
	BOOL fOk = th.ModuleFirst(&me);
	for (; fOk; fOk = th.ModuleNext(&me))
	{
		PVOID pvPreferredBaseAddr = NULL;
		pvPreferredBaseAddr =GetModulePreferredBaseAddr(dwProcessID, me.modBaseAddr);
		// 取得进程模块信息
		PUNFILE_INFO pUnFileInfo = new UNFILE_INFO;
		// 模块地址
		pUnFileInfo->dwHandle = (DWORD)me.modBaseAddr;
		// 模块类型
		pUnFileInfo->nFileType = UNTYPE_DLL;
		// 模块名称
		pUnFileInfo->strFileName = new char[strlen(me.szExePath)+1];
		memset( pUnFileInfo->strFileName, 0, strlen(me.szExePath)+1);
		strcpy( pUnFileInfo->strFileName, me.szExePath);
		// 保存打开的模块信息
		plsUnFileInfo.AddTail( pUnFileInfo);
	}
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CTestMfcDlg 对话框




CTestMfcDlg::CTestMfcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestMfcDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestMfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILE, m_ListBox);
}

BEGIN_MESSAGE_MAP(CTestMfcDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CTestMfcDlg 消息处理程序

BOOL CTestMfcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	

	CStringArray   aryFileList;
	CString sTemp;
	CString sExName;
	
	TCHAR szBuffer[MAX_PATH ] = {0};
	//得到程序的路径
	GetModuleFileName (NULL, szBuffer ,sizeof(szBuffer )/sizeof(szBuffer [0]));
	CString sFilePath = szBuffer ;
	SetDlgItemText (IDC_EDIT_FILE_PATH, sFilePath);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//int nIndex = -1;
	//nIndex = sFilePath.ReverseFind(_T('.'));
	//if (nIndex != -1)
	//{
	//	sTemp = sFilePath.Mid(nIndex+1);
	//}
	//sExName = sTemp;
	//MessageBox (sExName, _T("ExName"));

	//sTemp = GetParentDirName(sFilePath);
	//MessageBox (sTemp, _T("Parent Dir Name"));

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//CString sTemp;

	//CString sDirPath(_T("E:\\Qbox\\BSOD\\Debug"));
	//sDirPath = sDirPath + _T("\\*");
	//CFileFind ff;
	//BOOL res = ff.FindFile(sDirPath);   
	//while(res)
	//{
	//	res = ff.FindNextFile();
	//	if(ff.IsDirectory() && !ff.IsDots())
	//	{
	//		GetFileList(ff.GetFilePath(),aryFileList);
	//		continue;
	//	}
	//	else if(!ff.IsDirectory() && !ff.IsDots())  
	//	{
	//		aryFileList.Add(ff.GetFilePath());
	//		m_ListBox.AddString (aryFileList[0]);
	//		UpdateData (FALSE);
	//	}

	//}
	//ff.Close();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestMfcDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestMfcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTestMfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString CFileFind::GetFilePath() const
{
	ASSERT(m_hContext != NULL);
	ASSERT_VALID(this);

	CString strResult = m_strRoot;
	LPCTSTR pszResult;
	LPCTSTR pchLast;
	pszResult = strResult;
	pchLast = _tcsdec( pszResult, pszResult+strResult.GetLength() );
	ENSURE(pchLast!=NULL);
	if ((*pchLast != _T('\\')) && (*pchLast != _T('/')))
		strResult += m_chDirSeparator;
	strResult += GetFileName();
	return strResult;
}

void CTestMfcDlg::GetFileList(CString sFloderPath,CStringArray& aryFileList)
{
	if (sFloderPath.GetLength() == 0)
	{
		return;
	}
	CString sTemp;
	CString sDirPath;
	sDirPath = sFloderPath + _T("\\*");
	CFileFind ff;
	BOOL res = ff.FindFile(sDirPath);   
	while(res)
	{
		res = ff.FindNextFile();
		if(ff.IsDirectory() && !ff.IsDots())
		{
			GetFileList(ff.GetFilePath(),aryFileList);
			continue;
		}
		else if(!ff.IsDirectory() && !ff.IsDots())  
		{
			aryFileList.Add(ff.GetFilePath());			
		}

	}
	ff.Close();
}

CString CTestMfcDlg::GetParentDirName(CString sFilePath)
{
	int nIndex = -1;
	CString sTemp;
	nIndex = sFilePath.ReverseFind(_T('\\'));
	if (nIndex != -1)
	{
		sTemp = sFilePath.Left(nIndex);
		nIndex = sTemp.ReverseFind(_T('\\'));
		if (nIndex != -1)
		{
			sTemp = sTemp.Mid(nIndex+1);
		}
	}
	return sTemp;
}