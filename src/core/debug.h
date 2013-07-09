#ifndef DEBUG__
#define DEBUG__

/**
 * Enable or disable debug messages.
 */

#define _DEBUG_
//#undef _DEBUG_

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
#define V_SRC_FILE		1
#define SRC_CONTAINER	1
#define ROOT_CONTAINER	1
#define VIEW_MANAGER	1
#define MAIN_WINDOW		1
#define MAIN			1
#define CODE_EDITOR		1
#define SEARCH			1
#define STATUS_BAR		1
#define CLI				1
#define HIGHLIGHT		1
#define F_WATCHER		1
#define TAGS			1

/**
 * Debug macro.
 */

#define debug(level, module, msg) \
do { \
if (module && (level <= DEF_LEVEL)) { \
	std::cout << __FILE__ << " - " << __FUNCTION__ << " " << __LINE__ << " - " << msg << std::endl; }\
} while (0)

#else
#define	debug(level, module, msg)
#endif	// _DEBUG_

#endif // DEBUG__
