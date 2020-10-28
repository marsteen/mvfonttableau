//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	NStringTool.h
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************


#ifndef NSTRINGTOOL_H
#define NSTRINGTOOL_H

#include <sstream>
#include <string>
#include <vector>

#ifndef stringvector
typedef std::vector<std::string>	stringvector;
#endif


#ifndef charvector
typedef std::vector<char>			charvector;
#endif


namespace NStringTool
{
    int Split(std::string InputString, stringvector* ResultVector, char SplitChar);
    int Split(std::string InputString, stringvector* ResultVector, char SplitChar, const char b0, const char b1);
    bool Split(std::string InputString, std::string& s1, std::string& s2, char SplitChar);
    int Split(std::string InputString, stringvector* ResultVector, const char* SplitChars);
    int SplitByString(std::string InputString, stringvector* ResultVector, const std::string spstring);
    std::string UnSplit(const stringvector& sp, char SplitChar);
    int ReplaceStrings(std::string& InputString, const std::string OldString, const std::string NewString);
    void RemoveLeading(std::string& InString, char lchar);
    std::string RemoveChars(const std::string str, const std::string rm);
    std::string FileBasename(const std::string InStr);



    int ReadLine(std::ifstream& ifile, std::string* Line);

    std::string Cut(const std::string& InStr, char CutChar);
    bool Compare(const std::string& str1, const std::string& str2, bool IgnoreCase = false);
    std::string Trim(std::string str, const char* trimChars);
    std::string TrimValid(std::string InStr, const char* validChars);
    std::string FilterUmlaut(const std::string& InStr);
    std::string RemoveExtension(const std::string& InStr);
    std::string GetExtension(const std::string& InStr);
    bool Strcmpx(const char* s1, const char* s2);
    std::string Shorten(const std::string& InString, const std::string Punkte, int Limit);
    void RemoveUmlaut(char* c1, bool ToUpper);
    std::string Last(const std::string& InStr, unsigned int n);
    std::string First(const std::string& InStr, unsigned int n);
    std::string ReplaceApp(std::string InStr, std::string NewApp);
    std::string ExtractFilename(const std::string FullPathName);
    std::string ExtractPath(const std::string FullPathname);
    std::string StripExt(const std::string Filename);
    std::string ToUpper(const std::string& InString);
    std::string LineBreak(const std::string& str, int n);
    std::string WordBreak(const std::string& str, int n);
    int Contains(const std::string& str1, const std::string& str2);
    std::string Pathname(const std::string& FilenameStr, char PathDiff);



    void Trim(stringvector& sv, const char* trimChars);
    void TrimValid(stringvector& sv, const char* validChars);

    template<typename T>
    std::string toString(T i)
    {
        //std::stringstream istr;
        //istr <<  i;
        //return istr.str();
    }


    template<typename T>
    T Cast(const std::string& str)
    {
        T Value = 0;

        if (str.size() > 0)
        {
            std::stringstream istr(str);
            istr >> Value;
        }
        return Value;
    }


    template<typename T>
    T Cast(const std::string& str, int Precision)
    {
        T Value = 0;

        if (str.size() > 0)
        {
            std::stringstream istr(str);
            //istr.precision(Precision);
            istr >> Value;
        }
        return Value;
    }
}


#endif
