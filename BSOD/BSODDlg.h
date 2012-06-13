// BSODDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include <winternl.h>
#include "afxcmn.h"

#include <vector>
#include <map>
#include <atlstr.h>

using namespace std;

#define chDIMOF(Array) (sizeof(Array) / sizeof(Array[0]))
#define FUNCTION_DB_FILE _T("functions.db")
#define BUFF_LEN 50
#define Msg(x) MessageBox (_T(x));

typedef map< CString, CString > Attr_value;
typedef map<CString, CString>  AttrMap;
typedef map<CString, CString>* PAttrMap;
typedef map<int, AttrMap>  Func_Param;
typedef map<int, AttrMap>* PFunc_Param;

// CBSODDlg �Ի���
class CBSODDlg : public CDialog
{
// ����
public:
	CBSODDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_BSOD_DIALOG };

	protected:
	//VOID RtlInitUnicodeString_U(OUT PUNICODE_STRING DestinationString,IN PWSTR SourceString);

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	//BOOL CheckExistInFunctionsDB(LPWSTR lpFileName, LPWSTR readdata);

	//void ParseDataFromFunctionDB(LPCTSTR lpFileName, LPCTSTR readdata);
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	BOOL OnInitData();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddFunction();
	afx_msg void OnLbnSelchangeListToTest();
	afx_msg void OnBnClickedFuzz();
	afx_msg void OnInitLogPath();
	CString GetFilePath();
	int WriteLog(LPCTSTR lpFileName, LPCTSTR writedata);
	BOOL CheckExistInFuzzList(CString m_Funtion_Input);
	//void MainFuzz();
public:
	int m_ListIndex;
	CString Fuzz_FunName;
	vector<DWORD> Fuzz_Param;
	map<int, PAttrMap> fuzzFuncMap;
private:
	CListBox m_Function_List;
	CString m_Function_Input;
//public:
//	CString m_Function_Name;
//	CString m_Function_Args;
//	CString m_Function_DllName;

private:
	CString m_Function_Name;
	CString m_Function_Args;
	CString m_Function_DllName;
private:
	int Fuzz_loops;
	//CString Fuzz_loops;
	//CString Fuzz_seed;
	int Fuzz_seed;
	CString sFunctionDBfile;
private:
	CProgressCtrl m_CtrlProgress;
public:
	afx_msg void OnBnClickedRmButton();
private:
	CListCtrl m_List_Xml;//xml��ListControl�ؼ���������
public:
	afx_msg CString BootOpenDialog();
	afx_msg CString BootSaveAsDialog();
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void AddXmlToList(CString m_Xml_Path);
private:
	CString m_Xml_Path;//xml���ڵ�·��
public:
	afx_msg void OnLvnItemchangedListXml(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListXml(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMShowClickListXml();
private:
	CString m_RichEdit;//xml����ʾ�򣬿��Ա༭xml
public:
	afx_msg void OnBnClickedBtnXmlSave();
	afx_msg void OnBnClickedBtnXmlSaveAs();
	afx_msg void OnBnClickedBtnXmlDel();
	afx_msg void OnBnClickedBtnXmlFuzz();
	afx_msg void ParseXmlData(CString m_Xml_Path);
	afx_msg void FuzzXmlData();
private:
	CString m_Log_Path;
public:
	afx_msg void OnBnClickedBtnDelLog();
private:
	CString m_Log_Content;
public:
	afx_msg void OnEnChangeEditFuzzerLoops();
};


