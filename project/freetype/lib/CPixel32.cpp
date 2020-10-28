//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CPixel32.cpp
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************
//
// Inhalt: Deklaration der Klasse CPixel32
//
//------------------------------------------------------------------------------
// Revisionsgeschichte:
//
// 07.10.2009 erweitert - Martin Steen
//
//---------------------------------------------------------------------------

#include <CPixel32.h>

//---------------------------------------------------------------------------
//
// Klasse:    CPixel32
// Methode:		operator==
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CPixel32::operator==(const CPixel32& p2) const
{
	return *((int*) rgba) == *((int*) p2.rgba);
}

//---------------------------------------------------------------------------
//
// Klasse:    CPixel32
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CPixel32::operator!=(const CPixel32& p2) const
{
	return *((int*) rgba) != *((int*) p2.rgba);

}

//---------------------------------------------------------------------------
//
// Klasse:    CPixel32
// Methode:   SwapRedBlue
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CPixel32::SwapRedBlue()
{
	unsigned char swap = rgba[0];

	rgba[0] = rgba[2];
	rgba[2] = swap;
}

//---------------------------------------------------------------------------
//
// Klasse:    CPixel32
// Methode:   Set
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CPixel32::Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	rgba[0] = r;
	rgba[1] = g;
	rgba[2] = b;
	rgba[3] = a;
}

//---------------------------------------------------------------------------
//
// Klasse:    CPixel32
// Methode:   Brightness
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

int CPixel32::Brightness()
{
	return (int) rgba[0] + rgba[1] + rgba[2];
}

//---------------------------------------------------------------------------
//
// Klasse:    CPixel32
// Methode:		GetInt
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

int CPixel32::GetInt()
{
	return *((int*) rgba);
}

