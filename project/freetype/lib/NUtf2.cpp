//***************************************************************************
//
//
// @PROJECT  :	Phonetik Library
// @VERSION  :	1.0
// @FILENAME :	NUtf2.cpp
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************
//
// Revisionsgeschichte:
//
// 21.07.2016 erstellt - Martin Steen
//
//------------------------------------------------------------------------------

#include <iostream>
#include <cstring>
#include <set>
#include <vector>

using namespace std;

//---------------------------------------------------------------------------
//
//
//---------------------------------------------------------------------------


struct SUtf16Entry
{
	unsigned char  mAnsiChar;
	unsigned short mUtf16value;
	bool operator<(const SUtf16Entry& k) const
	{
		return mUtf16value < k.mUtf16value;
	}
};



static set<SUtf16Entry> StaticUtf16ConvertSet;

//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   ror
//
// a wird um n Stellen nach rechts verschoben.
// Die herausgeschobenen Stellen werden in b hineingeschoben.
//
// Bespiel:
// Vorher: a=00001110 b=00001101
//
// ror(a, b, 2)
//
// Nachher: a=00000011 b=10001101
//
//---------------------------------------------------------------------------

static void ror(unsigned char& a, unsigned char& b, int n)
{
	unsigned char k = 0;
	unsigned char m = 0xFF;

	for (int i = 0; i < n; i++)
	{
		k >>= 1;
		m >>= 1;
    if (a & 1)
    {
			k |= 0x80;
		}
		a >>= 1;
	}
	b &= m; // Ausmaskieren
	b |= k;
}

//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   UtfToLong
//
//---------------------------------------------------------------------------

static unsigned int UtfToLong(const unsigned char* from, int n)
{
	unsigned int Utf32 = 0;
	unsigned char* UtfPtr = (unsigned char*) &Utf32;

	for (int i = 0; i < n; i++)
	{
    UtfPtr[i] = from[n - i - 1];
  }
  return Utf32;
}

//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   ShiftBits2
//
//
//
//
//---------------------------------------------------------------------------

static unsigned int ShiftBits2(const unsigned char* from)
{
	unsigned char tmp[2];
	memcpy(tmp, from, 2);

	tmp[0] &= 0x1F;
  ror(tmp[0], tmp[1], 2);

	//cout << "tmp[0]=" << (unsigned int) tmp[0] << endl;
	//cout << "tmp[1]=" << (unsigned int) tmp[1] << endl;


  return UtfToLong(tmp, 2);
}

//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   ShiftBits3
//
//
//
//
//---------------------------------------------------------------------------

static unsigned int ShiftBits3(const unsigned char* from)
{
	unsigned char  tmp[3];
	memcpy(tmp, from, 3);


	tmp[0] &= 0x0F;
  ror(tmp[1], tmp[2], 2);
  ror(tmp[0], tmp[1], 4);

  return UtfToLong(tmp, 3);
}

//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   ShiftBits4
//
//
//
//
//---------------------------------------------------------------------------

static unsigned int ShiftBits4(const unsigned char* from)
{
	unsigned char  tmp[4];
	memcpy(tmp, from, 4);

	tmp[0] &= 0x07;
  ror(tmp[2], tmp[3], 2);
  ror(tmp[1], tmp[2], 4);
  ror(tmp[0], tmp[1], 6);

  return UtfToLong(tmp, 4);
}


#define SIZE_utf16table 256

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
// Tabelle der ANSI-Zeichen 128 bis 255 (einschliesslich) als UTF16 Wert

