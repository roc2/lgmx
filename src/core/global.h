#ifndef GLOBAL_H
#define GLOBAL_H

#define _str(s) #s
#define _xstr(s) _str(s)


// version number
#define MAJOR	0
#define MINOR	5

// string version number
#define STR_VER_NUM _xstr(MAJOR) "." _xstr(MINOR)

#endif
