//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CBlockOperations.h
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************

#ifndef CBLOCKOPERATIONS_H
#define CBLOCKOPERATIONS_H

#include <CFileIO.h>
#include <CDataRect.h>

class CBlockOperations
{
	public:

    static void WriteBlock(CFileIO* fio, CDataRect* dstRect, const CDataRect* srcRect, int Xpos, int Ypos, int BytesProPixel);
    static void CopyBlock(CDataRect* dstRect, const CDataRect* srcRect, int Xpos, int Ypos, int BytesProPixel);
    static void CopyBlock2(CDataRect* dstRect, const CDataRect* srcRect, int Xpos, int Ypos, int CopyX, int CopyY, int BytesProPixel);

};

#endif
