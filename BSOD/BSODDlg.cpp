// BSODDlg.cpp : ʵ���ļ�
//
//��Win32 Console Application��ѡ��Project�����һ��ѡ������ú�ѡ��Generalѡ��
//������ͻῴ�� use of MFC�е�Ĭ��ѡ��Ϊ��Use Standard Windows Libraries
//ѡ���޸�Ϊ�� Use MFC in a Shared DLL


#include "stdafx.h"
#include "BSOD.h"
#include "BSODDlg.h"
#include "Functions.h"


using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString *pm_Function_Name;
CString *pm_Function_Args;
CString *pm_Function_DllName;
CListBox *pm_Function_List;
CProgressCtrl *pm_CtrlProgress;
vector<DWORD> *pFuzz_Param;
CString sFunctionDBfile_thread;
int Fuzz_loops_thread;


void ParseDataFromFunctionDB(CString lpFileName, CString readdata);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
# the calling convention is a string that consists of characters V, D, B, P, O, Q and U only
# for N-argument function, the calling convetion string must contain exactly N characters
#
#   V   - the argument is valid and static, fuzzer does not manipulate this argument, bruteforcer skip it
#   D   - the argument is probed in range 0x00000000 - 0xFFFFFFFF  0~4294967295
#   P   - the argument is probed in range 0x00000001 - 0xFFFFFF00  1~4294967040
#   B   - the argument is probed in range 0x7FFF0001 - 0xFFFFFFFF  2147418113~4294967295
#   O   - the argument is OBJECT_ATTRIBUTES structure with ObjectName probed as D
#   Q   - the argument is OBJECT_ATTRIBUTES structure with ObjectName.Buffer probed as D
#   U   - the argument is UNICODE_STRING structure with Buffer probed as D
#
*/
DWORD
GetAnyDword()
{
	DWORD dTemp;
	srand((int)time(0));

	dTemp = (rand () * rand ()) % 0xFFFFFFFF;
	return dTemp;
	//return 333444444;
}

DWORD
GetAnyPArg()
{
	DWORD dTemp;
	srand((int)time(0));

	dTemp = (rand () * rand ()) % 0xFFFFFF00;
	return dTemp;
	//return 333444444;
}

DWORD
GetAnyBArg()
{
	DWORD dTemp;
	srand((int)time(0));

	dTemp = (rand () + 2147418113) % 0xFFFFFFFF;
	return dTemp;
	//return 333444444;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CBSODDlg �Ի���




CBSODDlg::CBSODDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBSODDlg::IDD, pParent)
	, m_Function_Input(_T("NtCreateProcess"))
	, m_Function_Name(_T(""))
	, m_Function_Args(_T(""))
	, m_Function_DllName(_T(""))
	, Fuzz_loops(1)
	, Fuzz_seed(GetAnyDword ())
	, m_Xml_Path(_T(""))
	, m_RichEdit(_T(""))
	, m_Log_Path(_T(""))
	, m_Log_Content(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBSODDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TO_TEST, m_Function_List);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_Function_Input);
	DDX_Text(pDX, IDC_EDIT_FUNCTION_NAME, m_Function_Name);
	DDX_Text(pDX, IDC_EDIT_FUNCTION_ARGS, m_Function_Args);
	DDX_Text(pDX, IDC_EDIT_FUNCTION_DLLNAME, m_Function_DllName);
	DDX_Text(pDX, IDC_EDIT_FUZZER_LOOPS, Fuzz_loops);
	DDX_Text(pDX, IDC_EDIT_SEED, Fuzz_seed);
	DDX_Control(pDX, IDC_PROGRESS_FUNCTION, m_CtrlProgress);
	DDX_Control(pDX, IDC_LIST_XML, m_List_Xml);
	DDX_Text(pDX, IDC_EDIT_XML_PATH, m_Xml_Path);
	DDX_Text(pDX, IDC_RICHEDIT_SHOW, m_RichEdit);
	DDX_Text(pDX, IDC_EDIT2, m_Log_Path);
	DDX_Text(pDX, IDC_EDIT_SHOW_LOG, m_Log_Content);
}

BEGIN_MESSAGE_MAP(CBSODDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK_ADD_FUNCTION, &CBSODDlg::OnBnClickedAddFunction)
	ON_LBN_SELCHANGE(IDC_LIST_TO_TEST, &CBSODDlg::OnLbnSelchangeListToTest)
	ON_BN_CLICKED(IDFUZZ, &CBSODDlg::OnBnClickedFuzz)
	ON_BN_CLICKED(IDC_RM_BUTTON, &CBSODDlg::OnBnClickedRmButton)
	ON_BN_CLICKED(IDC_BTN_BROWSE, &CBSODDlg::OnBnClickedBtnBrowse)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_XML, &CBSODDlg::OnLvnItemchangedListXml)
	ON_NOTIFY(NM_CLICK, IDC_LIST_XML, &CBSODDlg::OnNMClickListXml)
	ON_BN_CLICKED(IDC_BTN_XML_SAVE, &CBSODDlg::OnBnClickedBtnXmlSave)
	ON_BN_CLICKED(IDC_BTN_XML_SAVE_AS, &CBSODDlg::OnBnClickedBtnXmlSaveAs)
	ON_BN_CLICKED(IDC_BTN_XML_DEL, &CBSODDlg::OnBnClickedBtnXmlDel)
	ON_BN_CLICKED(IDC_BTN_XML_FUZZ, &CBSODDlg::OnBnClickedBtnXmlFuzz)
	ON_BN_CLICKED(IDC_BTN_DEL_LOG, &CBSODDlg::OnBnClickedBtnDelLog)
END_MESSAGE_MAP()


// CBSODDlg ��Ϣ�������

