#include "stdafx.h"
#include "Functions.h"
#include <winternl.h>




LONG
pZwClose(HANDLE Handle)
{
	DefZwClose pZwClose = (DefZwClose) GetProcAddress (GetModuleHandle(_T("ntdll.dll")), "NtClose");
	if (pZwClose != NULL)
	{
		LONG lRet = pZwClose(Handle);
		if (lRet >= 0)
		{
			printf ("pZwClose success\n");
		}
		else
		{
			//printf ("pNtCreateFile fail\n");
			//MessageBox(NULL, _T("Fail!"), _T("Error"), MB_OK);
		}
	} 
	else
	{
		printf ("pZwClose GetProcAddress ntdll fail\n");
	}

	return TRUE;
}

LONG
pZwCreateProcess(
				 PHANDLE ProcessHandle,
				 ACCESS_MASK DesiredAccess,
				 POBJECT_ATTRIBUTES ObjectAttributes,
				 HANDLE ParentProcess,
				 BOOLEAN InheritObjectTable,
				 HANDLE SectionHandle,
				 HANDLE DebugPort,
				 HANDLE ExceptionPort)
{
	DefZwCreateProcess pZwCreateProcess = (DefZwCreateProcess) GetProcAddress (GetModuleHandle(_T("ntdll.dll")), "NtCreateProcess");
	if (pZwClose != NULL)
	{
		LONG lRet = pZwCreateProcess(
			ProcessHandle,
			DesiredAccess,
			ObjectAttributes,
			ParentProcess,
			InheritObjectTable,
			SectionHandle,
			DebugPort,
			ExceptionPort);
		if (lRet >= 0)
		{
			printf ("pZwCreatePort success\n");
		}
		else
		{
			//printf ("pNtCreateFile fail\n");
			//MessageBox(NULL, _T("Fail!"), _T("Error"), MB_OK);
		}
	} 
	else
	{
		printf ("pZwCreatePort GetProcAddress ntdll fail\n");
	}

	return TRUE;
}
LONG
pZwCreateFile(
			  __out PHANDLE  FileHandle,
			  __in ACCESS_MASK  DesiredAccess,
			  __in POBJECT_ATTRIBUTES  ObjectAttributes,
			  __out PIO_STATUS_BLOCK  IoStatusBlock,
			  __in_opt PLARGE_INTEGER  AllocationSize,
			  __in ULONG  FileAttributes,
			  __in ULONG  ShareAccess,
			  __in ULONG  CreateDisposition,
			  __in ULONG  CreateOptions,
			  __in_opt PVOID  EaBuffer,
			  __in ULONG  EaLength)
{
	DefZwCreateFile pNtCreateFile = (DefZwCreateFile) GetProcAddress (GetModuleHandle(_T("ntdll.dll")), "NtCreateFile");
	if (pNtCreateFile != NULL)
	{
		LONG lRet = pNtCreateFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes,
			ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength);
		if (lRet >= 0)
		{
			printf ("pNtCreateFile success\n");
		}
		else
		{
			//printf ("pNtCreateFile fail\n");
			//MessageBox(NULL, _T("Fail!"), _T("Error"), MB_OK);
		}
	} 
	else
	{
		printf ("DefZwCreateFile GetProcAddress ntdll fail\n");
	}

	return TRUE;
}

