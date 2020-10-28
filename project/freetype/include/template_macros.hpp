//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	template_macros.hpp
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************


#ifndef TEMPLATE_MACROS_HPP
#define TEMPLATE_MACROS_HPP

template <typename T>
T Tmax(T a, T b)
{	
	return (a > b) ? a : b;
}

template <typename T>
T Tmin(T a, T b)
{	
	return (a < b) ? a : b;
}


#endif