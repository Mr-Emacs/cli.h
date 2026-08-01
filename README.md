# cli.h

Simple to use library for parsing command line arguments.

All you need is to download cli.h and cli.c and include them in your project and start to use them.

## Example

This is a full working example the main.c is also a working example that contains all of the comments explaining the api

```c
#include <stddef.h>
#include <stdio.h>

#include "cli.h"

void build_fn(command_t *cmd) {
    char *output = cli_get_string(cmd, "output");
    char *source = cli_get_positional(cmd, "source");

    puts("build_fn");
    printf("  Output: %s\n", output);
    printf("  Source: %s\n", source);
}

void run_fn(command_t *cmd) {
    puts("run_fn");
}

void build_exe_fn(command_t *cmd) {
    char *output = cli_get_string(cmd, "output");
    char *source = cli_get_positional(cmd, "source");

    puts("build_exe_fn");
    printf("  Output: %s\n", output);
    printf("  Source: %s\n", source);
}

int main(int argc, char **argv) {
    cli_t *cli = cli_init(NULL, NULL);
    command_t *root = cli_root(cli);

    command_t *build = cli_command(root, "build", "Will build source files");
    cli_command_callback(build, build_fn);

    command_t *run = cli_command(root, "run", "Will run the final executable after build");
    cli_command_callback(run, run_fn);

    char *out;
    cli_create_string(build, "output", "Output executable name", &out);
    cli_alias(build, "output", 'o');

    char *source;
    cli_positional(build, "source", "Source code path", &source, true);

    command_t *build_exe = cli_command(build, "exe", "Build final executable");
    cli_command_callback(build_exe, build_exe_fn);

    cli_status_t status = cli_parse(cli, argc, argv);

    if (argc == 1) {
        cli_usage(cli);
        goto err;
    }

    if (status == CLI_HELP)
        goto suc;

    if (status == CLI_ERROR)
        goto err;

suc:
    cli_destroy(cli);
    return 0;

err:
    cli_destroy(cli);
    return 1;
}
```

To compile this example run

```sh
gcc -o main main.c cli.c #or what ever you called the file.
``

The license under which the file is in MIT LICENSE.