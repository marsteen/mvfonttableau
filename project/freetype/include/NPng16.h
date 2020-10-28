
#ifndef NPng16_H
#define NPng16_H

#include <CDataRect.h>


namespace NPng16
{
	bool png_read_file(const char* Filename, size_t file_pos, CDataRect& dr);
	int png_write(const char* Filename, CDataRect& dr);
	

};

#endif