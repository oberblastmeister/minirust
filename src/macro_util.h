#ifndef _LOX_MACRO_UTIL_H
#define _LOX_MACRO_UTIL_H

#define CAT(a, b) a##b

#define PASTE(a, b) CAT(a, b)

#define JOIN(prefix, name) PASTE(prefix, PASTE(_, name))
#define _JOIN(prefix, name) PASTE(_, PASTE(prefix, PASTE(_, name)))

#endif
