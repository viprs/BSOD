

#define TEST_FILE L"\\??\\c:\\a.txt"
#define LOG_FILE L"Log2012-03-18.txt"



#define FILE_SUPERSEDE                  0x00000000
#define FILE_OPEN                       0x00000001
#define FILE_CREATE                     0x00000002
#define FILE_OPEN_IF                    0x00000003
#define FILE_OVERWRITE                  0x00000004
#define FILE_OVERWRITE_IF               0x00000005
#define FILE_MAXIMUM_DISPOSITION        0x00000005

#define OBJ_CASE_INSENSITIVE            0x00000040
#define FILE_NON_DIRECTORY_FILE         0x00000040

#define InitializeObjectAttributes( p, n, a, r, s ) { \
	(p)->Length = sizeof( OBJECT_ATTRIBUTES );          \
	(p)->RootDirectory = r;                             \
	(p)->Attributes = a;                                \
	(p)->ObjectName = n;                                \
	(p)->SecurityDescriptor = s;                        \
	(p)->SecurityQualityOfService = NULL;               \
}

//typedef struct _UNICODE_STRING {
//	USHORT Length;
//	USHORT MaximumLength;
//	PWSTR  Buffer;
//} UNICODE_STRING;
//typedef UNICODE_STRING *PUNICODE_STRING;
//typedef const UNICODE_STRING *PCUNICODE_STRING;

//typedef
//LONG 
//(WINAPI *DefZwCreateFile)(
//						  __out PHANDLE  FileHandle,
//						  __in ACCESS_MASK  DesiredAccess,
//						  __in POBJECT_ATTRIBUTES  ObjectAttributes,
//						  __out PIO_STATUS_BLOCK  IoStatusBlock,
//						  __in_opt PLARGE_INTEGER  AllocationSize,
//						  __in ULONG  FileAttributes,
//						  __in ULONG  ShareAccess,
//						  __in ULONG  CreateDisposition,
//						  __in ULONG  CreateOptions,
//						  __in_opt PVOID  EaBuffer,
//						  __in ULONG  EaLength
//						  );
 