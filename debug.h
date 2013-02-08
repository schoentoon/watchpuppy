#ifndef _DEBUG_H
#define _DEBUG_H

/** Compile using -DDEV ($ make dev) to enable the
 * DEBUG(...) macro, which works exactly the same
 * as printf. In case it's compiled without -DDEV
 * it won't even compile these statements in.
 */
#ifdef DEV
#  include <stdio.h>
#  define DEBUG(...) printf(__VA_ARGS__);
#else
#  define DEBUG(...)
#endif

#endif //_DEBUG_H