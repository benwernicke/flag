#include <stdio.h>
#include <stdlib.h>

#include "../flag.h"

bool flag_help = 0;
bool flag_version = 0;
char* flag_name = NULL;

flag_t flags[] = {
    {
        .short_identifier = 'h',
        .long_identifier = "help",
        .description = "print this page and exit",
        .target = &flag_help,
        .type = FLAG_BOOL,
    },
    {
        .short_identifier = 'v',
        .long_identifier = "version",
        .description = "print version and exit",
        .target = &flag_version,
        .type = FLAG_BOOL,
    },
    {
        .short_identifier = 0,
        .long_identifier = "name",
        .description = "will print given name in greeting",
        .target = &flag_name,
        .type = FLAG_STR,
    },
};

uint32_t flags_len = sizeof(flags) / sizeof(*flags);

int main(int argc, char** argv)
{
    flag_error_t e = flag_parse(argc, argv, flags, flags_len, NULL, NULL);

    if (e) {
        fprintf(stderr, "Error: '%s' at: %s\n", flag_error_format(e),
            *flag_error_position());
        exit(1);
    }

    if (flag_help) {
        flag_print_usage(
            stdout, "This program will show a nice greeting", flags, flags_len);
        exit(0);
    } else if (flag_version) {
        printf("Version: 69\n");
        exit(0);
    }

    printf("Greetings, %s!\n", flag_name ? flag_name : "stranger");
    return 0;
}
