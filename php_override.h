
#ifndef PHP_OVERRIDE_H
#define PHP_OVERRIDE_H

#define OVERRIDE_EXT_VERSION "1.0.0"

extern zend_module_entry override_module_entry;
#define phpext_override_ptr &override_module_entry

#ifdef PHP_WIN32
#   define PHP_OVERRIDE_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#   define PHP_OVERRIDE_API __attribute__ ((visibility("default")))
#else
#   define PHP_OVERRIDE_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#ifdef ZTS
#define OVERRIDE_G(v) TSRMG(override_globals_id, zend_override_globals *, v)
#else
#define OVERRIDE_G(v) (override_globals.v)
#endif

#endif  /* PHP_OVERRIDE_H */
