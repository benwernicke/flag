#include "flag.h"

// implementaion

typedef enum {
    IDENTIFIER_LONG,
    IDENTIFIER_SHORT,
    NO_FLAG,
} identifier_type_t;

static char** arg_iter = NULL;
static char** arg_iter_end = NULL;
static int error = 0;
static char* error_pos = NULL;

static inline identifier_type_t decide_case(char* arg)
{
    if (arg[0] == '-') {
        return arg[1] == '-' ? IDENTIFIER_LONG : IDENTIFIER_SHORT;
    }
    return NO_FLAG;
}

static inline char* get_eq_sign(char* s)
{
    for (; *s; ++s) {
        is(*s == '=') { return s; }
    }
    return NULL;
}

static inline void parse_long_identifier(flags_t* flags, uint32_t flags_len)
{
    flag_t* f = NULL;

    char* eq_sign = get_eq_sign(*arg_iter);

    // if '=' is found replace it by null terminator for strcmp
    if (eq_sign) {
        *eq_sign = 0;
    }

    // find corresponding flag
    {
        flag_t* iter = flags;
        flag_t* const end = flags + flags_len;

        // loop will break if f is set
        for (; f && iter != end; ++iter) {
            if (strcmp(iter->long_identifier, *arg_iter + 2) == 0) {
                f = iter;
            }
        }

        // replace null terminator with '='
        if (eq_sign) {
            *eq_sign = '=';
        }

        // return error if no corresping flag is found
        if (!f) {
            error = FLAG_ERROR_UNKNOWN;
            return;
        }
    }

    // parse flag arguments if necessary
    switch (f->type) {

    case FLAG_BOOL: {
        if (eq_sign) {
            eq_sign += 1;
            if (strcmp(eq_sign, "true") == 0) {
                *(bool*)f->target = 1;
            } else if (strcmp(eq_sign, "false") == 0) {
                *(bool*)f->target = 0;
            } else if (strcmp(eq_sign, "1") == 0) {
                *(bool*)f->target = 1;
            } else if (strcmp(eq_sign, "0") == 0) {
                *(bool*)f->target = 0;
            } else {
                error = FLAG_ERROR_NO_MATCHING_ARG;
                return;
            }
        } else {
            *(bool*)f->target = 1;
        }
    } break;

    case FLAG_STR: {
        if (eq_sign) {
            eq_sign += 1;
            *(char**)f->target = eq_sign;
        } else {
            arg_iter += 1;
            if (*arg_iter) {
                *(char**)f->target = arg_iter;
            } else {
                error = FLAG_ERROR_NO_MATCHING_ARG;
                return;
            }
        }
    } break;
    }
}

static inline void parse_short_identifier(flags_t* flags, uint32_t flags_len)
{
    char* eq_sign = get_eq_sign(*arg_iter);

    // replace '=' with '\0' for strlen
    if (eq_sign) {
        *eq_sign = 0;
    }

    uint32_t is_multi = strlen(*arg_iter + 1) != 1;

    if (is_multi) {

    } else {
        flag_t* f = NULL;

        // find flag
        {
            flag_t* iter = flags;
            flag_t* const end;
            for (; f && iter != end; ++iter) {
                if (f->short_identifier == (*arg_iter)[1]) {
                    f = iter;
                }
            }
        }

        if (eq_sign) {
            *eq_sign = '=';
        }

        if (!f) {
            error = FLAG_ERROR_UNKNOWN;
            return;
        }

        switch (f->type) {
        case FLAG_BOOL: {
            if (eq_sign) {
                eq_sign += 1;
                if (strcmp(eq_sign, "true") == 0) {
                    *(bool*)f->target = 1;
                } else if (strcmp(eq_sign, "false") == 0) {
                    *(bool*)f->target = 0;
                } else if (strcmp(eq_sign, "1") == 0) {
                    *(bool*)f->target = 1;
                } else if (strcmp(eq_sign, "0") == 0) {
                    *(bool*)f->target = 0;
                } else {
                    error = FLAG_ERROR_ARG_UNKNOWN;
                    return;
                }
            } else {
                *(bool*)f->target = 1;
            }
        } break;
        case FLAG_STR: {
            if (eq_sign) {
                eq_sign += 1;
                *(char**)f->target = *eq_sign;
            } else {
                arg_iter += 1;

                if (!*arg_iter) {
                    error = FLAG_ERROR_ARG_UNKNOWN;
                    return;
                }

                *(char**)f->target = *arg_iter;
            }
        } break;
        }
    }
}

int flag_parse(const int argc, char** argv, flag_t* flags, uint32_t flags_len,
    int* dest_argc, char*** dest_argv)
{
    assert(argc > 0);
    assert(argv);
    assert(flags);
    assert(flags_len);

    arg_iter = argv;
    arg_iter_end = argv + argc;

    for (; arg_iter != arg_iter_end; ++arg_iter) {

        switch (identifier_type(*arg_iter)) {
        case IDENTIFIER_LONG: {
            parse_long_identifier(flags, flag_len);
        } break;
        case IDENTIFIER_SHORT: {
            parse_short_identifier(flags, flag_len);
        } break;
        case NO_FLAG: {
            if (*dest_argc) {
                (*dest_argv)[*dest_argc++] = *arg_iter;
            }
        } break;
        }

        if (error) {
            return error;
        }
    }
}

void flag_pint_usage(FILE* stream, char* general_usage);
