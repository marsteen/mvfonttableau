//***************************************************************************
//
//
// @PROJECT  :	Phonetik Library
// @VERSION  :	1.0
// @FILENAME :	NUtf.h
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************


#ifndef NUTF2_H
#define NUTF2_H

namespace NUtf2
{
    unsigned int Utf8_to_Utf32(const unsigned char* Utf8, int* Size);
    std::string Utf8_to_Ansi(const unsigned char* Utf8, int* Errors);
    std::string Ansi_to_Utf8(const unsigned char* Utf8, int* Errors);
    std::string ResolveAnsiUmlauteString(const std::string& s);
}

#endif
