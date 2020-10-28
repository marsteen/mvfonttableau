//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	NStringTool.cpp
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************
//
// Inhalt: Implementation von NStringTool
//
//------------------------------------------------------------------------------
// Revisionsgeschichte:
//
// 02.11.2007 erstellt - Martin Steen
// 20.07.2010 erweitert - Martin Steen
//
//------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string.h>
#include <ctype.h>


#include <NStringTool.h>

namespace NStringTool
{
	//---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:    Split
	//
	//---------------------------------------------------------------------------

	int Split(std::string InputString, stringvector* ResultVector, char SplitChar)
	{
		int OldIndex = 0;
		int NewIndex;

		do
		{
			NewIndex = InputString.find(SplitChar, OldIndex);

			if (NewIndex != std::string::npos)
			{
				ResultVector->push_back(InputString.substr(OldIndex, NewIndex - OldIndex));
				OldIndex = NewIndex + 1;
			}
			else
			{
				ResultVector->push_back(InputString.substr(OldIndex, InputString.size() - OldIndex));
			}
		}
		while (NewIndex != std::string::npos);

		return ResultVector->size();
	}

	//---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:    UnSplit
	//
	//---------------------------------------------------------------------------
	
	std::string UnSplit(const stringvector& sp, char SplitChar)
	{
	  std::string rs = sp[0];
	  
	  
	  for (int i = 1; i < sp.size(); i++)
	  {
	    rs += SplitChar;	     
	    rs += sp[i];
	  }	
	  return rs;
	}
	
	
	//---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:    SplitByString
	//
	//---------------------------------------------------------------------------

	int SplitByString(std::string InputString, stringvector* ResultVector, const std::string spstring)
	{
		int OldIndex = 0;
		int NewIndex;

		do
		{
			NewIndex = InputString.find(spstring, OldIndex);

			if (NewIndex != std::string::npos)
			{
				ResultVector->push_back(InputString.substr(OldIndex, NewIndex - OldIndex));
				OldIndex = NewIndex + spstring.size();
			}
			else
			{
				ResultVector->push_back(InputString.substr(OldIndex, InputString.size() - OldIndex));
			}
		}
		while (NewIndex != std::string::npos);

		return ResultVector->size();
	}
	
	//---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:    ReplaceStrings
	//
	//---------------------------------------------------------------------------

	int ReplaceStrings(std::string& InputString, const std::string OldString, const std::string NewString)
	{
	  stringvector sp;
	  
	  
	  SplitByString(InputString, &sp, OldString);
	  
	  InputString.clear();
	  for (int i = 0; i < sp.size(); i++)
	  {
	    InputString += sp[i];
	    
	    if (i < sp.size()-1)
	    {	    
	      InputString += NewString;
	    }
	  }
	  return sp.size();
	}
	

  //---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:    Split
	//
	//---------------------------------------------------------------------------

	int Split(std::string InputString, stringvector* ResultVector, const char* SplitChars)
	{
		int OldIndex = 0;
		int SplitCharAnz = strlen(SplitChars);
		bool finished = false;
		int n  = 0;

		while (!finished)
		{
			int MinIndex = (int) std::string::npos;
			
			int sc = -1;
			for (int i = 0; i < SplitCharAnz; i++)
			{					
				int NewIndex = InputString.find(SplitChars[i], OldIndex);
				
				if (NewIndex != std::string::npos)
				{				
					if ((NewIndex < MinIndex) || (MinIndex ==  std::string::npos))
					{
						MinIndex = NewIndex;
						sc = SplitChars[i];
					}
				}
			}
			
			//
			//
			//

			if (sc >= 0)
			{
				
				std::string part = InputString.substr(OldIndex, MinIndex - OldIndex + 1);
				
				ResultVector->push_back(part);
				
				OldIndex = MinIndex + 1;
			}
			else
			{
				ResultVector->push_back(InputString.substr(OldIndex, InputString.size() - OldIndex));
				finished = true;
			}
			n++;
		}

		return ResultVector->size();
	}	
	
  //---------------------------------------------------------------------------
  //
  // Namespace:  NStringTool
  // Methode:    Split
  //
  // Splittet einen String in zwei Teile durch das Zeichen SplitChar
  //
  // Return     true = Zeichen gefunden
  //            false = Zeichen NICHT gefunden (kein Ergebnis)
  //
  //---------------------------------------------------------------------------