BOOL CBSODDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_List_Xml.InsertColumn (0, _T("�ļ���"), LVCFMT_LEFT, 2*8*sizeof(_T("�ļ���")));
	m_List_Xml.InsertColumn (1, _T("·��"), LVCFMT_LEFT, 3*8*sizeof(_T("·��")));
	m_List_Xml.InsertColumn (2, _T("״̬"), LVCFMT_LEFT, 8*sizeof(_T("״̬")));

	//m_List_Xml.InsertItem (0, _T("Fun1"));
	//m_List_Xml.SetItemText(0, 1, _T("C:\\functions.xml"));

	//m_List_Xml.SetItemText(0, 1, _T("C:\\fds.txt"));

	//m_List_Xml.InsertItem (1, _T("Fun2"));
	//m_List_Xml.SetItemText(1, 1, _T("jkfdsj"));
	//m_List_Xml.SetItemText(1, 1, _T("C:\\fds.txt"));

	((CButton*)GetDlgItem(IDC_CHECK_LOOPS))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_LOG))->SetCheck(TRUE);

	OnInitLogPath ();
	//MessageBox (sLogPath);
	OnInitData ();


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
BOOL CBSODDlg::OnInitData()
{
	//��ʼ��Fuzz DB file
	sFunctionDBfile = GetFilePath ();
	sFunctionDBfile += _T("functions.db");

	pm_Function_List               = &m_Function_List;
	pm_CtrlProgress                = &m_CtrlProgress;
	pFuzz_Param                    = &Fuzz_Param;
	Fuzz_loops_thread              = Fuzz_loops;
	sFunctionDBfile_thread         = sFunctionDBfile;
	pm_Function_Name               = &m_Function_Name;
	pm_Function_Args               = &m_Function_Args;
	pm_Function_DllName            = &m_Function_DllName;
	
	return TRUE;
}
CString CBSODDlg::GetFilePath()
{
	TCHAR szTMP[MAX_PATH];
	GetModuleFileName (NULL, szTMP , chDIMOF(szTMP ));
	CString sPath;
	sPath.Format (_T("%s"), szTMP);

	int nIndex = -1;
	nIndex = sPath.ReverseFind (_T( '\\'));
	if (nIndex != -1)
	{
		sPath = sPath.Left (nIndex+1);
	}

	return sPath;
}
//��ʼ��log�ļ���
void CBSODDlg::OnInitLogPath()
{
	CString sLogPath;
	sLogPath = GetFilePath ();

	CTime    tm    = CTime::GetCurrentTime(); 
	int    year    = tm.GetYear();
	int    mouth   = tm.GetMonth();
	int      day   = tm.GetDay();
	CString SysDate = _T("");
	SysDate.Format(_T("%d-%d-%d"),year,mouth,day);

	m_Log_Path = sLogPath + _T("BSODhook") + SysDate + _T(".log");
	UpdateData (FALSE);
	//WriteLog (m_Log_Path, _T("fuck uuuuuu"));
	//WriteLog (m_Log_Path, _T("�ص��еͶ�"));
	//WriteLog (m_Log_Path, _T("kkk"));
}
void CBSODDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

