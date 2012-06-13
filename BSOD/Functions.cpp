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