  bool Split(std::string InputString, std::string& s1, std::string& s2, char SplitChar)
  {
    bool r;
    int OldIndex = 0;
    int NewIndex;

    NewIndex = InputString.find(SplitChar, OldIndex);

    if (NewIndex != std::string::npos)
    {
      s1 = InputString.substr(OldIndex, NewIndex - OldIndex);
      OldIndex = NewIndex + 1;

      s2 = InputString.substr(OldIndex, InputString.size() - OldIndex);

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
	// Namespace:  NStringTool
	// Methode:    Split
	//
	//---------------------------------------------------------------------------

	int Split(std::string InputString, stringvector* ResultVector, char SplitChar, const char b0, const char b1)
	{
		int BrackedOpen = 0;
		std::string InputStringTrimmed;

		for (int i = 0; i < InputString.size(); i++)
		{
			char c = InputString.at(i);
			if (c == b0)
			{
				BrackedOpen += 1;
			}
			else
			if (c == b1)
			{
				BrackedOpen -= 1;
			}
			else
			{
				if ((BrackedOpen > 0) && (c == SplitChar))
				{
					InputStringTrimmed += '*';
				}
				else
				{
					InputStringTrimmed += c;
				}
			}
		}

		//std::cout << "InputStringTrimmed=" << InputStringTrimmed << std::endl;
		int r = Split(InputStringTrimmed, ResultVector, SplitChar);

		for (stringvector::iterator it = ResultVector->begin(); it != ResultVector->end(); ++it)
		{
			for (int i = 0; i < it->size(); i++)
			{
				if ((*it)[i] == '*')
				{
					(*it)[i] = SplitChar;
				}
			}

		}
		return r;
	}
	
	//---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:    ReplaceApp
	//
	//---------------------------------------------------------------------------

	std::string ReplaceApp(std::string InStr, std::string NewApp)
	{
		std::string NewName;
		int PointPos= InStr.rfind('.');
		
		if (PointPos != std::string::npos)
		{
			NewName = InStr.substr(0, PointPos) + NewApp;
		}
		else
		{
			NewName = InStr + NewApp;
		}
		return NewName;
	}
	
	//---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:    FileBasename
	//
	//---------------------------------------------------------------------------

	std::string FileBasename(const std::string InStr)
	{
		std::string NewName;
		int PointPos= InStr.rfind('.');
		
		if (PointPos != std::string::npos)
		{
			NewName = InStr.substr(0, PointPos);
		}
		else
		{
		  NewName = InStr;	
		}
		return NewName;
	}
	
		
//---------------------------------------------------------------------------
//
// Klasse:    NStringTool
// Methode:		RemoveChars
//
//
// Entfernen der Zeichen in rm aus str
//
//---------------------------------------------------------------------------

std::string RemoveChars(const std::string str, const std::string rm)
{
  std::string erg;
  for (int i = 0; i < str.size(); i++)
  {
    if (rm.find(str[i]) == std::string::npos)
    {
      erg += str[i];
    }  
  }
  return erg;
}		
		
//---------------------------------------------------------------------------
//
// Klasse:    NStringTool
// Methode:		ExtractPath
//
//---------------------------------------------------------------------------

std::string ExtractPath(const std::string FullPathname)
{
  
  std::string Filename;
  int pos = FullPathname.rfind('/');
  
  if (pos != std::string::npos)    
  {
    Filename = FullPathname.substr(0, pos);  
	}
	else
	{
	  Filename = FullPathname;
	}
  return Filename;
}


//---------------------------------------------------------------------------
//
// Klasse:    NStringTool
// Methode:		ExtractFilename
//
//---------------------------------------------------------------------------

std::string ExtractFilename(const std::string FullPathname)
{
  
  std::string Filename;
  int pos = FullPathname.rfind('/');
  
  if (pos != std::string::npos)    
  {
    Filename = FullPathname.substr(pos + 1);  
	}
	else
	{
	  Filename = FullPathname;
	}
  return Filename;
}


//---------------------------------------------------------------------------
//
// Klasse:    NStringTool
// Methode:		StripExt
//
// Entfernen der Datei-Extension
//   vorher: datei.txt
//  nachher: datei
//
//---------------------------------------------------------------------------

std::string StripExt(const std::string Filename)
{
  
  std::string FilenameStripped;
  int pos = Filename.rfind('.');
  
  if (pos != std::string::npos)    
  {
    FilenameStripped = Filename.substr(0, pos);  
	}
	else
	{
	  FilenameStripped = Filename;
	}
  return FilenameStripped;
}
	
		



	//---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:    Trim
	//
	//---------------------------------------------------------------------------

	std::string Trim(std::string InStr, const char* trimChars)
	{
		std::string OutStr;
		std::string::size_type StartIndex;
		std::string::size_type EndIndex;

		StartIndex = InStr.find_first_not_of(trimChars);



		if (StartIndex != std::string::npos)
		{
			EndIndex = InStr.find_last_not_of(trimChars);
			if (EndIndex != std::string::npos)
			{
				if (EndIndex >= StartIndex)
				{
					OutStr = InStr.substr(StartIndex, EndIndex - StartIndex + 1);
				}
			}
		}

		return OutStr;
	}



	//---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:    TrimValid
	//
	//---------------------------------------------------------------------------

	std::string TrimValid(std::string InStr, const char* validChars)
	{
		std::string OutStr;
		std::string::size_type StartIndex;
		std::string::size_type EndIndex;

		StartIndex = InStr.find_first_of(validChars);



		if (StartIndex != std::string::npos)
		{
			EndIndex = InStr.find_last_of(validChars);
			if (EndIndex != std::string::npos)
			{
				if (EndIndex >= StartIndex)
				{
					OutStr = InStr.substr(StartIndex, EndIndex - StartIndex + 1);
				}
			}
		}

		return OutStr;
	}

//---------------------------------------------------------------------------
//
// Namespace:  NStringTool
// Methode:    Trim
//
//---------------------------------------------------------------------------

void Trim(stringvector& sv, const char* trimChars)
{
	for (int i = 0; i < sv.size(); i++)
	{
		sv[i] = Trim(sv[i], trimChars);
	}

}

//---------------------------------------------------------------------------
//
// Namespace:  NStringTool
// Methode:    TrimValid
//
//---------------------------------------------------------------------------

void TrimValid(stringvector& sv, const char* validChars)
{
	for (int i = 0; i < sv.size(); i++)
	{
		sv[i] = TrimValid(sv[i], validChars);
	}

}



	//---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:    ReadLine
	//
	//---------------------------------------------------------------------------

	int ReadLine(std::ifstream& ifile, std::string* Line)
	{
		char ch;
		bool EndLine = false;

		Line->clear();
		do
		{
			ifile.get(ch);
			if (!ifile.eof())
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
			}
		}
		while (!EndLine);
		return true;
	}


	//---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:    GetExtension
	//
	//
	//
	//---------------------------------------------------------------------------


	std::string GetExtension(const std::string& InStr)
	{
		std::string::size_type idx = InStr.rfind('.');
		if (idx == std::string::npos)
		{
			return "";
		}
		return InStr.substr(idx);
	}

	
	
	//---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:    RemoveExtension
	//
	//
	//
	//---------------------------------------------------------------------------


	std::string RemoveExtension(const std::string& InStr)
	{
		std::string::size_type idx = InStr.rfind('.');
		if (idx == std::string::npos)
		{
			return InStr;
		}
		return InStr.substr(0, idx);
	}

	//---------------------------------------------------------------------------
	//
	// Namespace:  NStringTool
	// Methode:    Cut
	//
	// Schneidet den InStr VOR CutChar ab
	//
	// Beispiel:
	//
	//   Aus c:\temp\datei.txt wird c:\temp
	//   (Trennzeichen: \)
	//
	//
	//---------------------------------------------------------------------------

	std::string Cut(const std::string& InStr, char CutChar)
	{
		std::string::size_type idx = InStr.rfind(CutChar);
		if (idx == std::string::npos)
		{
			return InStr;
		}
		return InStr.substr(0, idx);
	}


//---------------------------------------------------------------------------
//
// Namespace:  NStringTool
// Methode:    Shorten
//
//
//
//---------------------------------------------------------------------------

std::string Shorten(const std::string& InString, const std::string Punkte, int Limit)
{
	std::string OutString;

	if (InString.size() < Limit)
	{
		OutString = InString;
	}
	else
	{
		OutString = InString.substr(0, Limit) + Punkte;
	}
	return OutString;
}


//---------------------------------------------------------------------------
//
// Namespace:  NStringTool
// Methode:    First
//
// Liefert die ersten n Zeichen eines String
//
//---------------------------------------------------------------------------

std::string First(const std::string& InString, int n)
{
	return InString.substr(n);
}

//---------------------------------------------------------------------------
//
// Namespace:  NStringTool
// Methode:    Last
//
// Liefert die letzten n Zeichen eines String
//
//---------------------------------------------------------------------------

std::string Last(const std::string& InString, int n)
{
	return InString.substr(InString.size() - n, n);
}


//---------------------------------------------------------------------------
//
// Namespace:  NStringTool
// Methode:    LineBreak
//
// Zeilenumbruch nach n Zeichen
//
//---------------------------------------------------------------------------

std::string LineBreak(const std::string& str, int n)
{
	std::string nstr;
	if (str.size() < n)
	{
		return str;
	}
	else
	{
		int k = 0;
		
		while (k < str.size())
		{										
			if (k + n < str.size())
			{				
				nstr += str.substr(k,  n) + '\n';
			}
			else
			{			  
				nstr += str.substr(k,  str.size() - k);
			}
			k += n;					
		}		
	}
	return nstr;
}


/*
struct equal_ignore_case
{
    equal_ignore_case();
    bool operator()(char ch1, char ch2) 
		{
        return toupper(ch1) == toupper(ch2);
    }    
};
*/


void ToUpper(std::string& str)
{
  for (std::string::iterator it = str.begin(); it != str.end(); ++it)
	{
		*it = toupper(*it);
	}
}

/*
static bool equal_ignore_case(char ch1, char ch2)
{
	return toupper(ch1) == toupper(ch2);
}

int Contains(const std::string& str1, std::string& str2)
{
	int r = 0;
	
	if (str2.size() > 0)
	{	
		std::string::const_iterator it = std::search(str1.begin(), str1.end(),  str2.begin(), str2.end(), equal_ignore_case);
		r = ((it == str1.end()) ? 0 : 1);
	}
	
	return r;
}
*/

int Contains(const std::string& str1, const std::string& str2)
{
	int r = 0;
	if (str2.size() > 0)
	{	
		std::string u1 = str1;
		std::string u2 = str2; 
		
		ToUpper(u1);
		ToUpper(u2);

		
		r = ((u1.find(u2) ==  std::string::npos) ? 0 : 1);		
	}
	
	return r;
}



//---------------------------------------------------------------------------
//
// Namespace:  NStringTool
// Methode:    WordBreak
//
// Zeilenumbruch nach n Zeichen 
// Zeilenumbruch \n wird zwischen zwei Worten einfuegt
// 
//---------------------------------------------------------------------------

std::string WordBreak(const std::string& str, int n)
{
	std::string nstr;
	
	if (str.size() < n)
	{
		return str;
	}
	else
	{
		stringvector sp;
		std::string lstr;

		Split(str, &sp, ' ');		
		const int len = sp.size();
		
		for (int i = 0; i < len; i++)
		{			
      if (lstr.size() + sp[i].size() < n)
			{
			  lstr += sp[i];
				if (i < len-1) lstr += " ";
			}
			else
			{
			  nstr += lstr + '\n';			  
			  lstr = sp[i];
				if (i < len-1) lstr += " ";
			}			
		}
		nstr += lstr;
		
		return nstr;
	}		
		
	
	
	
	return nstr;
}




//---------------------------------------------------------------------------
//
// Namespace:  NStringTool
// Methode:    ToUpper
//
// Alle Zeichen gross machen
//
//---------------------------------------------------------------------------

std::string ToUpper(const std::string& InString)
{

  std::string ustr;
  
  for (int i = 0; i < InString.size(); i++)
  {
    ustr += toupper(InString[i]);   
  }
  return ustr;
}

//---------------------------------------------------------------------------
//
// Namespace:  NStringTool
// Methode:    RemoveLeading
//
//
//---------------------------------------------------------------------------

void RemoveLeading(std::string& InString, char lchar)
{
	if (InString[0] == lchar)
	{
	  InString = InString.substr(1);
	}
}


//---------------------------------------------------------------------------
//
// Namespace:  NStringTool
// Methode:    Pathname
//
// Rueckgabe:  Pfadname des Dateiamens
//
//---------------------------------------------------------------------------

std::string Pathname(const std::string& FilenameStr, char PathDiff)
{
	std::string PathStr;

	int SlashIndex = FilenameStr.find_last_of('/');
	if (SlashIndex != std::string::npos)
	{
		PathStr = FilenameStr.substr(0, SlashIndex);
	}
	return PathStr;
}

/*
	//---------------------------------------------------------------------------
	//
	// Namespace: NStringTool
	// Methode:		Strcmpx
	//
	// Vergleich zweier Strings ohne Beruecksichtigung der Gross/Klein-
	// schreibung
	//
	// Return: wie strcmp (0: Strings sind gleich)
	//
	//---------------------------------------------------------------------------

	int Strcmpx(const char* s1, const char* s2)
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

		delete bstr2;
		delete bstr1;

		return s;
	}
	*/


  //
	// End of namespace NStringTool
	//
}