//дlog����
int CBSODDlg::WriteLog(LPCTSTR lpFileName, LPCTSTR writedata)
{
	CString Temp;
	HANDLE hFile;
	DWORD  dwBytesWritten;

	hFile = CreateFile(lpFileName,             // file name
		FILE_APPEND_DATA,               // append data to a file
		0,                              // do not share
		NULL,                           // default security
		OPEN_ALWAYS,                    // existing file only
		FILE_ATTRIBUTE_NORMAL,          // normal file
		NULL);                          // no template
	if (INVALID_HANDLE_VALUE != hFile)
	{
		WORD Head=0xfeff;
		WriteFile (hFile, &Head, sizeof(WORD), &dwBytesWritten, NULL);
		Temp.Format (L"%s\r\n", writedata);
		WriteFile (hFile, Temp, lstrlen(Temp) * sizeof(TCHAR), &dwBytesWritten, NULL);
		CloseHandle (hFile);
	}
	else
	{
		MessageBox(_T("Could Not create file!!"));
	}

	m_Log_Content += writedata;
	m_Log_Content += _T("\r\n");
	UpdateData (FALSE);
	//CStdioFile sLogFile;
	//sLogFile.Open (m_Log_Path, CFile::modeRead);
	//CString strRead, strTemp;

	//while ( sLogFile.ReadString (strTemp) )
	//{
	//	strTemp += _T("\n");
	//	strRead += strTemp;
	//}
	//m_Log_Content = strRead;
	//sLogFile.Close ();
	//UpdateData(FALSE);


	return TRUE;
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBSODDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CBSODDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString GetFileName (CString sFilePath )
{
	int nIndex = -1;
	CString sTemp;
	nIndex = sFilePath.ReverseFind (_T( '\\'));
	if (nIndex != -1)
	{
		sTemp = sFilePath.Mid (nIndex+1);
	}
	return sTemp;
}


/**************************************************************
* ģ������CheckExistInFunctionsDB()
* ���ܣ�ƥ���ļ����׵����Ƿ����ض��ַ�����ͬ
* ���ܲ������ļ��� lpFileName, Ҫ���ҵ��ַ��� readdata
* ����ֵ���Ƿ���� (TRUE OR FALSE )
* ԭ��
* ��ȡ��ʽ: ���ж�ȡ, ���ж����ַ�����, ��֮���ûس���������
* ƥ���׸��ַ���������Ƕ�ȡ��һ��
* �����ͬ�ٷŵ�CSting�ƥ�������ַ����Ƿ���ͬ
**************************************************************/
//BOOL CheckExistInFunctionsDB(LPWSTR lpFileName, LPWSTR readdata)

BOOL CheckExistInFunctionsDB(CString lpFileName, CString readdata)
{
	ifstream fin(lpFileName);
	const int LINE_LENGTH = 200;
	char str[LINE_LENGTH];

	CString In_readdata(readdata);
	CString Function_Name;
	CString Function_Args;
	CString Function_DllName;
	while( fin.getline(str, LINE_LENGTH) )
	{
		if ('#' == str[0] || '\0' == str[0] || ' ' == str[0])
		{
			continue;
		} 
		else
		{
			//get the three parts of the string
			CString Function_Temp(str);
			//���ַ�����ȡ�ú��������Ƚ�
			//��Function_Temp�д�NtAdjustPrivilegesToken DDDDDD ntdll.dll��ȡ��NtAdjustPrivilegesToken����ɾ��
			int b = Function_Temp.Find (' ');
			Function_Name = Function_Temp.Left (b);
			if (In_readdata == Function_Name)
			{
				return TRUE;
			} 
			else
			{
				continue;
			}
		}
	}
	return FALSE;
}
BOOL CBSODDlg::CheckExistInFuzzList(CString m_Funtion_Input)
{

	CString cTemp;
	vector<CString> vFuzzList;
	int nIndex = m_Function_List.GetCount();
	for (int j=0; j<nIndex;j++)
	{
		m_Function_List.GetText (j, cTemp);
		vFuzzList.push_back (cTemp);
	}
	for (int j=0;j<nIndex;j++)
	{
		m_Function_List.GetText (j, cTemp);
		if (m_Funtion_Input == cTemp)
		{
			MessageBox(_T("�ú�������ӣ�"), _T("����"));
			return FALSE;
		}
	}
	return TRUE;
}


/**************************************************************
* ģ������OnBnClickedAddFunction()
* ���ܣ���Ӻ�����Fuzz_List�У��˴���һ���жϣ��Ƿ���functions.db�У�
*       ����Ǿ����
* ���ܲ�����NULL
* ����ֵ��NULL
* ���ڣ�2012-05-18���

**************************************************************/

void CBSODDlg::OnBnClickedAddFunction()
{
	UpdateData (TRUE);

	//GetDlgItem (IDC_FUNCTION_NAME)->SetWindowText (_T("sdfsf"));

	//CString lpFileName =_T("functions.db");
	//CString readdata = m_Function_Input;

	//���!��������function.db�ļ��в�����ӽ�ȥ������FuzzList��û�еĺ���
	//if (CheckExistInFunctionsDB (FUNCTION_DB_FILE, ))
	//if (CheckExistInFunctionsDB (_T("functions.db"),_T("NtAdjustPrivilegesToken")))

	if (CheckExistInFunctionsDB (sFunctionDBfile,m_Function_Input))
	{
		if (CheckExistInFuzzList(m_Function_Input))
		{
			m_Function_List.AddString (m_Function_Input);
			WriteLog (m_Log_Path, m_Function_Input+_T(" found."));
			MessageBox (_T("��ӳɹ���"));
		}

	}
	else
	{
		MessageBox (_T("�޴˺��������������룡"));
	}
	
	
	// TODO: Add your control notification handler code here
}

//ģ������ParseDataFromFunctionDB(LPCTSTR lpFileName, LPCTSTR readdata)
//��ȡ��ʽ: ���ж�ȡ, ���ж����ַ�����, ��֮���ûس���������
//If we were interested in preserving whitespace, 
//we could read the file in Line-By-Line using the I/O getline() function.
//void CBSODDlg::ParseDataFromFunctionDB(LPCTSTR lpFileName, LPCTSTR readdata)
void ParseDataFromFunctionDB(CString lpFileName, CString readdata)
{
	ifstream fin(lpFileName);
	CString In_readdata(readdata);
	const int LINE_LENGTH = 200;
	char str[LINE_LENGTH];

	while( fin.getline(str, LINE_LENGTH) )
	{
		if ('#' == str[0] || '\0' == str[0] || ' ' == str[0])
		{
			continue;
		} 
		else
		{
			//get the three parts of the string
			CString Function_Temp(str);
			//���ַ�����ȡ�ú�����
			//��Function_Temp�д�NtAdjustPrivilegesToken DDDDDD ntdll.dll��ȡ��NtAdjustPrivilegesToken����ɾ��
			int b = Function_Temp.Find (' ');

			if (In_readdata == Function_Temp.Left (b))
			{
				//b = Function_Temp.Find (' ');
				*pm_Function_Name = Function_Temp.Left (b);
				Function_Temp.Delete (0, b+1);

				//��Function_Temp�д�DDDDDD ntdll.dll��ȡ��DDDDDD����ɾ��
				b = Function_Temp.Find (' ');
				*pm_Function_Args = Function_Temp.Left (b);
				Function_Temp.Delete (0, b+1);

				//����Function_Temp�е�ntdll.dll��ֵ��Function_DllName
				*pm_Function_DllName = Function_Temp;
				//	UpdateData (FALSE);
				break;
			}

		}
	}
}
/**************************************************************
* ģ������OnLbnSelchangeListToTest()
* ���ܣ������Fuzz_List�е�һ�����������ѯfunctions.db�У���
*       ��������������ģ������ʾ�ڿؼ���
* ���ܲ�����NULL
* ����ֵ��NULL
* ���ڣ�2012-05-18���

**************************************************************/

void CBSODDlg::OnLbnSelchangeListToTest()
{
	CString temp;
	// TODO: Add your control notification handler code here
	int nIndex = m_Function_List.GetCurSel();
	m_Function_List.GetText (nIndex, temp);
	//MessageBox (temp);
	//ParseDataFromFunctionDB(_T("functions.db"),temp);
	ParseDataFromFunctionDB(sFunctionDBfile,temp);

	UpdateData (FALSE);
	//ѡ��ListBox�б��У���ǰѡ�����һ��Ŀ
	//int nCount = m_Function_List.GetCount();
	//if ((nIndex != LB_ERR) && (nCount > 1))
	//{
	//	if (++nIndex < nCount)
	//		m_Function_List.SetCurSel(nIndex);
	//	else
	//		m_Function_List.SetCurSel(0);
	//}


}




//һ����һЩ��������

VOID
RtlInitUnicodeString_U(OUT PUNICODE_STRING DestinationString,
					   IN PWSTR SourceString)
{
	USHORT Length = 0;
	DestinationString->Length = 0;
	DestinationString->Buffer = SourceString;

	if (SourceString) {
		while (*SourceString++) {
			Length += sizeof(*SourceString);
		}
		DestinationString->Length = Length;
		DestinationString->MaximumLength = Length+(USHORT)sizeof(UNICODE_NULL);
	}
	else {
		DestinationString->MaximumLength = 0;
	}
}




//������һЩ��������
/**************************************************************
* ģ������
* ���ܣ����Go�����Ȼ�ȡCListBox�еĺ�������¼��һ��Vector���棬
*       ��ָ��ȡ�����������еĴ���Ƚϣ�ѡ��ִ��һ�λ򼸶δ���
* ���ܲ�����NULL
* ����ֵ��NULL
* ���ڣ�2012-05-18���

**************************************************************/

void MainFuzz()
{
	CString cTemp;
	vector<CString> vFuzzList;
	int nIndex = pm_Function_List->GetCount();
	pm_CtrlProgress->SetRange (0,nIndex);
	pm_CtrlProgress->SetPos(0);
	for (int j=0; j<nIndex;j++)
	{
		pm_Function_List->GetText (j, cTemp);
		vFuzzList.push_back (cTemp);
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//POBJECT_ATTRIBUTES poa;
	//PIO_STATUS_BLOCK pio;
	//PUNICODE_STRING pus;

	for (unsigned int i=0; i<vFuzzList.size(); i++)
	{
		OBJECT_ATTRIBUTES oa = {0};
		UNICODE_STRING uniName = {0};
		RtlInitUnicodeString_U (&uniName, TEST_FILE);
		InitializeObjectAttributes(&oa, (PUNICODE_STRING)GetAnyDword (), OBJ_CASE_INSENSITIVE, 0, 0);
		
		
		ParseDataFromFunctionDB(sFunctionDBfile_thread, vFuzzList.at (i));

		for (int loop=0; loop < Fuzz_loops_thread; loop++)//����Fuzz����
		{
			CString Temp(*pm_Function_Args);
			while(Temp != _T(""))
			{
				if (_T("v") == Temp.Left (1) || _T("V") == Temp.Left (1))
				{
					pFuzz_Param->push_back (0);
					Temp.Delete (0, 1);
					continue;
				}
				if (_T("d") == Temp.Left (1) || _T("D") == Temp.Left (1))
				{
					pFuzz_Param->push_back (GetAnyDword ());
					Temp.Delete (0, 1);
					continue;
				}
				if (_T("p") == Temp.Left (1) || _T("P") == Temp.Left (1))
				{
					pFuzz_Param->push_back (GetAnyPArg ());
					Temp.Delete (0, 1);
					continue;
				}
				if (_T("b") == Temp.Left (1) || _T("B") == Temp.Left (1))
				{
					pFuzz_Param->push_back (GetAnyBArg ());
					Temp.Delete (0, 1);
					continue;
				}
				if (_T("o") == Temp.Left (1) || _T("O") == Temp.Left (1))
				{
					pFuzz_Param->push_back (GetAnyDword ());
					Temp.Delete (0, 1);
					continue;
				}
				if (_T("q") == Temp.Left (1) || _T("Q") == Temp.Left (1))
				{
					Temp.Delete (0, 1);
					continue;
				}
				if (_T("u") == Temp.Left (1) || _T("U") == Temp.Left (1))
				{
					Temp.Delete (0, 1);
					continue;
				}
			}

			DWORD dTemp = 0;
			int ESP_Size = 0;


			for (int k=pFuzz_Param->size ()-1;k>=0;k--)
			{
				dTemp = pFuzz_Param->at (k);
				ESP_Size += sizeof(dTemp);
				__asm{
					mov edx,dTemp;
					push edx;
				}
			}
			if (_T("NtCreateFile") == vFuzzList.at (i))
			{
				__asm{
					call pZwCreateFile;
					mov  eax ,ESP_Size;
					add esp,eax;
				}
			}
			if (_T("NtAdjustPrivilegesToken") == vFuzzList.at (i))
			{
				__asm{
					call pZwCreateFile;
					mov  eax ,ESP_Size;
					add esp,eax;
				}
			}
			if (_T("NtAlertThread") == vFuzzList.at (i))
			{
				__asm{
					call pZwCreateFile;
					mov  eax ,ESP_Size;
					add esp,eax;
				}
			}
			if (_T("NtCreateProcess") == vFuzzList.at (i))
			{
				__asm{
					call pZwCreateProcess;
					mov  eax ,ESP_Size;
					add esp,eax;
				}
			}
			if (_T("NtClose") == vFuzzList.at (i))
			{
				__asm{
					call pZwClose;
					mov  eax ,ESP_Size;
					add esp,eax;
				}
			}
			if (_T("NtCreateMutant") == vFuzzList.at (i))
			{
				__asm{
					call pZwCreateMutant;
					mov  eax ,ESP_Size;
					add esp,eax;
				}
			}
			if (_T("NtCreatePort") == vFuzzList.at (i))
			{
				__asm{
					call pZwCreatePort;
					mov  eax ,ESP_Size;
					add esp,eax;
				}
			}
		}
		pm_CtrlProgress->SetPos (i+1);
	}//end of FuzzList 
	//pZwCreateFile ((PHANDLE)GetAnyDword (), GetAnyDword (), &oa, (PIO_STATUS_BLOCK)GetAnyDword (), (PLARGE_INTEGER)GetAnyDword (), GetAnyDword (),
	//	GetAnyDword (), GetAnyDword (), GetAnyDword (), (PVOID)GetAnyDword (), GetAnyDword ());




	//delete poa;
	//delete pio;
	//delete pus;
}
void CBSODDlg::OnBnClickedFuzz()
{
	UpdateData (TRUE);
	OnInitData ();

	CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)MainFuzz,NULL,0,NULL);
	Msg("Fuzz ���Խ�����");

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//���ܣ����Clist���еġ�������Ŀ��������ÿһ���������ŵ�Vector��
/*
	CString cTemp;
	vector<CString> vFuzzList;
	int nIndex = m_Function_List.GetCount();
	m_CtrlProgress.SetRange (0,nIndex);
	m_CtrlProgress.SetPos(0);
	for (int j=0; j<nIndex;j++)
	{
		m_Function_List.GetText (j, cTemp);
		vFuzzList.push_back (cTemp);
	}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//POBJECT_ATTRIBUTES poa;
	//PIO_STATUS_BLOCK pio;
	//PUNICODE_STRING pus;

	for (unsigned int i=0; i<vFuzzList.size(); i++)
	{
		OBJECT_ATTRIBUTES oa = {0};
		UNICODE_STRING uniName = {0};
		RtlInitUnicodeString_U (&uniName, TEST_FILE);
		InitializeObjectAttributes(&oa, (PUNICODE_STRING)GetAnyDword (), OBJ_CASE_INSENSITIVE, 0, 0);

		ParseDataFromFunctionDB(sFunctionDBfile, vFuzzList.at (i));

		for (int loop=0; loop < Fuzz_loops; loop++)//����Fuzz����
		{
			CString Temp(m_Function_Args);
			while(Temp != _T(""))
			{
				if (_T("v") == Temp.Left (1) || _T("V") == Temp.Left (1))
				{
					Fuzz_Param.push_back (0);
					Temp.Delete (0, 1);
					continue;
				}
				if (_T("d") == Temp.Left (1) || _T("D") == Temp.Left (1))
				{
					Fuzz_Param.push_back (GetAnyDword ());
					Temp.Delete (0, 1);
					continue;
				}
				if (_T("p") == Temp.Left (1) || _T("P") == Temp.Left (1))
				{
					Fuzz_Param.push_back (GetAnyPArg ());
					Temp.Delete (0, 1);
					continue;
				}
				if (_T("b") == Temp.Left (1) || _T("B") == Temp.Left (1))
				{
					Fuzz_Param.push_back (GetAnyBArg ());
					Temp.Delete (0, 1);
					continue;
				}
				if (_T("o") == Temp.Left (1) || _T("O") == Temp.Left (1))
				{
					Fuzz_Param.push_back (GetAnyDword ());
					Temp.Delete (0, 1);
					continue;
				}
				if (_T("q") == Temp.Left (1) || _T("Q") == Temp.Left (1))
				{
					Temp.Delete (0, 1);
					continue;
				}
				if (_T("u") == Temp.Left (1) || _T("U") == Temp.Left (1))
				{
					Temp.Delete (0, 1);
					continue;
				}
			}

			DWORD dTemp = 0;
			int ESP_Size = 0;


			for (int k=Fuzz_Param.size ()-1;k>=0;k--)
			{
				dTemp = Fuzz_Param.at (k);
				ESP_Size += sizeof(dTemp);
				__asm{
					mov edx,dTemp;
					push edx;
				}
			}
			if (_T("NtCreateFile") == vFuzzList.at (i))
			{
				__asm{
					call pZwCreateFile;
					mov  eax ,ESP_Size;
					add esp,eax;
				}
			}
			if (_T("NtAdjustPrivilegesToken") == vFuzzList.at (i))
			{
				__asm{
					call pZwCreateFile;
					mov  eax ,ESP_Size;
					add esp,eax;
				}
			}
			if (_T("NtAlertThread") == vFuzzList.at (i))
			{
				__asm{
					call pZwCreateFile;
					mov  eax ,ESP_Size;
					add esp,eax;
				}
			}
			if (_T("NtCreateProcess") == vFuzzList.at (i))
			{
				__asm{
					call pZwCreateProcess;
					mov  eax ,ESP_Size;
					add esp,eax;
				}
			}
			if (_T("NtClose") == vFuzzList.at (i))
			{
				__asm{
					call pZwClose;
					mov  eax ,ESP_Size;
					add esp,eax;
				}
			}
			if (_T("NtCreateMutant") == vFuzzList.at (i))
			{
				__asm{
					call pZwCreateMutant;
					mov  eax ,ESP_Size;
					add esp,eax;
				}
			}
			if (_T("NtCreatePort") == vFuzzList.at (i))
			{
				__asm{
					call pZwCreatePort;
					mov  eax ,ESP_Size;
					add esp,eax;
				}
			}
		}
		m_CtrlProgress.SetPos (i+1);
	}//end of FuzzList 
		//pZwCreateFile ((PHANDLE)GetAnyDword (), GetAnyDword (), &oa, (PIO_STATUS_BLOCK)GetAnyDword (), (PLARGE_INTEGER)GetAnyDword (), GetAnyDword (),
		//	GetAnyDword (), GetAnyDword (), GetAnyDword (), (PVOID)GetAnyDword (), GetAnyDword ());




	//delete poa;
	//delete pio;
	//delete pus;
*/
}

