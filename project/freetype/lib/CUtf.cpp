//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CUtf.cpp
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************

//
// Inhalt: Implementation der Klasse CUtf
//
//------------------------------------------------------------------------------
// Revisionsgeschichte:
//
// 19.10.2010 erstellt - Martin Steen
//
//------------------------------------------------------------------------------

#include <map>
#include <iostream>
#include <string>
#include <set>

#include <CUtf.h>
#include <cstdio>
#include <cstring>
#include <CHexConv.h>
#include <NStringTool.h>
#include "Utf8Table.h"


using namespace std;



struct SHtag
{
    const char* mHtag;  // HTML-Tag z.B. "amp"
    int			mUtag;  // UTF8-Tag
};

struct SHtmlUtf8
{
    SHtmlUtf8(std::string t)
    {
        mHtmlTag = t;
    }


    SHtmlUtf8(const SHtag& htag)
    {
        mHtmlTag = htag.mHtag;
        mUtf8Code = htag.mUtag;
    }


    bool operator<(const SHtmlUtf8& k) const
    {
        return mHtmlTag < k.mHtmlTag;
    }


    std::string mHtmlTag;
    int			mUtf8Code;
};



static bool sInit = false;
static map<int, int> Utf8Map;
static set<SHtmlUtf8> HtmlTagSet;



static SHtag sHtmlTagTable[] =
{
    { "amp", 0x26 },
    { NULL,	   -1 }
};


//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:		InitTable
//
//
//---------------------------------------------------------------------------

static void InitTable()
{
    if (!sInit)
    {
        //
        // UTF8-Tablellen initialisieren
        //

        int TableSize = sizeof(sUtf8Table) / sizeof(int);
        //cout << "TableSize=" << TableSize << endl;

        for (int i = 0; i < TableSize; i += 2)
        {
            int Index = sUtf8Table[i+1];
            int Value = sUtf8Table[i];
            Utf8Map[Index] = Value;

            //cout << "Index:" << Index << " Value:" << Value << endl;
        }
        sInit = true;


        //
        // HTML-Tags initialisieren
        //


        for (int i = 0; sHtmlTagTable[i].mHtag != NULL; i += 1)
        {
            SHtmlUtf8 htag(sHtmlTagTable[i]);
            HtmlTagSet.insert(htag);
        }
    }

    // Test

    /*
     * cout.setf(ios::hex, ios::basefield);
     * cout << "Utf8Map[0xFC]=" << Utf8Map[0xFC] << endl;
     * cout.setf(ios::dec, ios::basefield);
     */
}


//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:		ParseString
//
// Geparst wird, was nach &# oder &amp;# kommt
//
//---------------------------------------------------------------------------

