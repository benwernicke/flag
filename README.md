# flag
This is a very simple flag parser

## Documentation

```
int flag_parse(const int argc, char** argv, flag_t* flags, uint32_t flags_len,
        int* dest_argc, char*** dest_argv);

```

This will parse a given `argc`/`argv` for flags defined in the `flags` array
with `flags_len` length. If needed a non `NULL` pointer can be past to
`dest_argc` and one to `dest_argv` to get argv without any flags or the arguments of
any flags.

Note: `dest_argv` must be an array which is at least big enough to hold entire
`argv`

Flags are defined like this:

```
flag_t flags[] = {
    {
        .short_identifier = 'h',    // can be '\0' if not needed
        .long_identifier = "help",  // can be NULL if not needed
        .description = "print this page and exit", // cannot be NULL
        .target = &flag_help,       // cannot be null must be of type according to .type
        .type = FLAG_BOOL,          // type of the flag: either FLAG_BOOL or FLAG_STR
    },
};
```

If `flag_parse()`  encounters an error, either a found flag is not present in
the flags array (= `FLAG_ERROR_FLAG_UNKNOWN`) or a flag which requires an
argument but doesnt have an argument (= `FLAG_ERROR_ARG_UNKNOWN`)

```
void flag_print_usage(
    FILE* stream, char* general_usage, flag_t* flags, uint32_t flags_len);
```

Prints the `general_usage` and all flag descriptions to `stream`

## TODO
- add a options variable to `flag_parse` and `flag_print_usage` to further
  sepcify how the functions behave under what input
