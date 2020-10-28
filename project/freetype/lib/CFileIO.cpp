//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CFileIO.cpp
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************
//
// Inhalt: Implementation der Klasse CFileIO
//
//------------------------------------------------------------------------------
// Revisionsgeschichte:
//
// 08.01.2001 erstellt - Martin Steen
//
//------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <CFileIO.h>


using namespace std;



//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   GetChar
//
// Parameter: char* Filename - Name der Datei
//
// Return:    Eingelesenes Zeichen
//
//---------------------------------------------------------------------------

int CFileIO::GetChar()
{
  char ch;
  mFs->get(ch);
  return ch;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   IsEof
//
//
//---------------------------------------------------------------------------

bool CFileIO::IsEof()
{
	if (mFs == NULL) return true;
  return mFs->eof();
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   Clear
//
//
//
//
//---------------------------------------------------------------------------

void CFileIO::Clear()
{
	if (mFs != NULL)
	{
		mFs->clear();
	}
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   SkipBytes
//
// Parameter: int nBytes - Anzahl an Bytes ueberspringen (Eingabedatei)
//
//
//---------------------------------------------------------------------------

void CFileIO::SkipBytes(int nBytes)
{
	mFs->seekg(nBytes, ios_base::cur);
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   GetFileSize
//
// Parameter: char* Filename - Name der Datei
//
// Return:    int - Groesse der Datei in Bytes
//
//---------------------------------------------------------------------------

int CFileIO::GetFileSize(const char* Filename)
{
  long FileSize;
  ifstream fin(Filename, ios::in | ios::binary);
  fin.seekg(0, ios::end);
  FileSize = fin.tellg();
  fin.close();
  return FileSize;
}


//--------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   FileExists
//
// Parameter: char* Filename - Name der Datei
//
// Return:    true  - Datei wurde geoeffnet
//            false - Datei konnte nicht geöffnet werden
//
// ueberpruefen, ob Datei existiert
//
//---------------------------------------------------------------------------
/*
bool CFileIO::FileExists(const char* Filename)
{
  ifstream fin(Filename, ios::in | ios::binary);
  bool s = fin.good();

  if (s)
  {
    fin.close();
  }
  return s;
}
*/

bool CFileIO::FileExists(const char* Filename) 
{
	struct stat buffer;   
  return stat(Filename, &buffer) == 0; 
	
   //return access(Filename, F_OK) != -1;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		ListDir
//
// Falls DoRemove = true:
//
//  Level = 0:    nur Unterordner loeschen
//  Level = 1:    Ordner mit Unterordnern loeschen   
//
//---------------------------------------------------------------------------

bool CFileIO::ListDir(const std::string Dir, std::vector<std::string>* FileList, int Level, bool DoRemove)
{
  struct dirent *CurrentFile;
  DIR*   Directory;
  bool   r = false;

	Directory = opendir(Dir.c_str());


  if (FileList != NULL)
  {
	  FileList->clear();
	}

	if (Directory != NULL)
	{
		r = true;
		Directory = opendir(Dir.c_str());

		int i = 0;
		while ((CurrentFile = readdir(Directory)) != NULL)
		{
			struct stat FileInfo;
			

		  string Filename = CurrentFile->d_name;
		  string FullPath = Dir + '/' + Filename;
		  		  
		  if ((Filename != ".") && (Filename != ".."))
		  {
		   if (stat(FullPath.c_str(), &FileInfo) == 0)
				{
			
					if (FileInfo.st_mode & S_IFDIR)
					{
					  //cout << "  Dir :" << FullPath << endl;					
						ListDir(FullPath, FileList, Level + 1, DoRemove);
					}
					else
					{
					  if (FileList != NULL)
					  {
							FileList->push_back(Filename);      
						}
 					  //cout << "  File:" << FullPath << endl;
 					  if (DoRemove)
 					  {
 					    remove(FullPath.c_str());
 					  } 					  
					}
				}
			}	  
		}
		closedir(Directory);
		
		if (DoRemove)
		{
		  if (Level > 0)
		  {
		  	rmdir(Dir.c_str());
		  }
		}
		
	}
	return r;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		RemoveDir
//
//---------------------------------------------------------------------------

void CFileIO::RemoveDir(const char* dir)
{
#if 0
	char cmd[512];
  sprintf(cmd, "rm -rf %s", dir);	
	system(cmd);
#endif	
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		RemoveFile
//
//---------------------------------------------------------------------------

void CFileIO::RemoveFile(const std::string Filename)
{
  if (FileExists(Filename.c_str()))
  {
    remove(Filename.c_str());
    /*
		char* cmd = new char[512];
		sprintf(cmd, "rm -rf %s", Filename.c_str());	
		system(cmd);
		delete[] cmd;
		*/
	}
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		RenameFile
//
//---------------------------------------------------------------------------

void CFileIO::RenameFile(const std::string Filename, const std::string NewFilename)
{
  if (FileExists(Filename.c_str()))
  {
	  rename(Filename.c_str(), NewFilename.c_str());
	  /*
		char* cmd = new char[512];
		sprintf(cmd, "mv %s %s", Filename.c_str(), NewFilename.c_str());	 // Umbenennen
		system(cmd);
		delete[] cmd;
		*/
	}
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		RenameAndRemoveDir
//
//---------------------------------------------------------------------------

void CFileIO::RenameAndRemoveDir(const char* dir)
{
#if 0
	char cmd[512];
	
	
  sprintf(cmd, "mv %s %s.tmp", dir, dir);	 // Umbenennen
	system(cmd);
  sprintf(cmd, "rm -rf %s.tmp", dir);	     // Umbenannte Datei loeschen
	system(cmd);
#endif	
	
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		FileAttribut
//
// return: 0 = Datei nicht gefunden
//         1 = "normale" Datei
//         2 = Ordner
//
//---------------------------------------------------------------------------

/*
int CFileIO::FileAttribut(const char* Filename)
{
	int r = EFA_FILE;

#ifdef __WIN32__

	DWORD fa = GetFileAttributes(Filename);
	if (fa == 0xFFFFFFFF)
	{
		r = EFA_NOT_FOUND;
	}
	else
	if (fa & FILE_ATTRIBUTE_DIRECTORY)
	{
		r = EFA_FOLDER;
	}
#endif

	return r;

}
*/

//--------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   OpenFileRead
//
// Parameter: char* Filename - Name der Datei
//
// Return:    true  - Datei wurde geoeffnet
//            false - Datei konnte nicht geoeffnet werden
//
// Öffnen einer Datei zum Lesen
//
//---------------------------------------------------------------------------

bool CFileIO::OpenFileRead(const char* Filename, OPENMODE OpenMode)
{
  if (mFs != NULL) delete mFs;
  mFs = new fstream(Filename, ios::in | OpenMode);
  mOpenRead = mFs->good();
  return mOpenRead;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   OpenFileWrite
//
// Parameter: char* Filename - Name der Datei
//
// Return:    true  - Datei wurde geöffnet
//            false - Datei konnte nicht geöffnet werden
//
// Öffnen einer Datei zum Lesen
//
//---------------------------------------------------------------------------

bool CFileIO::OpenFileWrite(const char* Filename, OPENMODE OpenMode)
{
  if (mFs != NULL) delete mFs;
  mFs = new fstream(Filename, ios::out | OpenMode);
  mOpenWrite = mFs->good();
  return mOpenWrite;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   OpenFileAppend
//
// Parameter: char* Filename - Name der Datei
//
// Return:    true  - Datei wurde geöffnet
//            false - Datei konnte nicht geöffnet werden
//
// Öffnen einer Datei zum Lesen
//
//---------------------------------------------------------------------------

bool CFileIO::OpenFileAppend(const char* Filename, OPENMODE OpenMode)
{
  if (mFs != NULL) delete mFs;
  mFs = new fstream(Filename, ios::app | ios::out| OpenMode);
  mOpenWrite = mFs->good();
  return mOpenWrite;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   OpenFileChange
//
// Parameter: char* filename - Name der Datei
//            int pos        - Schreibposition
//
// Return:    true  - Datei wurde geöffnet
//            false - Datei konnte nicht geöffnet werden
//
// Oeffnen einer Datei zum Aendern
//
//---------------------------------------------------------------------------

bool CFileIO::OpenFileChange(const char* Filename, int Position)
{
	bool suc = OpenFileWrite(Filename, ios::out | ios::binary | ios::in);

	if (suc)
	{
		SetReadPos(Position);
		SetWritePos(Position);
	}
	return suc;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadFile
//
// Parameter: char* Filename - Name der Datei
//
//            int* nBytes    - Anzahl der Bytes die gelesen wurden (Größe
//                             der Datei).
//
// Return:    void* - Zeiger auf Puffer, in den die Daten gelesen wurden,
//                    oder NULL (Fehlerfall)
//
//
//
// Lesen des Inhalts einer Datei in einen Puffer. Der Speicher für den
// Puffer wird reserviert. Zurückgegeben werden ein Zeiger auf den
// Puffer sowie die Anzahl der eingelesenen Bytes
//
//---------------------------------------------------------------------------

void* CFileIO::ReadFile(const char* Filename, int* nBytes)
{
	CFileIO fio;
  char* buffer = NULL;
  *nBytes = CFileIO::GetFileSize(Filename);
  if (nBytes != NULL)
  {
    if (fio.OpenFileRead(Filename, ios::binary))
    {
      buffer = new char[*nBytes+1];
      if (buffer != NULL)
      {
        fio.ReadBytes(buffer, *nBytes);
        buffer[*nBytes] = 0;
      }
    }
    fio.CloseFile();
  }
  return buffer;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   WriteFile
//
// Parameter: char* Filename - Name der Datei
//
//            int   nBytes    - Anzahl der Bytes die gelesen wurden (Größe
//                             der Datei).
//
//            const void* - Zeiger auf Puffer, dessen Daten geschrieben
//                          werden
//
//
//---------------------------------------------------------------------------

bool CFileIO::WriteFile(const char* Filename, const void* buffer, int nBytes)
{
	bool suc = false;
	CFileIO fio;

  if (nBytes > 0)
  {
    if (fio.OpenFileWrite(Filename, ios::binary))
    {
			fio.WriteBytes(buffer, nBytes);
			suc = true;
			fio.CloseFile();
    }
  }
  return suc;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   StaticWriteFile
//
//
//---------------------------------------------------------------------------

bool CFileIO::StaticWriteFile(const std::string Filename, const void* Data, int DataSize)
{
  bool r;
	ofstream outfile(Filename.c_str(), ios::binary);
	if (outfile.good())
	{
		outfile.write((const char*) Data, DataSize);
		outfile.close();
		r = true;
	}
	else
	{
	  r = false;
	}
	
	return r;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   WriteBytes
//
//
//
// Einlesen einer Anzahl von Bytes aus einer geöffneten Datei
//
//---------------------------------------------------------------------------

bool CFileIO::WriteBytes(const void* buffer, int nBytes)
{
  if (mOpenWrite)
  {
    mFs->write((const char*) buffer, nBytes);
  }
  return mOpenWrite;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   WriteString
//
//
//---------------------------------------------------------------------------

bool CFileIO::WriteString(const char* str)
{
 	return WriteBytes(str, strlen(str));
}

#if 0
//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReplaceAllStrings
//
//
//---------------------------------------------------------------------------

class CStrpos : public CListElem<CStrpos>
{
	public:

		char* mPos;
};

class CStrposList : public CList<CStrpos>
{
};


static void CopyStringBytes(char* dst, char* src, int num)
{
	int i;
	for (i = 0; (i < num) && (*src != 0); i++)
	{
		*(dst++) = *(src++);
	}
	*dst = 0;
}

char* CFileIO::ReplaceAllStrings(char* SrcString, char* SearchString, char* SubString, int* Count)
{
	CStrposList slist;
	int   cnt = 0;
	char* pos;
	char* StartPos  = SrcString;
	int   LenSearch = strlen(SearchString);
	int   LenSub    = strlen(SubString);
	int   LenNew;
	char* NewString = NULL;

	do
	{
		pos = strstr(StartPos, SearchString);
		if (pos != NULL)
		{
			CStrpos* se = slist.NewListElem();
			se->mPos = pos;
			StartPos = pos + LenSearch;
			cnt++;
		}
	}
	while (pos != NULL);

	*Count = cnt;

	if (cnt > 0)
	{
		LenNew = strlen(SrcString) + (cnt * (LenSub - LenSearch));

		NewString = new char[LenNew+1];
		char* NewStringPos = NewString;

		StartPos = SrcString;
		for (CStrpos* se = slist.GetFirst();
									se != NULL;
									se = slist.GetNext())
		{

			int len = se->mPos - StartPos;
			CopyStringBytes(NewStringPos, StartPos, len);
			NewStringPos += len;
			CopyStringBytes(NewStringPos, SubString, LenSub);
			NewStringPos += LenSub;

			StartPos = se->mPos + LenSearch;
		}
		CopyStringBytes(NewStringPos, StartPos, strlen(StartPos));
		slist.ClearList();
	}
	else
	{
		NewString = new char[strlen(SrcString) + 1];
		strcpy(NewString, SrcString);
	}

	return NewString;
}
#endif

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadBytes
//
// Parameter: void* buffer - Zeiger auf Puffer, in den die Datei eingelesen
//                           werden
//
//            int nBytes   - Anzahl der Bytes, die eingelesen werden sollen
//
// Return:    true  - Daten wurden eingelesen
//            false - Daten konnten nicht eingelesen werden
//
//
//
// Einlesen einer Anzahl von Bytes aus einer geöffneten Datei
//
//---------------------------------------------------------------------------

bool CFileIO::ReadBytes(void* buffer, int nBytes)
{
  if (mOpenRead)
  {
    mFs->read((char*) buffer, nBytes);
  }
  return mOpenRead;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadLine
//
// Einlesen einer Text-Zeile aus einer Datei
// Die Datei muss im Binär-Modus geöffnet sein
//
//---------------------------------------------------------------------------

bool CFileIO::ReadLine(vector<char>* Line)
{
	char ch;
	bool EndLine = false;
	bool IsEof = false;

	Line->clear();
	do
	{
		if (mFs->get(ch))
		{
			if (ch != 0x0D)
			{
				if (ch != 0x0A)
				{
        	Line->push_back(ch);
				}
				else
				{
					EndLine = true;
				}
			}
		}
		else
		{
			EndLine = true;
			IsEof = true;
		}
	}
	while (!EndLine);
	return !IsEof;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadLine
//
// Einlesen einer Text-Zeile aus einer Datei
// Die Datei muss im Binär-Modus geöffnet sein
//
//---------------------------------------------------------------------------

bool CFileIO::ReadLine(string* Line)
{
	char ch;
	bool EndLine = false;
	bool IsEof = false;
	int Limit = 200;
	int n = 0;

	Line->clear();
	do
	{
		ch = mFs->get();
		if (!mFs->eof())
		{
			if (ch != 0x0D)
			{
				if (ch != 0x0A)
				{
        	Line->push_back(ch);
				}
				else
				{
					EndLine = true;
				}
			}
		}
		else
		{
			EndLine = true;
			IsEof   = true;
		}
	}
	while (!EndLine);
	return !IsEof;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadLine
//
// Einlesen einer Text-Zeile aus einer Datei
// Die Datei muss im Binär-Modus geöffnet sein
//
//---------------------------------------------------------------------------

bool CFileIO::ReadLine(string* Line, int max)
{
	char ch;
	bool EndLine = false;
	bool IsEof = false;
	int  n = 0;
  if (max < 0) 
  {
   max = 10000; 
  }

	Line->clear();
	do
	{
		ch = mFs->get();
		
		if ((n++ < max) && !mFs->eof())
		{
			if (ch != 0x0D)
			{
				if (ch != 0x0A)
				{
        	Line->push_back(ch);
				}
				else
				{
					EndLine = true;
				}
			}
		}
		else
		{
			EndLine = true;
			IsEof   = true;
		}
	}
	while (!EndLine);
	return !IsEof;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadSplitLine
//
// Parameter:
//
// Return:
//
//
// Lesen von Bytes aus einer Datei
//
//---------------------------------------------------------------------------

bool CFileIO::ReadSplitLine(stringvector* StringVector, char Div)
{
	string LineBuffer;
	bool Eof = ReadLine(&LineBuffer);
	StringVector->clear();
	NStringTool::Split(LineBuffer, StringVector, Div);

	return Eof;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadSplitLine
//
//
//---------------------------------------------------------------------------

bool CFileIO::ReadSplitLine(stringvector* StringVector, char Div, int max)
{
	string LineBuffer;
	bool Eof = ReadLine(&LineBuffer, max);
	StringVector->clear();
	NStringTool::Split(LineBuffer, StringVector, Div);

	return Eof;
}




//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   ReadBytes
//
// Parameter:
//
// Return:
//
//
// Lesen von Bytes aus einer Datei
//
//---------------------------------------------------------------------------

void* CFileIO::ReadBytes(int nBytes)
{
	char* buffer = NULL;
  if (mOpenRead)
  {
		buffer = new char[nBytes];
    mFs->read(buffer, nBytes);
  }
  return buffer;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   CloseFile
//
// Parameter:
//
// Return:
//
//
// Schliessen einer Datei
//
//---------------------------------------------------------------------------

void CFileIO::CloseFile()
{
  if (mOpenWrite || mOpenRead)
  {
    mFs->close();
    mOpenRead = false;
    mOpenWrite = false;
    delete mFs;
 	  mFs = NULL;
  }
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   SetReadPos
//
//
//
//
//---------------------------------------------------------------------------

bool CFileIO::SetReadPos(int pos)
{
	mFs->seekg(pos, ios::beg);
	return (int) mFs->tellg() == pos;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   SetReadPos
//
//
//
//
//---------------------------------------------------------------------------

bool CFileIO::SetReadPos2(int pos, SEEKDIR where)
{
	mFs->seekg(pos, where);
	return (int) mFs->tellg() == pos;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   SetReadPos
//
//
//
//
//---------------------------------------------------------------------------

void CFileIO::SetWritePosEnd()
{
	mFs->seekp(0, ios_base::end);
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   SetReadPos
//
//
//
//
//---------------------------------------------------------------------------

void CFileIO::SetWritePos(unsigned int pos)
{
	mFs->seekp(pos, ios_base::beg);
	//mFs->seekg(pos, ios_base::beg);
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   GetReadPos
//
//
//
//
//---------------------------------------------------------------------------

int CFileIO::GetReadPos()
{
	return mFs->tellg();
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   GetWritePos
//
//
//
//
//---------------------------------------------------------------------------

int CFileIO::GetWritePos()
{
	return mFs->tellp();
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		GuessFileFormat
//
// Return:
//
//  EFILETYPE_UNKNOWN,
// 	EFILETYPE_JPG,
// 	EFILETYPE_TIF,
// 	EFILETYPE_PNG,
// 	EFILETYPE_TGA,
// 	EFILETYPE_RAW
//
//---------------------------------------------------------------------------

EFileType CFileIO::GuessFileFormat(const char* Filename)
{
	const char* AppList[] =
	{
		".IDAT",
		".IDX",
		".DAT",		
		".JPG",
		".TIF",
		".PNG",
		".TGA",
		".RAW",
		".GIF",		
		NULL
	};

	int  FileType;
	char FilenameCopy[256];
	strcpy(FilenameCopy, Filename);

	if (strlen(FilenameCopy) > 4)
	{
		for (int i = 0; FilenameCopy[i] != 0; i++)
		{
			FilenameCopy[i] = toupper(FilenameCopy[i]);
		}

		for (int i = 0; AppList[i] != NULL; i++)
		{
			char* FilenameCopyPtr = FilenameCopy + strlen(FilenameCopy) - strlen(AppList[i]);
			if (strcmp(FilenameCopyPtr, AppList[i]) == 0)
			{
				return (EFileType) (i+1);
			}
		}
	}
	return EFILETYPE_UNKNOWN;
}



//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		ReplaceApp
//
//---------------------------------------------------------------------------

char* CFileIO::ReplaceApp(const char* Filename, const char* neuApp)
{
  char  NameBuffer[512];
  char* NeuFilename;

  strcpy(NameBuffer, Filename);
  char* Punkt = strrchr(NameBuffer, '.');

  if (Punkt != NULL) *Punkt = 0;

  strcat(NameBuffer, neuApp);

  NeuFilename = new char[strlen(NameBuffer) + 1];

  strcpy(NeuFilename, NameBuffer);

  return NeuFilename;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		Strcmpx
//
//---------------------------------------------------------------------------

bool CFileIO::Strcmpx(const char* s1, const char* s2)
{
	if ((s1 == NULL) || (s2 == NULL))
	{
		return false;
	}

	char* bstr1 = new char[strlen(s1) + 1];
	char* bstr2 = new char[strlen(s2) + 1];

	strcpy(bstr1, s1);
	strcpy(bstr2, s2);

	for (char* c1 = bstr1; *c1 != 0; c1++)
	{
		*c1 = toupper(*c1);
	}
	for (char* c1 = bstr2; *c1 != 0; c1++)
	{
		*c1 = toupper(*c1);
	}

	int s = strcmp(bstr1, bstr2);

	delete[] bstr2;
	delete[] bstr1;

	return s == 0;
}

#if 0
//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		DebugFile
//
//---------------------------------------------------------------------------


bool CFileIO::DebugFile(const std::string& filename)
{
  bool r = false;
  CFileIO fio;
  int ln = 0;
  
  gdstr << "========== CFileIO::DebugFile START:" << filename;
  gdlog();
  
  if (fio.OpenFileRead(filename.c_str(), ios::binary))
  {
    std::string Line;
     
    
    while (fio.ReadLine(&Line, -1))
    {
      gdstr << "  " << ++ln << Line;
      gdlog();
    }
    fio.CloseFile();
    r = true;
  }
  else  
  {
    gdstr << "file not found:" << filename;
    gdlog();
    
  }
  gdstr << "========== CFileIO::DebugFile END";
  gdlog();  
  
  return r;
}

#endif

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		ResolveWildCard
//
//---------------------------------------------------------------------------

bool CFileIO::ResolveWildCard(const char* Dir, const char* WildCard, stringvector* FileList)
{
	struct stat FileInfo;
  struct dirent *CurrentFile;
  DIR*   Directory;
  char** DirList = NULL;
  bool   r = false;

	Directory = opendir(Dir);

	//cout << "CFileIO::ResolveWildCard Dir=" << Dir << " Wildcard=" << WildCard << endl;
	
//	gdstr << "CFileIO::ResolveWildCard Dir=" << Dir << " Wildcard=" << WildCard;
//	gdlog();
	

	if (WildCard[0] == '*')
	{
		WildCard++;
	}
	FileList->clear();

	if (Directory != NULL)
	{
		r = true;
		Directory = opendir(Dir);

		int i = 0;
		while ((CurrentFile = readdir(Directory)) != NULL)
		{
			char* EndString = CurrentFile->d_name + (strlen(CurrentFile->d_name) - strlen(WildCard));

			if (Strcmpx(EndString, WildCard))
			{
				string Filename = CurrentFile->d_name;
				FileList->push_back(Filename);
			}

		//cout << "File:" << CurrentFile->d_name << endl;
		}
		closedir(Directory);
	}
	return r;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		StripExtension
//
// Liefert den Dateinamen ohne Datei-Anhang zurueck
//
//---------------------------------------------------------------------------

string CFileIO::StripExtension(const char* FullPathName)
{
	string OutString;
	char* TmpName = NewString(FullPathName);
  char* slash = strrchr(TmpName, '.');

  if (slash != NULL)
  {
		*slash = 0;
		OutString = TmpName;
	}
  else
  {
		OutString = FullPathName;
	}
	return OutString;
}

//---------------------------------------------------------------------------
//
//
// Klasse:  CFileIO
// Methode: Fill0
//
//   Integer-Zahl in String umwandeln und dabei mit Nullen
//   auffuellen.
//
//   int i - Zahl
//   int n - Anzahl der Stellen
//
//---------------------------------------------------------------------------

const char* CFileIO::Fill0(int i, int n)
{
	static char strbuffer[16];
	return Fill0(strbuffer, i, n);
}

//---------------------------------------------------------------------------
//
//
// Klasse:  CFileIO
// Methode: Fill0
//
//   Integer-Zahl in String umwandeln und dabei mit Nullen
//   auffüllen.
//
//   int i - Zahl
//   int n - Anzahl der Stellen
//
//---------------------------------------------------------------------------

string CFileIO::Fill0str(int i, int n)
{
	string fstr = Fill0(i, n);
	return fstr;
}

//---------------------------------------------------------------------------
//
//
// Klasse:  CFileIO
// Methode: Fill0
//
//   Integer-Zahl in String umwandeln und dabei mit Nullen
//   auffüllen.
//
//   int i - Zahl
//   int n - Anzahl der Stellen
//
//---------------------------------------------------------------------------

const char* CFileIO::Fill0(char* strbuffer, int i, int n)
{
	sprintf(strbuffer, "%d", i);

	char len = strlen(strbuffer);

	if (len < n)
	{
		char strbuffer2[16];

		strcpy(strbuffer2, strbuffer);
		strcpy(strbuffer + (n - len), strbuffer2);

		for (int j = 0; j < n - len; j++)
		{
			strbuffer[j] = '0';
		}
	}
	return strbuffer;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   CreateDir
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CFileIO::CreateDir(const char* NewFolder)
{
 	mkdir(NewFolder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
 	
 	#if 0



	#if defined __WIN32__

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	return CreateDirectory(NewFolder, &sa);

  #elif defined __unix__

	mkdir(NewFolder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	return 1;

	#else
	FEHLER CFileIO::CreateDir
	#endif
	return 0;
	#endif
	
	
	return 1;
	
	
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   NewString
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

char* CFileIO::NewString(const char* OldStr)
{
  char* NewStr = NULL;
  
  if (OldStr != NULL)
  {
		NewStr = new char[strlen(OldStr) + 1];
		strcpy(NewStr, OldStr);
	}
  return NewStr;
}


//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		ExtractFilename
//
//---------------------------------------------------------------------------

char* CFileIO::ExtractFilename(const char* FullPathName)
{
  const char* slash = strrchr(FullPathName, '/');

  if (slash == NULL)
  {
		slash = strrchr(FullPathName, '\\');

		if (slash == NULL)
		{
			return NewString(FullPathName);
		}
	}
  return NewString(slash + 1);
}




//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   WriteFromFile 
//
//
//---------------------------------------------------------------------------

int CFileIO::WriteFromFile(const char* Filename)
{
  CFileIO fio2;
	int FileSize = 0;
	const char* FileData = (const char*) fio2.ReadFile(Filename, &FileSize);

	if (FileData != NULL)
	{
		WriteBytes(FileData, FileSize);
		delete[] FileData;
	}
	return FileSize;
}

//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:   WriteFromFileBytes 
//
// Liest "Size" Bytes an Stelle "Offset" aus der Datei "Filename" 
// und schreibt diese in Datei (Datei muss zum Schreiben geoeffnet sein
//
//---------------------------------------------------------------------------

bool CFileIO::WriteFromFileBytes(const char* Filename, int Offset, int Size)
{
  CFileIO fio2;
  bool r = false;
	
	
	
	if (fio2.OpenFileRead(Filename, ios::binary))
	{	
		char* FileData = new char[Size];
		
		fio2.SetReadPos(Offset);
		fio2.ReadBytes(FileData, Size);
		fio2.CloseFile();
	
 		WriteBytes(FileData, Size);
		delete[] FileData;
		
		r = true;
	}
	return r;
}



//---------------------------------------------------------------------------
//
// Klasse:    CFileIO
// Methode:		ExtractPath
//
//---------------------------------------------------------------------------

const char* CFileIO::ExtractPath(const char* FullPathName)
{
	char* PathName = new char[strlen(FullPathName) + 1];

  strcpy(PathName, FullPathName);

  char* slash = strrchr(PathName, '\\');

 // cout << "Slash=" << slash << endl;

  if (slash != NULL)
  {
		slash[1] = 0;
	}
	else
	{
		slash = strrchr(PathName, '/');
		if (slash != NULL)
		{
			slash[1] = 0;
		}
	}
  return PathName;
}
