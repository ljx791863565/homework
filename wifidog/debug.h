#ifndef _DEBUG_H_
#define _DEBUG_H_

#define debug(level, format...) _debug(__FILE__, __LINE__, level, format)

void _debug(const char *filename, int line, int level, const char *format, ...);
#endif