static unsigned char utf16table[] =
{
  0xAC,0x20,0x81,0x00,0x1A,0x20,0x92,0x01,0x1E,0x20,0x26,0x20,0x20,0x20,0x21,0x20,0xC6,0x02,0x30,0x20,0x60,0x01,0x39,0x20,0x52,0x01,0x8D,0x00,0x7D,0x01,0x8F,0x00,0x90,
  0x00,0x18,0x20,0x19,0x20,0x1C,0x20,0x1D,0x20,0x22,0x20,0x13,0x20,0x14,0x20,0xDC,0x02,0x22,0x21,0x61,0x01,0x3A,0x20,0x53,0x01,0x9D,0x00,0x7E,0x01,0x78,0x01,0xA0,0x00,
  0xA1,0x00,0xA2,0x00,0xA3,0x00,0xA4,0x00,0xA5,0x00,0xA6,0x00,0xA7,0x00,0xA8,0x00,0xA9,0x00,0xAA,0x00,0xAB,0x00,0xAC,0x00,0xAD,0x00,0xAE,0x00,0xAF,0x00,0xB0,0x00,0xB1,
  0x00,0xB2,0x00,0xB3,0x00,0xB4,0x00,0xB5,0x00,0xB6,0x00,0xB7,0x00,0xB8,0x00,0xB9,0x00,0xBA,0x00,0xBB,0x00,0xBC,0x00,0xBD,0x00,0xBE,0x00,0xBF,0x00,0xC0,0x00,0xC1,0x00,
  0xC2,0x00,0xC3,0x00,0xC4,0x00,0xC5,0x00,0xC6,0x00,0xC7,0x00,0xC8,0x00,0xC9,0x00,0xCA,0x00,0xCB,0x00,0xCC,0x00,0xCD,0x00,0xCE,0x00,0xCF,0x00,0xD0,0x00,0xD1,0x00,0xD2,
  0x00,0xD3,0x00,0xD4,0x00,0xD5,0x00,0xD6,0x00,0xD7,0x00,0xD8,0x00,0xD9,0x00,0xDA,0x00,0xDB,0x00,0xDC,0x00,0xDD,0x00,0xDE,0x00,0xDF,0x00,0xE0,0x00,0xE1,0x00,0xE2,0x00,
  0xE3,0x00,0xE4,0x00,0xE5,0x00,0xE6,0x00,0xE7,0x00,0xE8,0x00,0xE9,0x00,0xEA,0x00,0xEB,0x00,0xEC,0x00,0xED,0x00,0xEE,0x00,0xEF,0x00,0xF0,0x00,0xF1,0x00,0xF2,0x00,0xF3,
  0x00,0xF4,0x00,0xF5,0x00,0xF6,0x00,0xF7,0x00,0xF8,0x00,0xF9,0x00,0xFA,0x00,0xFB,0x00,0xFC,0x00,0xFD,0x00,0xFE,0x00,0xFF,0x00
};

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
// Tabelle der ANSI-Zeichen 128 bis 255 (einschliesslich) als UTF8 Wert

static unsigned char utf8table[] =
{
  0xE2,0x82,0xAC,0xC2,0x81,0xE2,0x80,0x9A,0xC6,0x92,0xE2,0x80,0x9E,0xE2,0x80,0xA6,0xE2,0x80,0xA0,0xE2,0x80,0xA1,0xCB,0x86,0xE2,0x80,0xB0,0xC5,0xA0,0xE2,0x80,0xB9,0xC5,
  0x92,0xC2,0x8D,0xC5,0xBD,0xC2,0x8F,0xC2,0x90,0xE2,0x80,0x98,0xE2,0x80,0x99,0xE2,0x80,0x9C,0xE2,0x80,0x9D,0xE2,0x80,0xA2,0xE2,0x80,0x93,0xE2,0x80,0x94,0xCB,0x9C,0xE2,
  0x84,0xA2,0xC5,0xA1,0xE2,0x80,0xBA,0xC5,0x93,0xC2,0x9D,0xC5,0xBE,0xC5,0xB8,0xC2,0xA0,0xC2,0xA1,0xC2,0xA2,0xC2,0xA3,0xC2,0xA4,0xC2,0xA5,0xC2,0xA6,0xC2,0xA7,0xC2,0xA8,
  0xC2,0xA9,0xC2,0xAA,0xC2,0xAB,0xC2,0xAC,0xC2,0xAD,0xC2,0xAE,0xC2,0xAF,0xC2,0xB0,0xC2,0xB1,0xC2,0xB2,0xC2,0xB3,0xC2,0xB4,0xC2,0xB5,0xC2,0xB6,0xC2,0xB7,0xC2,0xB8,0xC2,
  0xB9,0xC2,0xBA,0xC2,0xBB,0xC2,0xBC,0xC2,0xBD,0xC2,0xBE,0xC2,0xBF,0xC3,0x80,0xC3,0x81,0xC3,0x82,0xC3,0x83,0xC3,0x84,0xC3,0x85,0xC3,0x86,0xC3,0x87,0xC3,0x88,0xC3,0x89,
  0xC3,0x8A,0xC3,0x8B,0xC3,0x8C,0xC3,0x8D,0xC3,0x8E,0xC3,0x8F,0xC3,0x90,0xC3,0x91,0xC3,0x92,0xC3,0x93,0xC3,0x94,0xC3,0x95,0xC3,0x96,0xC3,0x97,0xC3,0x98,0xC3,0x99,0xC3,
  0x9A,0xC3,0x9B,0xC3,0x9C,0xC3,0x9D,0xC3,0x9E,0xC3,0x9F,0xC3,0xA0,0xC3,0xA1,0xC3,0xA2,0xC3,0xA3,0xC3,0xA4,0xC3,0xA5,0xC3,0xA6,0xC3,0xA7,0xC3,0xA8,0xC3,0xA9,0xC3,0xAA,
  0xC3,0xAB,0xC3,0xAC,0xC3,0xAD,0xC3,0xAE,0xC3,0xAF,0xC3,0xB0,0xC3,0xB1,0xC3,0xB2,0xC3,0xB3,0xC3,0xB4,0xC3,0xB5,0xC3,0xB6,0xC3,0xB7,0xC3,0xB8,0xC3,0xB9,0xC3,0xBA,0xC3,
  0xBB,0xC3,0xBC,0xC3,0xBD,0xC3,0xBE,0xC3,0xBF

};

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
//
// Offset eines Zeichens in der Tabelle utf8table
// (die Zeichen in Tabelle utf8table koennen 2 oder 3 Bytes haben)
//

