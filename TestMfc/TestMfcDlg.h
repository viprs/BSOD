
// TestMfcDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CTestMfcDlg �Ի���
class CTestMfcDlg : public CDialog
{
// ����
public:
	CTestMfcDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	void GetFileList(CString sFloderPath,CStringArray& aryFileList);
	CString CTestMfcDlg::GetParentDirName(CString sFilePath);
public:
	CListBox m_ListBox;
};