LONG
pZwCreateKey(
			 PHANDLE KeyHandle,
			 ACCESS_MASK DesiredAccess,
			 POBJECT_ATTRIBUTES ObjectAttributes,
			 ULONG TitleIndex,
			 PUNICODE_STRING Class,
			 ULONG CreateOptions,
			 PULONG Disposition)
{
	DefZwCreateKey pNtCreateKey = (DefZwCreateKey) GetProcAddress (GetModuleHandle(_T("ntdll.dll")), "NtCreateKey");
	if (pNtCreateKey != NULL)
	{
		LONG lRet = pNtCreateKey(
					KeyHandle,
					DesiredAccess,
					ObjectAttributes,
					TitleIndex,
					Class,
					CreateOptions,
					Disposition);
		if (lRet >= 0)
		{
			AfxMessageBox (_T("pNtCreateFile success\n"));
		}
		else
		{
			//printf ("pNtCreateFile fail\n");
			//MessageBox(NULL, _T("Fail!"), _T("Error"), MB_OK);
		}
	} 
	else
	{
		AfxMessageBox (_T("DefZwCreateFile GetProcAddress ntdll fail\n"));
	}

	return TRUE;
}
LONG
pZwCreateSection(
				 OUT PHANDLE  SectionHandle,
				 IN ACCESS_MASK  DesiredAccess,
				 IN POBJECT_ATTRIBUTES  ObjectAttributes  OPTIONAL,
				 IN PLARGE_INTEGER  MaximumSize  OPTIONAL,
				 IN ULONG  SectionPageProtection,
				 IN ULONG  AllocationAttributes,
				 IN HANDLE  FileHandle  OPTIONAL
				 )
{
	DefZwCreateSection pNtCreateSection = (DefZwCreateSection) GetProcAddress (GetModuleHandle(_T("ntdll.dll")), "NtCreateSection");
	if (pNtCreateSection != NULL)
	{
		LONG lRet = pNtCreateSection(
			SectionHandle,
			DesiredAccess,
			ObjectAttributes,
			MaximumSize,
			SectionPageProtection,
			AllocationAttributes,
			FileHandle);
		if (lRet >= 0)
		{
			AfxMessageBox (_T("pNtCreateSection success\n"));
		}
		else
		{
			//printf ("pNtCreateFile fail\n");
			//MessageBox(NULL, _T("Fail!"), _T("Error"), MB_OK);
		}
	} 
	else
	{
		AfxMessageBox (_T("pNtCreateSection GetProcAddress ntdll fail\n"));
	}

	return TRUE;
}
LONG
pZwDeleteFile(
			 IN POBJECT_ATTRIBUTES  ObjectAttributes
			 )
{
	DefZwDeleteFile pNtDeleteFile = (DefZwDeleteFile) GetProcAddress (GetModuleHandle(_T("ntdll.dll")), "NtDeleteFile");
	if (pNtDeleteFile != NULL)
	{
		LONG lRet = pNtDeleteFile(ObjectAttributes);
		if (lRet >= 0)
		{
			AfxMessageBox (_T("pNtDeleteFile success\n"));
		}
		else
		{
			//printf ("pNtCreateFile fail\n");
			//MessageBox(NULL, _T("Fail!"), _T("Error"), MB_OK);
		}
	} 
	else
	{
		AfxMessageBox (_T("pNtDeleteFile GetProcAddress ntdll fail\n"));
	}

	return TRUE;
}
LONG
pZwDeleteKey(
			 IN HANDLE  KeyHandle
			 )
{
	DefZwDeleteKey pNtDeleteKey = (DefZwDeleteKey) GetProcAddress (GetModuleHandle(_T("ntdll.dll")), "NtDeleteKey");
	if (pNtDeleteKey != NULL)
	{
		LONG lRet = pNtDeleteKey(KeyHandle);
		if (lRet >= 0)
		{
			AfxMessageBox (_T("pNtDeleteKey success\n"));
		}
		else
		{
			//printf ("pNtCreateFile fail\n");
			//MessageBox(NULL, _T("Fail!"), _T("Error"), MB_OK);
		}
	} 
	else
	{
		AfxMessageBox (_T("pNtDeleteKey GetProcAddress ntdll fail\n"));
	}

	return TRUE;
}