static int utf8offset[] =
{
	0,3,5,8,10,13,16,19,22,24,27,29,32,34,36,38,40,42,45,48,51,54,57,60,63,65,68,70,73,75,77,79,81,83,85,87,89,91,93,95,97,99,101,103,
	105,107,109,111,113,115,117,119,121,123,125,127,129,131,133,135,137,139,141,143,145,147,149,151,153,155,157,159,161,163,165,167,169,
	171,173,175,177,179,181,183,185,187,189,191,193,195,197,199,201,203,205,207,209,211,213,215,217,219,221,223,225,227,229,231,233,235,
	237,239,241,243,245,247,249,251,253,255,257,259,261,263,265,267,269,271
};



//---------------------------------------------------------------------------
//
// namespace: NUtf2
//
//---------------------------------------------------------------------------

namespace NUtf2
{
	//---------------------------------------------------------------------------
	//
	// namespace: NUtf2
	// Methode:
	//
	//
	//
	//
	//---------------------------------------------------------------------------


	int Utf8_char_len(const unsigned char Utf8)
	{
		int Size = 0;
		int n = Utf8;
		//cout << std::hex << "n=" << n << " nibble=" << (Utf8 & 0xF0) << endl;
		if ((Utf8 & 0x80) == 0) // Einstellig, ASCII-Code
		{
			 Size = 1;
		}
		else if ((Utf8 & 0xE0) == 0xC0) // zweistellig, beginnt mit 110
		{
			Size = 2;
		}
		else if ((Utf8 & 0xF0) == 0xE0) // dreistellig, beginnt mit 1110
		{
			Size = 3;
		}
		else if ((Utf8 & 0xF8) == 0xF0) // vierstellig, beginnt mit 11110
		{
			Size = 4;
		}
		return Size;
	}


	//---------------------------------------------------------------------------
	//
	// namespace: NUtf2
	// Methode:
	//
	//
	//
	//
	//---------------------------------------------------------------------------

	unsigned int Utf8_to_Utf32(const unsigned char* Utf8, int* Size)
	{
		 unsigned int Utf32 = 0;
		 *Size = Utf8_char_len(Utf8[0]);

		 /*
		 unsigned int (*ShiftBitsTab[])(const unsigned char*) =
		 {
				ShiftBits2,
				ShiftBits3,
				ShiftBits4
		 };
 		Utf32 = ShiftBitsTab[*Size](Utf8);
 		*/

		switch (*Size)
		{
      case 1:  Utf32 = Utf8[0]; break;
      case 2:  Utf32 = ShiftBits2(Utf8); break;
      case 3:  Utf32 = ShiftBits3(Utf8); break;
      case 4:  Utf32 = ShiftBits4(Utf8); break;
		}
		return Utf32;
	}




	//---------------------------------------------------------------------------
	//
	// namespace: NUtf2
	// Methode  : Utf8_to_Ansi
	//
	//---------------------------------------------------------------------------

