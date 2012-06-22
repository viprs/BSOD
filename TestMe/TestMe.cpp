#include <atlstr.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <string>
#include <math.h>
#include <map>
#include <vector>
#include <WinCrypt.h>


using namespace std;


//#include <afxcoll.h>
//CString
//GetFilePath() const
//{
//	ASSERT(m_hContext != NULL);
//	ASSERT_VALID(this);
//
//	CString strResult = m_strRoot;
//	LPCTSTR pszResult;
//	LPCTSTR pchLast;
//	pszResult = strResult;
//	pchLast = _tcsdec( pszResult, pszResult+strResult.GetLength() );
//	ENSURE(pchLast!=NULL);
//	if ((*pchLast != _T('\\')) && (*pchLast != _T('/')))
//		strResult += m_chDirSeparator;
//	strResult += GetFileName();
//	return strResult;
//}
//
//void
//GetFileList(CString sFloderPath,CStringArray& aryFileList)
//{
//	if (sFloderPath.GetLength() == 0)
//	{
//		return;
//	}
//	CString sTemp;
//	CString sDirPath;
//	sDirPath = sFloderPath + _T("\\*");
//	CFileFind ff;
//	BOOL res = ff.FindFile(sDirPath);   
//	while(res)
//	{
//		res = ff.FindNextFile();
//		if(ff.IsDirectory() && !ff.IsDots())
//		{
//			GetFileList(ff.GetFilePath(),aryFileList);
//			continue;
//		}
//		else if(!ff.IsDirectory() && !ff.IsDots())  
//		{
//			aryFileList.Add(ff.GetFilePath());			
//		}
//
//	}
//	ff.Close();
//}

int fun(int a, short b)
{
	int x = a + 1;
	short y = b + 2; 
	a = x + 1;
	y = y + a;
	return a+x+y;
}


int 
main()
{
	int m = 1;
	short n = 2; 
	int sum = 0; 
	sum = fun(m, n);
	m = sum +1;
	return 0;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//DWORD dwLen = 4,test;
	//BYTE* pbBuffer =(BYTE*) malloc (4*sizeof(BYTE));
	//HCRYPTPROV hCryptProv;
	//if(!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0))
	//	printf("CryptAcquireContext Failed!");

	//if(hCryptProv)
	//	for (int i=0;i<100;i++)
	//	{
	//		if(CryptGenRandom( hCryptProv, dwLen, (BYTE*)pbBuffer))
	//		{
	//			//printf ("%d  ", *pbBuffer);
	//			RtlMoveMemory(&test, pbBuffer,4);
	//			if ( ((2147418113 < fabsl (test)) && (fabsl (test) <4294967295)) )
	//			{
	//				printf ("%d\n",fabsl (test));
	//			}
	//			
	//		}
	//	}

	//if(hCryptProv)
	//	if(!CryptReleaseContext(hCryptProv, 0))
	//		printf("CryptReleaseContext Failed!");
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	
	//return 333444444;


	//Temp += 2147483648;
	//return (PVOID)Temp;
}
	//printf ("");
	//CStringArray   as;  
	//as.Add("aaaa");  
	//as.Add("bbb");  
	//as.Add("CCC");  
	//as.Add("dddddddddddddd");  

	//int   size=as.GetSize();  
	//CStdioFile   file;  
	//CString   strTemp;  
	//file.Open("Save.txt",CFile::modeCreate|CFile::modeWrite);  

	//for(int   i=0;i<size;i++)  
	//{  
	//	strTemp=as.GetAt(i);  
	//	file.WriteString(strTemp+"\n");  
	//}  
	//file.Close();