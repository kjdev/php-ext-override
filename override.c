
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"
#include "php_override.h"

ZEND_FUNCTION(override_function);

ZEND_BEGIN_ARG_INFO_EX(arginfo_override_function, 0, 0, 3)
    ZEND_ARG_INFO(0, name)
    ZEND_ARG_INFO(0, args)
    ZEND_ARG_INFO(0, code)
    ZEND_ARG_INFO(0, origin)
ZEND_END_ARG_INFO()

static const zend_function_entry override_functions[] = {
    ZEND_FE(override_function, arginfo_override_function)
    ZEND_FE_END
};

ZEND_MINFO_FUNCTION(override)
{
    php_info_print_table_start();
    php_info_print_table_row(2, "Override support", "enabled");
    php_info_print_table_row(2, "Extension Version", OVERRIDE_EXT_VERSION);
    php_info_print_table_end();
}

zend_module_entry override_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    "override",
    override_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    ZEND_MINFO(override),
#if ZEND_MODULE_API_NO >= 20010901
    OVERRIDE_EXT_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_OVERRIDE
ZEND_GET_MODULE(override)
#endif

#define OVERRIDE_FUNCTION_INSPECT 0
#define OVERRIDE_FUNCTION_REMOVE  1
#define OVERRIDE_FUNCTION_RENAME  2

static int
override_fetch_function(char *fname, long fname_len,
                        zend_function **pfe, int flag TSRMLS_DC)
{
    zend_function *fe;

    php_strtolower(fname, strlen(fname));

    if (zend_hash_find(EG(function_table), fname, fname_len+1,
                       (void **)&fe) == FAILURE) {
        zend_error(E_WARNING, "%s not found", fname);
        return FAILURE;
    }

    if (fe->type != ZEND_USER_FUNCTION && fe->type != ZEND_INTERNAL_FUNCTION) {
        zend_error(E_WARNING, "%s is not a user or internal function", fname);
        return FAILURE;
    }

    if (pfe) {
        *pfe = fe;
    }

    return SUCCESS;
}

ZEND_FUNCTION(override_function)
{
    char *fname, *fargs, *fcode, *forigin = NULL;
    long fname_len, fargs_len, fcode_len, forigin_len = 0;
    char *eval_code, *eval;
    zend_function *fe, func;
    int retval, ftype = OVERRIDE_FUNCTION_INSPECT;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
                              "sss|s", &fname, &fname_len,
                              &fargs, &fargs_len, &fcode, &fcode_len,
                              &forigin, &forigin_len) == FAILURE) {
        RETURN_FALSE;
    }

    if (forigin && forigin_len > 0) {
        ftype = OVERRIDE_FUNCTION_RENAME;
    }

    if (override_fetch_function(fname, fname_len, &fe,
                                ftype TSRMLS_CC) != SUCCESS) {
        RETURN_FALSE;
    }

    func = *fe;
    if (fe->type == ZEND_USER_FUNCTION && ftype == OVERRIDE_FUNCTION_RENAME) {
        function_add_ref(&func);
    }

    if (zend_hash_del(EG(function_table), fname, fname_len+1) == FAILURE) {
        zend_error(E_WARNING, "Error removing reference to function name %s()",
                   fname);
        zend_function_dtor(&func);
        RETURN_FALSE;
    }

    if (ftype == OVERRIDE_FUNCTION_RENAME) {
        if (func.type == ZEND_USER_FUNCTION) {
            efree((char *)func.common.function_name);
            func.common.function_name = estrndup(forigin, forigin_len);
        }
        if (zend_hash_add(EG(function_table), forigin, forigin_len+1,
                          &func, sizeof(zend_function), NULL) == FAILURE) {
            zend_error(E_WARNING, "Unable to rename function %s()", forigin);
            zend_function_dtor(fe);
            RETURN_FALSE;
        }
    }

    spprintf(&eval_code, 0, "function %s(%s){%s}", fname, fargs, fcode);
    if (!eval_code) {
        RETURN_FALSE;
    }

    eval = zend_make_compiled_string_description(
        "runtime created function" TSRMLS_CC);
    retval = zend_eval_string(eval_code, NULL, eval TSRMLS_CC);
    efree(eval_code);
    efree(eval);

    if (retval == SUCCESS) {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}
