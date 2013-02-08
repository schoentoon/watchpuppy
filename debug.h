#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef DEV
#  include <stdio.h>
#  define DEBUG(...) printf(__VA_ARGS__);
#else
#  define DEBUG(...)
#endif

#endif //_DEBUG_H