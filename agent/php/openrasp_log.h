/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2007 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header,v 1.16.2.1.2.1 2007/01/01 19:32:09 iliaa Exp $ */

#ifndef OPENRASP_LOG_H
#define OPENRASP_LOG_H

#include "openrasp.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#include "php_streams.h"

#ifdef __cplusplus
}
#endif

#define RASP_LOGGER                                 "rasp"
#define ALARM_LOGGER                                "alarm"
#define PLUGIN_LOGGER                               "plugin"
#define POLICY_LOGGER                               "policy"                                        

typedef enum log_appender_t {
	  FILE_APPENDER = 1 << 0, 
    SYSLOG_APPENDER  = 1 << 1
} log_appender;

//reference https://en.wikipedia.org/wiki/Syslog
typedef enum severity_level_t {
  LEVEL_INFO  = 6, 
  LEVEL_DEBUG = 7,
  LEVEL_ALL   = 8
} severity_level;

typedef struct _rasp_logger_entry_t
{
    char           *name;
    zend_bool       accessable;
    int             available_token;
    long            last_logged_time;
    zend_bool       initialized;
    severity_level  level;
    log_appender    appender;
    php_stream     *stream_log;
} rasp_logger_entry;

/*
  	Declare any global variables you may need between the BEGIN
	and END macros here:
*/
ZEND_BEGIN_MODULE_GLOBALS(openrasp_log)

char                  *formatted_date_suffix;
php_stream            *syslog_stream;
zval                  *alarm_request_info;
zval                  *policy_request_info;
zend_bool              enable_alarm_syslog;
zend_bool              in_request_process;
rasp_logger_entry      alarm_logger;
rasp_logger_entry      rasp_logger;
rasp_logger_entry      plugin_logger;
rasp_logger_entry      policy_logger;

ZEND_END_MODULE_GLOBALS(openrasp_log)

ZEND_EXTERN_MODULE_GLOBALS(openrasp_log);

/* In every utility function you add that needs to use variables
   in php_rasp_globals, call TSRMLS_FETCH(); after declaring other
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as RASP_G(variable).  You are
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define OPENRASP_LOG_G(v) TSRMG(openrasp_log_globals_id, zend_openrasp_log_globals *, v)
#define OPENRASP_LOG_GP() ((zend_openrasp_log_globals *)(*((void ***)tsrm_ls))[TSRM_UNSHUFFLE_RSRC_ID(openrasp_log_globals_id)])
#else
#define OPENRASP_LOG_G(v) (openrasp_log_globals.v)
#define OPENRASP_LOG_GP() (&openrasp_log_globals)
#endif

PHP_MINIT_FUNCTION(openrasp_log);
PHP_MSHUTDOWN_FUNCTION(openrasp_log);
PHP_RINIT_FUNCTION(openrasp_log);
PHP_RSHUTDOWN_FUNCTION(openrasp_log);
PHP_MINFO_FUNCTION(openrasp_log);

#endif /* OPENRASP_LOG_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */