#ifndef DEBUG__
#define DEBUG__

/**
 * Enable or disable debug messages.
 */

#define _DEBUG_

#ifdef _DEBUG_

#include <iostream>

/**
 * Log level.
 */

#define EMERG		0
#define ALERT		1
#define CRIT		2
#define ERR			3
#define WARNING		4
#define NOTICE		5
#define INFO		6
#define DEBUG		7

/**
 * Default log level.
 */

#define DEF_LEVEL	DEBUG

/**
 * Modules definition.
 */

#define VIEW			1
#define SRC_FILE		1
#define SRC_CONTAINER	1
#define VIEW_MANAGER	1

/**
 * Debug macro.
 */

#define debug(level, module, msg) \
do { \
if (module && (level <= DEF_LEVEL)) { \
	std::cout << __FUNCTION__ << " " << __LINE__ << " - " << msg << std::endl; }\
} while (0)

#else
#define	debug(level, module, msg)
#endif	// _DEBUG_

#endif // DEBUG