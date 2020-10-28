//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CFileIO.h
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************

#ifndef CFILEIO_H
#define CFILEIO_H


#ifndef NULL
#define NULL ((void*) 0L)
#endif

#ifdef __APPLE__
#define OPENMODE std::_Ios_Openmode
#define SEEKDIR  std::_Ios_Seekdir
#else
#define OPENMODE int
#define SEEKDIR int
#endif


#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <NStringTool.h>
#include <EFileTypes.h>


enum EFileAttribut
{
	EFA_NOT_FOUND,
	EFA_FILE,
	EFA_FOLDER
};

class CFileIO
{
	public:

		CFileIO()
		{
			mOpenRead = false;
			mOpenWrite = false;
			mFs = NULL;
		}

		static void*  ReadFile(const char* Filename, int* nBytes);
		bool   OpenFileRead(const char* Filename, OPENMODE OpenMode);
		bool   OpenFileWrite(const char*Filename, OPENMODE OpenMode);
		bool   OpenFileAppend(const char* Filename, OPENMODE OpenMode);
		bool   OpenFileChange(const char* Filename, int Position);
		bool   ReadBytes(void* buffer, int nBytes);
		void*  ReadBytes(int nBytes);
		bool   WriteBytes(const void* buffer, int nBytes);
		bool   WriteString(const char* str);
		bool   WriteFile(const char* Filename, const void* Buffer, int nBytes);
		char*  ReplaceAllStrings(char* SrcString, char* SearchString, char* SubString, int* Count);
		int    GetChar();
		bool   IsEof();
		void   Clear();
		void   SkipBytes(int nBytes);
		static EFileType GuessFileFormat(const char* Filename);
		static char*  ReplaceApp(const char* Filename, const char* neuApp);
		bool   ReadLine(std::vector<char>* Line);
		bool   ReadLine(std::string* Line);
		bool   ReadLine(std::string* Line, int max);
		bool   ReadSplitLine(stringvector* StringVector, char Div);
		bool   ReadSplitLine(stringvector* StringVector, char Div, int max);
		int    WriteFromFile(const char* Filename);
		bool   WriteFromFileBytes(const char* Filename, int Offset, int Size);

		static bool StaticWriteFile(const std::string Filename, const void* Data, int DataSize);
		static std::string StripExtension(const char* FullPathName);
		static bool   ResolveWildCard(const char* Dir, const char* WildCard, stringvector* FileList);
		static char*  NewString(const char* OldStr);
		static const char*  Fill0(int i, int n);
		static const char*  Fill0(char* strbuffer, int i, int n);
		static bool   FileExists(const char* Filename);
		static char*  ExtractFilename(const char* FullPathName);
		static const char*  ExtractPath(const char* FullPathName);
		static bool   Strcmpx(const char* s1, const char* s2);
		static int GetFileSize(const char* Filename);
		static bool CreateDir(const char* NewFolder);
		static bool ListDir(const std::string Dir, std::vector<std::string>* FileList, int Level, bool DoRemove);
    static void RemoveDir(const char* Folder);
		static void RenameAndRemoveDir(const char* dir);
		static void RenameFile(const std::string Filename, const std::string NewFilename);
		static void RemoveFile(const std::string Filename);
    static bool DebugFile(const std::string& filename);


		void   DeleteStringList(char** StrList);
		std::string Fill0str(int i, int n);

		template <typename Tobject>
		bool ReadObject(Tobject* obj)
		{
			return ReadBytes(obj, sizeof(Tobject));
		}
		
		template <typename Tobject>
		bool ReadObjects(Tobject* obj, int n)
		{
			return ReadBytes(obj, sizeof(Tobject) * n);
		}		

		template <typename Tobject>
		bool WriteObject(const Tobject* obj)
		{
			return WriteBytes(obj, sizeof(Tobject));
		}

		template <typename Tobject>
		bool WriteObjectArray(const Tobject* obj, int Count)
		{
			WriteBytes(obj, sizeof(Tobject) * Count);
		}

		bool Good()
		{
			return mFs->good();
		}


		void  CloseFile();

		bool 	SetReadPos(int pos);
		bool 	SetReadPos2(int pos, SEEKDIR where);
		int  	GetReadPos();

		int  	GetWritePos();
		void  SetWritePos(unsigned int pos);
		void 	SetWritePosEnd();



	protected:

		std::fstream* mFs;
		bool     mOpenWrite;
		bool     mOpenRead;
};

#endif
