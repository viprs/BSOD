#pragma once
//#include "Functions.h"
#include <winternl.h>



typedef
LONG
(WINAPI *DefZwClose)(HANDLE Handle);
LONG pZwClose(HANDLE Handle);
typedef
LONG
(WINAPI *DefZwCreateProcess)(
							 PHANDLE ProcessHandle,
							 ACCESS_MASK DesiredAccess,
							 POBJECT_ATTRIBUTES ObjectAttributes,
							 HANDLE ParentProcess,
							 BOOLEAN InheritObjectTable,
							 HANDLE SectionHandle,HANDLE DebugPort,
							 HANDLE ExceptionPort);
LONG pZwCreateProcess(
				 PHANDLE ProcessHandle,
				 ACCESS_MASK DesiredAccess,
				 POBJECT_ATTRIBUTES ObjectAttributes,
				 HANDLE ParentProcess,
				 BOOLEAN InheritObjectTable,
				 HANDLE SectionHandle,
				 HANDLE DebugPort,
				 HANDLE ExceptionPort);

typedef
LONG 
(WINAPI *DefZwCreateFile)(
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
						  __in ULONG  EaLength
						  );
LONG pZwCreateFile(
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
			  __in ULONG  EaLength);

typedef
LONG 
(WINAPI *DefZwCreateKey)(
						 OUT PHANDLE  KeyHandle,
						 IN ACCESS_MASK  DesiredAccess,
						 IN POBJECT_ATTRIBUTES  ObjectAttributes,
						 IN ULONG  TitleIndex,
						 IN PUNICODE_STRING  Class  OPTIONAL,
						 IN ULONG  CreateOptions,
						 OUT PULONG  Disposition  OPTIONAL
						 );



typedef
LONG
(WINAPI *DefZwCreateMutant)(
							PHANDLE MutantHandle,
							ACCESS_MASK DesiredAccess,
							POBJECT_ATTRIBUTES ObjectAttributes,
							BOOLEAN InitialOwner);
LONG
pZwCreateMutant(
				PHANDLE MutantHandle,
				ACCESS_MASK DesiredAccess,
				POBJECT_ATTRIBUTES ObjectAttributes,
				BOOLEAN InitialOwner);

typedef
LONG
(WINAPI *DefZwCreatePort)(
						  PHANDLE PortHandle,
						  POBJECT_ATTRIBUTES ObjectAttributes,
						  ULONG MaxDataSize,
						  ULONG MaxMessageSize,
						  ULONG Reserved);
LONG pZwCreatePort(
					PHANDLE PortHandle,
					POBJECT_ATTRIBUTES ObjectAttributes,
					ULONG MaxDataSize,
					ULONG MaxMessageSize,
					ULONG Reserved);