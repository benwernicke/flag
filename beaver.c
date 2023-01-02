#include "lib/beaver.h"

#define FLAGS "-g -Wall -Werror -Og -pedantic -Wextra"

module_t modules[] = {
    { .name = "greetings", .src = "examples/greetings.c" },
    { .name = "greetings", .src = "flag.c" },
};

uint32_t modules_len = sizeof(modules) / sizeof(*modules);

char* greetings[] = { "greetings", NULL };

int main(int argc, char** argv)
{
    auto_update(argv);
    if (argc == 1) {
        compile(greetings, FLAGS);
    } else if (strcmp(argv[1], "clean") == 0) {
        rm("$(find build -type f)");
        rm("out");
    } else {
        fprintf(stderr, "\033[31mError:\033[39m unknown option: '%s'\n", argv[1]);
        exit(1);
    }
    return 0;
}
