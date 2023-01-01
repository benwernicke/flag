#ifndef FLAG_H
#define FLAG_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    FLAG_BOOL,
    FLAG_STR,
} flag_type_t;

typedef struct flag_t flag_t;
struct flag_t {
    char short_identifier;
    char* long_identifier;
    char* description;
    void* target;
    flag_type_t type;
};

int flag_parse(const int argc, char** argv, flag_t* flags,
    uint32_t flags_len, int* dest_argc, char*** dest_argv);

void flag_pint_usage(FILE* stream, char* general_usage);

#endif