	std::string Utf8_to_Ansi(const unsigned char* Utf8, int* Errors)  
	{
    std::string AnsiString;
		vector<unsigned int> Utf32vec;

		if (Errors != NULL)
		{
			*Errors = 0;
		}

		for (int i = 0; Utf8[i] != 0; )
		{
			int Size;
			unsigned int Utf32 = Utf8_to_Utf32(Utf8 + i, &Size);
			i += Size;
			Utf32vec.push_back(Utf32);

		}

		if (StaticUtf16ConvertSet.size() == 0)
		{

			//
			// Tabelle wird initialisiert
			//

			int j = 0;
			for (int i = 0; i < 128; i++)
			{
				SUtf16Entry u16;
				u16.mUtf16value = (utf16table[j+1] << 8) | utf16table[j];
				u16.mAnsiChar = i + 128;
				StaticUtf16ConvertSet.insert(u16);
				j += 2;
			}
		}

		//cout << "StaticUtfConvertSet.size()=" << StaticUtfConvertSet.size() << endl;

    for (vector<unsigned int>::const_iterator it = Utf32vec.begin(); it != Utf32vec.end(); ++it)
    {
			if ((*it & 0xFFFFFF80) == 0) // 7-Bit ASCII
			{
				// Falls das Zeichen ein 7-Bit-Ascii zeichen ist,
				// kann es ohne Umwandlung uebernommen werden

				AnsiString += (char) *it;
			}
			else
			if ((*it & 0xFFFF0000) != 0)
			{
				// Zeichen mit mehr als 16 Bit koennen nicht
				// umgewandelt werden
				// cout << "no 16 bit" << endl;
				if (Errors != NULL)
				{
					*Errors += 1;
				}
				// Error, nur 16 Bit UTF zulaessig
			}
			else
			{
				// Das Zeichen in der Utf-Tabelle finden

				SUtf16Entry se;
				se.mUtf16value = (unsigned short) *it;
				set<SUtf16Entry>::iterator st = StaticUtf16ConvertSet.find(se);
				if (st != StaticUtf16ConvertSet.end())
				{
					AnsiString += st->mAnsiChar;
				}
				else
				{
					cout << "no ansi!" << endl;
					if (Errors != NULL)
					{
						*Errors += 1;
					}
					// Nicht in Tabelle gefunden (keine Ansi-Zeichen?)
				}
			}
		}
		return AnsiString;
	}

	//---------------------------------------------------------------------------
	//
	// namespace: NUtf2
	// Methode:   AnsiChar2Utf8
	//
	//
	//
	//
	//---------------------------------------------------------------------------


	static std::string AnsiChar2Utf8(unsigned char AnsiChar)
	{
		std::string Utf8String;

		int Offset = utf8offset[AnsiChar];
		Utf8String += utf8table[Offset];
		Utf8String += utf8table[Offset + 1];

	/*
			int Size = Utf8_char_len(utf8table[j]);

			if (i == AnsiChar)
			{

				Utf8String += utf8table[j];
				Utf8String += utf8table[j + 1];
				break;
			}
			j += Size;

		}
	*/
		return Utf8String;
	}
	
  //---------------------------------------------------------------------------
	//
	// namespace: NUtf2
	// Methode  : ResolveAnsiUnmlauteString
	//
	//---------------------------------------------------------------------------
	
	
	std::string ResolveAnsiUmlauteString(const std::string& s)
	{
	  std::string rs;
	  
	  // E4 F6 FC C4 D6 DC
	  
	  for (int i = 0; i < s.size(); i++)
	  {
	    switch ((unsigned char) s[i])
	    {
	      case 0xE4: 
	      case 0xC4:
	      
		      rs += "AE"; 
		      break;
		      
	      case 0xF6: 
	      case 0xD6:
	      
	      	rs+= "OE"; 
	      	break;
	      		      	
	      case 0xFC: 
	      case 0xDC:
	      
	      	rs += "UE";       
	      	break;
	      	
	     case 0xDF: // sz
	     
	     		rs += "SS";
	     		break; 	
	      	
	      default:
	      
	      	rs += s[i];
	      	break;
	      	
	    
	    
	    }
	  }
	  return rs;	   
}



	//---------------------------------------------------------------------------
	//
	// namespace: NUtf2
	// Methode  : Ansi_To_Utf8
	//
	//---------------------------------------------------------------------------

	std::string Ansi_to_Utf8(const unsigned char* Ansi, int* Errors)
	{
			string Utf8String;
			for (int i = 0; Ansi[i] != 0; i++)
			{

				if ((Ansi[i] & 0x80) == 0) // Kleiner als 128
				{
					Utf8String += Ansi[i];
				}
				else
				{
					Utf8String += AnsiChar2Utf8(Ansi[i] - 128);
				}
			}

/*
      {
				int j = 0;
				for (int i = 0; i < 128; i++)
				{
					int Size = Utf8_char_len(utf8table[j]);

					cout << j << ",";
					j += Size;

				}
			}
			cout << endl;
*/

/*
			int j = 0;
			for (int i = 0; i < 128; i++)
			{
				int Utf8value;
				int Size = Utf8_char_len(utf8table[j]);

				switch (Size)
				{
					case 1: Utf8value =  utf8table[j]; break;
					case 2: Utf8value = (utf8table[j+1] << 8) | utf8table[j]; break;
					case 3: Utf8value = (utf8table[j+2] << 16) | (utf8table[j+1] << 8) | utf8table[j]; break;
					case 4: Utf8value = (utf8table[j+3] << 24) | (utf8table[j+2] << 16) | (utf8table[j+1] << 8) | utf8table[j]; break;
				}
				cout << "i=" << i << " Size=" << Size << " v=" << Utf8value << "," << endl;

				j += Size;
			}
*/




      return Utf8String;

	}


};
