
#ifndef _ZIPAPI_H_
#define _ZIPAPI_H_

#ifdef M4_USING_DLL
	#ifdef M4_O_M4FILEMANAGER
		#define M4_DECL_FILE_MANAGER_ZIP __declspec(dllexport)
	#else
		#define M4_DECL_FILE_MANAGER_ZIP __declspec(dllimport)
	#endif
#else
	#define M4_DECL_FILE_MANAGER_ZIP
#endif

M4_DECL_FILE_MANAGER_ZIP int M4FMExpandZip(const char* zipFile, const char* outputDirectory);
M4_DECL_FILE_MANAGER_ZIP int M4GenerateZipInternal(int argc,char *argv[], unsigned *errorCode, char *errorParameter, int paramMaxLen);
M4_DECL_FILE_MANAGER_ZIP int M4ExpandZipInternal(int argc,char *argv[], unsigned *errorCode, char *errorParameter, int paramMaxLen);
M4_DECL_FILE_MANAGER_ZIP int M4GetFileCrcInternal(const char* filenameinzip, void*buf, unsigned long size_buf, unsigned long* result_crc, unsigned *errorCode, char *errorParameter, int paramMaxLen);

#endif