static void ParseString(const char** XmlPtr, string* Utf8Str)
{
    //cout << "ParseString start " <<  (*XmlPtr)[0] << endl;


    //gdstr << "ParseString:" <<  (*XmlPtr)[0];
    //gdlog();


    if ((*XmlPtr)[0] == 'x') // Utf / Hexcode
    {
        string HexStr;

        for (*XmlPtr += 1; (**XmlPtr != 0) && (**XmlPtr != ';'); *XmlPtr += 1)
        {
            HexStr.push_back(**XmlPtr);
        }
        //GlobalDebugT("HexStr=", HexStr, DBG_TEMP2);


        if (HexStr.size() > 0)
        {
            int XmlCode = CHexConv::StringToInt(HexStr.c_str());
            //GlobalDebugT("XmlCode=", XmlCode, DBG_TEMP2);


            int Utf8Code = Utf8Map[XmlCode];

            /*
             * cout.setf(ios::hex, ios::basefield);
             * cout << " HexStr=" << HexStr
             *       << " XmlCode=" << XmlCode
             *       << " Utf8Code=" << Utf8Code << endl;
             * cout.setf(ios::dec, ios::basefield);
             */


            char HiByteUtf = Utf8Code >> 8;
            char LoByteUtf = Utf8Code & 0xFF;

            Utf8Str->push_back(HiByteUtf);
            Utf8Str->push_back(LoByteUtf);
        }
    }
    else // UTF Dezimal Code
    if (((*XmlPtr)[0] >= '0') && ((*XmlPtr)[0] <= '9'))
    {
        string DecStr;



        for ( ; (**XmlPtr != 0) && (**XmlPtr != ';'); *XmlPtr += 1)
        {
            DecStr.push_back(**XmlPtr);
        }

        //gdstr << "DecStr=" << DecStr;
        //gdlog();


        if (DecStr.size() > 0)
        {
            bool SkipConvert = false;
            int XmlCode = NStringTool::Cast<int>(DecStr);

            switch (XmlCode)
            {
                case 150:

                    XmlCode = 173;
                    //Utf8Str->push_back('-');
                    //SkipConvert = true;
                    break;
            }


            if (!SkipConvert)
            {
                int Utf8Code = Utf8Map[XmlCode];

                /*
                 *          cout.setf(ios::hex, ios::basefield);
                 *          cout << "XmlCode=" << XmlCode << endl;
                 *          cout << "Utf8Code=" << Utf8Code << endl;
                 *          cout.setf(ios::dec, ios::basefield);
                 */
                if (Utf8Code == 0)
                {
                    Utf8Str->push_back(' ');
                }
                else
                {
                    char HiByteUtf = Utf8Code >> 8;
                    char LoByteUtf = Utf8Code & 0xFF;

                    Utf8Str->push_back(HiByteUtf);
                    Utf8Str->push_back(LoByteUtf);
                }
            }


/*
 *          switch (XmlCode)
 *          {
 *              case 150: // Bindestrich (ndash)
 *
 *                  Utf8Str->push_back(0x20);
 *                  Utf8Str->push_back(0x13);
 *                  break;
 *
 *          }
 */
        }
    }

//  gdstr << "ParseString OK";
//	gdlog();
}


//---------------------------------------------------------------------------
//
// Klasse:    CUtf
// Methode:		XmlToUtf8
//
// Parameter: XmlStr  - String mit Zeichencodierung im HTML/XMl-Format
//            Utf8Str - String im Utf8-Format (Rueckgabe)
//
//---------------------------------------------------------------------------


void CUtf::XmlToUtf8(const char* XmlStr, string* Utf8Str)
{
    InitTable();
    for (const char* XmlPtr = XmlStr; *XmlPtr != 0; XmlPtr++)
    {
//		gdstr << "x=" << XmlPtr[0];
        //gdlog();

#if 1
        if (strncmp(XmlPtr, "&#", 2) == 0)
        {
            XmlPtr += 2;
            ParseString(&XmlPtr, Utf8Str);
        }

/*
 *      else
 *      if (strncmp(XmlPtr, "&", 1) == 0)
 *      {
 *          string CodeStr;
 *
 *          for (XmlPtr += 1; (*XmlPtr != 0) && (*XmlPtr != ';'); XmlPtr++)
 *          {
 *              CodeStr.push_back(*XmlPtr);
 *          }
 *
 *
 *          SHtmlUtf8 HtmlTag(CodeStr);
 *
 *          set<SHtmlUtf8>::iterator it = HtmlTagSet.find(HtmlTag);
 *
 *          if (it != HtmlTagSet.end())
 *          {
 *              char HiByteUtf = it->mUtf8Code >> 8;
 *              char LoByteUtf = it->mUtf8Code & 0xFF;
 *
 *              if (HiByteUtf != 0)
 *              {
 *                  Utf8Str->push_back(HiByteUtf);
 *              }
 *              Utf8Str->push_back(LoByteUtf);
 *          }
 *
 *      }
 */
        else
#endif
        {
            Utf8Str->push_back(*XmlPtr);
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CUtf
// Methode:		XmlToUtf8
//
// Parameter: XmlStr  - String mit Zeichencodierung im HTML/XMl-Format
//
//
//---------------------------------------------------------------------------

std::string CUtf::XmlToUtf8(const std::string& XmlStr)
{
    std::string Utf8Str;

    XmlToUtf8(XmlStr.c_str(), &Utf8Str);
    return Utf8Str;
}


//---------------------------------------------------------------------------
//
// Klasse:    CUtf
// Methode:		ToUpper
//
// Parameter:
//
//
//---------------------------------------------------------------------------

std::string CUtf::ToUpper(const std::string in_str)
{
    std::string out_str;

    for (std::string::const_iterator it = in_str.begin(); it != in_str.end(); ++it)
    {
        out_str += towupper(*it);
    }
    return out_str;
}