/**************************************************************
* �ؼ�����OnBnClickedRmButton
* ���ܣ������ɾ���������Ȼ�ȡCListBox�е�ǰѡ�����ַ�����GetCurSel ()
*       ��DeleteString (Index)ɾ���������EditControl��ֵ
* ���ܲ�����NULL
* ����ֵ��NULL
* ���ڣ�2012-06-03���

**************************************************************/
void CBSODDlg::OnBnClickedRmButton()
{
	// TODO: Add your control notification handler code here
	int Index = m_Function_List.GetCurSel ();
	if ( LB_ERR == m_Function_List.DeleteString (Index) )
	{
		MessageBox (_T("ɾ������!"));
	}
	m_Function_Name = _T("");
	m_Function_Args = _T("");
	m_Function_DllName = _T("");

	UpdateData (FALSE);
}
CString CBSODDlg::BootOpenDialog()
{
	CString sFilePath = _T("");
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("All Files (*.*)|*.*||"), NULL);

	if (dlgFile.DoModal())
	{
		sFilePath = dlgFile.GetPathName();
	}

	return sFilePath;
}
void CBSODDlg::OnBnClickedBtnBrowse()
{
	m_Xml_Path = BootOpenDialog ();
	UpdateData (FALSE);
	
	AddXmlToList(m_Xml_Path);
	
	m_Xml_Path = _T("");
	UpdateData (FALSE);
	// TODO: Add your control notification handler code here
}
void CBSODDlg::AddXmlToList(CString m_Xml_Path)
{
	CString sFileName;
	sFileName = GetFileName (m_Xml_Path);
	//
	auto int iItemCount;
	iItemCount = m_List_Xml.GetItemCount ();
	m_List_Xml.InsertItem (iItemCount, sFileName);
	m_List_Xml.SetItemText(iItemCount, 1, m_Xml_Path);
}
void CBSODDlg::OnLvnItemchangedListXml(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

/**************************************************************
* �ؼ�����OnNMClickListXml
* ���ܣ����CListControl�е�һ���ֶΣ���ǰѡ�����ַ�������ʾ��
*       EditControl�У�����XML_Manager����ʾ������OnNMShowClickListXml()
* ���ܲ�����NULL
* ����ֵ��NULL
* ���ڣ�2012-06-05���

**************************************************************/
void CBSODDlg::OnNMClickListXml(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if ((m_ListIndex = pNMListView->iItem) != -1)
	{
		//CString str;
		//str.Format (_T("%d"), m_ListIndex);
		//MessageBox (str);
		m_Xml_Path = m_List_Xml.GetItemText (m_ListIndex, 1);
		UpdateData (FALSE);
		OnNMShowClickListXml ();
	}
	// TODO: Add your control notification handler code here

	*pResult = 0;
}
void CBSODDlg::OnNMShowClickListXml()
{
	CString Xml_Path;
	Xml_Path = m_List_Xml.GetItemText (m_ListIndex, 1);
	//MessageBox (Xml_Path);

	CStdioFile xmlFile;
	xmlFile.Open (Xml_Path, CFile::modeRead);
	CString strRead, strTemp;

	while ( xmlFile.ReadString (strTemp) )
	{
		strTemp += _T("\n");
		strRead += strTemp;
	}
	m_RichEdit = strRead;
	UpdateData(FALSE);
	xmlFile.Close ();
}
void CBSODDlg::OnBnClickedBtnXmlSave()
{
	// TODO: Add your control notification handler code here
	CString Xml_Path;
	Xml_Path = m_List_Xml.GetItemText (m_ListIndex, 1);

	CStdioFile xmlFile;
	xmlFile.Open (Xml_Path, CFile::modeReadWrite);
	UpdateData (TRUE);

	xmlFile.WriteString (m_RichEdit);
	xmlFile.Close ();
}

void CBSODDlg::OnBnClickedBtnXmlSaveAs()
{
	// TODO: Add your control notification handler code here
	//WCHAR szFilter[] = _T("�ı��ļ� (*.txt)|*.txt|��ҳ�ļ� (*.html)|*.html||");

	//CFileDialog FileDlg(false,_T("txt"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
	//	szFilter,NULL);
}

void CBSODDlg::OnBnClickedBtnXmlDel()
{
	// TODO: Add your control notification handler code here
	m_List_Xml.DeleteItem (m_ListIndex);

	//���RichEdit������
	m_Xml_Path = _T("");
	m_RichEdit = _T("");
	UpdateData (FALSE);
}
void CBSODDlg::ParseXmlData(CString m_Xml_Path)
{


	LONG    param_num = 0;

	/*
	char m_strId[] = "book01";
	char m_strAuthor[] = "samren";
	char m_strTitle[] = "xml";
	MSXML2::IXMLDOMDocumentPtr pDoc; 
	MSXML2::IXMLDOMElementPtr  xmlRoot ;
	
	HRESULT hr;
	hr = ::CoInitialize(NULL);
	if(!SUCCEEDED(hr)) 
	{
		MessageBox(_T("COM ��ʼ������!")); 
		return ;
	}
	hr = pDoc.CreateInstance(_uuidof(MSXML2::DOMDocument));
	if(!SUCCEEDED(hr)) 
	{
		MessageBox(_T("�޷�����DOMDocument���������Ƿ�װ��MS XML Parser ���п�!")); 
		return ;
	}

	//���ڵ������ΪBook
	//����Ԫ�ز���ӵ��ĵ���
	xmlRoot=pDoc->createElement((_bstr_t)"Book");

	//��������
	xmlRoot->setAttribute("id",(const char *)m_strId);
	pDoc->appendChild(xmlRoot);
	MSXML2::IXMLDOMElementPtr pNode;

	//��ӡ�author��Ԫ��
	pNode=pDoc->createElement((_bstr_t)"Author");
	pNode->Puttext((_bstr_t)(const char *)m_strAuthor);
	xmlRoot->appendChild(pNode);

	//��ӡ�Title��Ԫ��
	pNode=pDoc->createElement("Title");
	pNode->Puttext((const char *)m_strTitle);
	xmlRoot->appendChild(pNode);

	//���浽�ļ� 
	//��������ھͽ���,���ھ͸��� 
	pDoc->save("d:\\he.xml");
	::CoUninitialize ();
*/

	m_Xml_Path = _T("c:\\functions.xml");
	//m_Xml_Path = _T("c:\\people.xml");
	BSTR    name, content;
	MSXML2::DOMNodeType type;
	MSXML2::IXMLDOMDocumentPtr pdoc;
	MSXML2::IXMLDOMElementPtr proot = NULL;

	HRESULT hr;
	hr = ::CoInitialize(NULL);
	if(!SUCCEEDED(hr)) 
	{
		MessageBox(_T("COM ��ʼ������!")); 
		return ;
	}
	hr = pdoc.CreateInstance(_uuidof(MSXML2::DOMDocument));
	if(!SUCCEEDED(hr)) 
	{
		MessageBox(_T("�޷�����DOMDocument���������Ƿ�װ��MS XML Parser ���п�!")); 
		return ;
	}
	//hr = pdoc->load("d:\\he.xml");
	//hr = pdoc->load(_T("c:\\people.xml"));
	//hr = pdoc->load(_T("c:\\functions.xml"));
	hr = pdoc->load( (_bstr_t)m_Xml_Path );
	//if(!SUCCEEDED(hr)) 
	//{
	//	MessageBox(_T("����XML  Error!")); 
	//	return ;
	//}

	//FYI: hr = -1  correct!!
	if (S_OK != hr)
	{
		//cout << "Load XML Error!\n" << endl;
		//MessageBox(_T("����XML  Error!"));
	}
	//MessageBox(_T("success!"));

	//----------------------------------��ȡ���ڵ�------------------------------------
	hr = pdoc->get_documentElement (&proot);
	if (S_OK != hr)
	{
		MessageBox (_T("��ȡ���ڵ���������XML�ĸ�ʽ�Ƿ���ȷ��"));
	}
	//----------------------------------���Ը��ڵ�------------------------------------
	CString str;
	CString strName;
	CString strContent;
	proot->get_nodeName(&name);
	str = name;
	if (_T("FUNCTIONS") == str.GetString ())
	{
		printf("%ws\n", str.GetString ());
	}

	proot->get_text(&content);
	str = content;
	proot->get_nodeType(&type);

	//----------------------------------��ȡ���ڵ�����------------------------------------
	MSXML2::IXMLDOMNamedNodeMapPtr pAttrMap = NULL;
	MSXML2::IXMLDOMNodePtr pAttrItem;
	proot->get_attributes(&pAttrMap);

	LONG count;
	pAttrMap->get_length (&count);   //�ڵ����Ա�����Ը���
	if (count > 0)
	{
		for (int i = 0; i<count; ++i)
		{
			pAttrMap->get_item (i, &pAttrItem);  //��ȡ��һ������
			pAttrItem->get_nodeName (&name);   //��������
			pAttrItem->get_text (&content);   //����ֵ
			pAttrItem = NULL;
		}
	}
	pAttrMap = NULL;
	//----------------------------��ȡ��Ԫ�ص���Ԫ���б�---------------------------   
	MSXML2::IXMLDOMNodeListPtr pnodelist;
	MSXML2::IXMLDOMNodeListPtr psecnodelist;
	hr = proot->get_childNodes(&pnodelist);

	LONG productNum = 0;
	LONG secproductNum = 0;
	LONG thdproductNum = 0;
	MSXML2::IXMLDOMNodePtr pnode = NULL;
	MSXML2::IXMLDOMNodePtr psecnode = NULL;
	MSXML2::IXMLDOMNodePtr pthdnode = NULL;

	hr = pnodelist->get_length (&productNum);
	if (productNum > 0)
	{
		for (int i = 0;i<productNum; ++i)
		{
			hr = pnodelist->get_item (i, &pnode);
			LONG count1;
			pnode->get_attributes (&pAttrMap);
			pAttrMap->get_length (&count1);
			if (count1 > 0)
			{	
				PAttrMap funcAttriMap = new AttrMap();
				for (int j = 0; j<count1; ++j)
				{
					pAttrMap->get_item (j, &pAttrItem);
					pAttrItem->get_nodeName (&name);
					strName = name;
					pAttrItem->get_text (&content);
					strContent = content;
					funcAttriMap->insert (make_pair (strName, strContent));
					if (_T("Zw") == strContent.Left (2))
					{
						Fuzz_FunName = _T("Nt") + strContent.Mid (2);
//						Fuzz_FunName = strContent;
					}
//					func[strName] = strContent;
					//Array_value[0][strName] = strContent;
					pAttrItem = NULL;
				}
				fuzzFuncMap[0] = funcAttriMap;
			}
			
			hr = pnode->get_childNodes (&psecnodelist);
			hr = psecnodelist->get_length (&secproductNum);
			param_num = secproductNum;
			if (secproductNum > 0)
			{	
				//PFunc_Param paramMap = new Func_Param();
				for (int j = 0; j<secproductNum;++j)
				{
					LONG count2;
					hr = psecnodelist->get_item (j, &psecnode);
					/*psecnodelist->get_text(&content);
					Array_value[j+1]["content"] = content;*/
					psecnode->get_attributes (&pAttrMap);
					pAttrMap->get_length (&count2);
					
					if (count2 > 0)
					{	
						int paramOrder = 0;
						PAttrMap paramAttriMap = new AttrMap();
						for (int k = 0; k < count2; ++k)
						{
							pAttrMap->get_item (k, &pAttrItem);
							pAttrItem->get_nodeName (&name);
							strName = name;
							pAttrItem->get_text (&content);
							strContent = content;
							paramAttriMap->insert(make_pair(strName, strContent));
							if (strName == "order")
							{
								paramOrder = _wtoi(strContent.GetString ());
							}
							//attriMap[strName] = strContent
							//Array_value[j+1][strName] = strContent;
							pAttrItem = NULL;
						}
						fuzzFuncMap.insert(make_pair(paramOrder, paramAttriMap));
					}
					
		/*			MSXML2::IXMLDOMNodeListPtr pthdnodelist;
					hr = psecnode->get_childNodes (&pthdnodelist);
					hr = pthdnodelist->get_length (&thdproductNum);
					if (thdproductNum > 0)
					{
						for (int l=0;l<thdproductNum; ++l)
						{
							LONG count3;
							hr = pthdnodelist->get_item (l, &pthdnode);
							pthdnode->get_attributes (&pAttrMap);
							pAttrMap->get_length (&count3);
							for (int n = 0;n<count3; ++n)
							{
								pAttrMap->get_item (n, &pAttrItem);
								pAttrItem->get_nodeName (&name);
								pAttrItem->get_text (&content);
							}
							MSXML2::IXMLDOMNodeListPtr pforthnodelist;
							hr = psecnode->get_childNodes (&pforthnodelist);
							hr = pforthnodelist->get_length (&thdproductNum);
						}
					}*/
					
				}
				
				//fuzzFuncMap["PARAMS"] = paramMap;
			}
			pAttrMap = NULL;
			psecnodelist = NULL;

		}
	}

	pnodelist		= NULL;
	proot			= NULL;
	pdoc			= NULL;
	pAttrMap		= NULL;
	pAttrItem		= NULL;
	psecnodelist	= NULL;
	pnode			= NULL;
	//psecnode		= NULL;
	pthdnode		= NULL;

	::CoUninitialize ();

}

void CBSODDlg::FuzzXmlData()
{
	HANDLE hfile = NULL;
	OBJECT_ATTRIBUTES oa = {0};
	UNICODE_STRING uniName = {0};
	IO_STATUS_BLOCK IoBlock = {0};

	RtlInitUnicodeString_U (&uniName, TEST_FILE);
	InitializeObjectAttributes(&oa, &uniName, OBJ_CASE_INSENSITIVE, 0, 0);

	/*******************************************************************************
	* ���ܣ��Ѳ�����map����ȡ�����ŵ�vector���档�����ݲ�ͬ������ѹ�벻ͬ�����ֵ
	*******************************************************************************/


	
	POBJECT_ATTRIBUTES poa;
	PIO_STATUS_BLOCK pio;
	PUNICODE_STRING pus;

	map<int, PAttrMap >::iterator fuzzFuncMapit = fuzzFuncMap.begin();


	for(int i=-1; fuzzFuncMapit != fuzzFuncMap.end () ; ++fuzzFuncMapit,++i)//i��Ϊvector�ŵĲ���������һ���Ǻ����������Դ�-1��ʼ
	{
		if (fuzzFuncMapit->first > 0)
		{//��FuzzFuncMap��ŵĲ���ȡ�������������ݷŵ�vector
			map<CString,CString>::iterator it = fuzzFuncMapit->second->find (_T("fuzz"));
			if (it != fuzzFuncMapit->second->end() && it->second == _T("true"))
			{
				map<CString,CString>::iterator itTemp = fuzzFuncMapit->second->find (_T("name"));

				//����PHANDLE��PLARGE_INTEGER���Ͳ�ͬ��ѹ�벻ͬ����ֵ
				if (itTemp != fuzzFuncMapit->second->end() && itTemp->second == _T("PHANDLE"))
				{
					Fuzz_Param.push_back (GetAnyDword ());
					continue;
				}
				if (itTemp != fuzzFuncMapit->second->end() && itTemp->second == _T("POBJECT_ATTRIBUTES"))
				{
					//why ?
					
					poa = new OBJECT_ATTRIBUTES;
					pus = new UNICODE_STRING;
					pus->Buffer = (PWSTR)GetAnyDword ();
					poa->ObjectName = pus;
					Fuzz_Param.push_back ((DWORD)poa);

					continue;
				}
				if (itTemp != fuzzFuncMapit->second->end() && itTemp->second == _T("PIO_STATUS_BLOCK"))
				{
					pio = new IO_STATUS_BLOCK;
					pio->Pointer = (PVOID)GetAnyDword ();
					Fuzz_Param.push_back ((DWORD)pio);

					continue;
				}
				if (itTemp != fuzzFuncMapit->second->end() && itTemp->second == _T("PLARGE_INTEGER"))
				{
					Fuzz_Param.push_back (GetAnyDword ());
					continue;
				}
			}
			else
			{ 
				it = fuzzFuncMapit->second->find (_T("content"));
				if (it != fuzzFuncMapit->second->end())
				{
					Fuzz_Param.push_back (_ttoi(it->second));
				}
			}
		}

	}



	DWORD dTemp = 0;
	int ESP_Size = 0;

	for (int i=0;i<Fuzz_loops;i++)
	{
		for (int k=Fuzz_Param.size ()-1;k>=0;k--)
		{
			dTemp = Fuzz_Param.at (k);
			ESP_Size += sizeof(dTemp);
			__asm{
				mov edx,dTemp;
				push edx;
			}
		}
		if (_T("NtCreateFile") == Fuzz_FunName)
		{
			__asm{
				call pZwCreateFile;
				mov  eax ,ESP_Size;
				add esp,eax;
			}
		}
	}

	delete poa;
	delete pio;
	delete pus;

}

void CBSODDlg::OnBnClickedBtnXmlFuzz()
{
	// TODO: Add your control notification handler code here
	ParseXmlData (m_Xml_Path);
	FuzzXmlData ();
}

void CBSODDlg::OnBnClickedBtnDelLog()
{
	// TODO: Add your control notification handler code here
	DeleteFile(m_Log_Path);
	m_Log_Content = _T("");
	UpdateData (FALSE);
}
