#pragma once
/*
	mpg123_msvc: MPEG Audio Decoder library wrapper header for MS VC++ 2005

	copyright 2008 by the mpg123 project - free software under the terms of the LGPL 2.1
	initially written by Patrick Dehne and Thomas Orgis.
*/

namespace libmpg123Namespace
{
	typedef long ssize_t;
	typedef __int32 int32_t;
	typedef unsigned __int32 uint32_t;

	#define MPG123_NO_CONFIGURE
	#include "mpg123-1.9.2/src/libmpg123/mpg123.h.in"
}
