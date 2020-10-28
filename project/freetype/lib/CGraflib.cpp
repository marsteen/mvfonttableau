//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CGraflib.cpp
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************
//
// Inhalt: Implementation von CGraflib
//
//  Basisklasse fuer verschiedenen Grafikformate
//
//------------------------------------------------------------------------------
// Revisionsgeschichte:
//
// 01.06.2013 erstellt- Martin Steen
//
//------------------------------------------------------------------------------

#include <CGraflib.h>

//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		SetError
//
// Parameter:
//
//
//---------------------------------------------------------------------------

static EGraflibError StaticErrCode = EGRAFLIB_ERROR_NONE;
static std::string StaticErrFunc;
static std::string StaticErrMsg;

void CGraflib::SetError(EGraflibError er, const char* ef, const char* msg)
{
    StaticErrCode = er;
    StaticErrFunc = ef;
    StaticErrMsg = msg;
}