//LONG
//pZwCreateProcess(
//				 PHANDLE ProcessHandle,
//				 ACCESS_MASK DesiredAccess,
//				 POBJECT_ATTRIBUTES ObjectAttributes,
//				 HANDLE ParentProcess,
//				 BOOLEAN InheritObjectTable,
//				 HANDLE SectionHandle,
//				 HANDLE DebugPort,
//				 HANDLE ExceptionPort)
//{
//	DefZwCreateProcess pZwCreateProcess = (DefZwCreateProcess) GetProcAddress (GetModuleHandle(_T("ntdll.dll")), "NtCreateProcess");
//	if (pZwCreateProcess != NULL)
//	{
//		LONG lRet = pZwCreateProcess(
//									 ProcessHandle,
//									 DesiredAccess,
//									 ObjectAttributes,
//									 ParentProcess,
//									 InheritObjectTable,
//									 SectionHandle,
//									 DebugPort,
//									 ExceptionPort);
//		if (lRet >= 0)
//		{
//			printf ("pZwCreateProcess success\n");
//		}
//		else
//		{
//			//printf ("pNtCreateFile fail\n");
//			MessageBox(NULL, _T("Fail!"), _T("Error"), MB_OK);
//		}
//	} 
//	else
//	{
//		printf ("DefZwCreateProcess GetProcAddress ntdll fail\n");
//	}
//
//	return TRUE;
//}


LONG
pZwCreateMutant(
				PHANDLE MutantHandle,
				ACCESS_MASK DesiredAccess,
				POBJECT_ATTRIBUTES ObjectAttributes,
				BOOLEAN InitialOwner)
{
	DefZwCreateMutant pZwCreateMutant = (DefZwCreateMutant) GetProcAddress (GetModuleHandle(_T("ntdll.dll")), "NtCreateMutant");
	if (pZwClose != NULL)
	{
		LONG lRet = pZwCreateMutant(
			MutantHandle,
			DesiredAccess,
			ObjectAttributes,
			InitialOwner);
		if (lRet >= 0)
		{
			printf ("pZwCreateMutant success\n");
		}
		else
		{
			//printf ("pNtCreateFile fail\n");
			//MessageBox(NULL, _T("Fail!"), _T("Error"), MB_OK);
		}
	} 
	else
	{
		printf ("pZwCreateMutant GetProcAddress ntdll fail\n");
	}

	return TRUE;
}

LONG
pZwCreatePort(
			  PHANDLE PortHandle,
			  POBJECT_ATTRIBUTES ObjectAttributes,
			  ULONG MaxDataSize,
			  ULONG MaxMessageSize,
			  ULONG Reserved)
{
	DefZwCreatePort pZwCreatePort = (DefZwCreatePort) GetProcAddress (GetModuleHandle(_T("ntdll.dll")), "NtCreatePort");
	if (pZwClose != NULL)
	{
		LONG lRet = pZwCreatePort(
			PortHandle,
			ObjectAttributes,
			MaxDataSize,
			MaxMessageSize,
			Reserved);
		if (lRet >= 0)
		{
			printf ("pZwCreatePort success\n");
		}
		else
		{
			//printf ("pNtCreateFile fail\n");
			//MessageBox(NULL, _T("Fail!"), _T("Error"), MB_OK);
		}
	} 
	else
	{
		printf ("pZwCreatePort GetProcAddress ntdll fail\n");
	}

	return TRUE;
}
LONG pZwDeleteValueKey(
					   HANDLE KeyHandle,
					   PUNICODE_STRING ValueName
					   )
{
	DefZwDeleteValueKey pZwDeleteValueKey = (DefZwDeleteValueKey) GetProcAddress (GetModuleHandle(_T("ntdll.dll")), "NtDeleteValueKey");
	if (pZwClose != NULL)
	{
		LONG lRet = pZwDeleteValueKey(
					KeyHandle,
					ValueName);
		if (lRet >= 0)
		{
			printf ("pZwDeleteValueKey success\n");
		}
		else
		{
			//printf ("pNtCreateFile fail\n");
			//MessageBox(NULL, _T("Fail!"), _T("Error"), MB_OK);
		}
	} 
	else
	{
		printf ("pZwDeleteValueKey GetProcAddress ntdll fail\n");
	}

	return TRUE;
}