//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	SFileReadInfo.h
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************

#ifndef SFILEREADINFO_H
#define SFILEREADINFO_H

#include <CFileIO.h>

struct SFileReadInfo
{
    SFileReadInfo()
    {
        mFio = NULL;
        mData = NULL;
    }


    CFileIO*		mFio;
    unsigned int	mFilePosition;
    unsigned int	mFileSize;
    void*			mData;
};
#endif
