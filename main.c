#include "cli.h"
#include <stddef.h>
#include <stdio.h>

// @note: A working example of how to use the library. We will be assumimg you
// are building a compiler as the original intent of the library was to be used
// in my own compiler.
//
// @howto: You will have to create the cli and the command structure. Everytime
// you want to create a new command you will call the the cli_command function
// @fn: command_t *cli_command(command_t *parent, const char *name, const char *desc);
//
// @fn: cli_t *cli_init(const char *name, const char *description);
// In here the reason you would want to set a name or a descrption is if you
// want to customize the default messages we get from the usage both can be null
// here. So that instead of 'Application' as the description and using the
// default program name aka argv[0] you obviously can customize it.
//
// @fn: command_t *cli_command(command_t *parent, const char *name, const char *desc);
// To supply the parent command_t you can get the parent command using the
//
// @fn: command_t *cli_root(cli_t *cli);
//
// Then you just pass the name of the command and the description note that the
// name can not be null how ever the description can be null. After that if you
// want to set a call back for this command you can bind it with
//
// @fn: void cli_command_callback(command_t *cmd, cli_call_back_t callback);
// You pass your command and the exepected call back with the signature of
// typedef void (*cli_call_back_t)(command_t *cmd);
//
// When you want to create subflags you would call to either of the following
// functions
// @fn: void cli_create_flag_bool(command_t *cmd, const char *name, const char
//                           *description, bool *value);
// ...
// you would create the value you want to store the state of the flag for
// example if you want --yes to print you an output you would create
//
// A working example of this case. We did disrigard error handeling
//
// cli_t *cli = cli_init(NULL, NULL);
// command_t *cmd = cli_root(cli);
// command_t *echo = cli_command(cmd, "echo", "Print output");
//
// bool output_yes = false;
// cli_create_flag_bool(cmd, "yes", "Output yes", &output_yes);
//
// cli_status_t status = cli_parse(cli, argc, argv);
//  if (argc == 1) {
//     cli_usage(cli);
//     return 1;
// }
//
// if (status == CLI_HELP)
//     return 0;
//
// if (status == CLI_ERROR)
// p    return 1;
//
// command_t *active = cli_active_command(cli);
// if (active == echo) {
//   if (output_yes)
//      puts("Yes");
// }

// @note: Full working code example

// a call back function to build the source code
void build_fn(command_t *cmd) {
    // will find the flag output and the get string value from it
    char *output = cli_get_string(cmd, "output");

    // will find the flag source and the get string value from it
    char *source = cli_get_positional(cmd, "source");

    // Logic of the function: For now for simplicity we do not do anything complicated.
    puts("build_fn");
    printf("  Output: %s\n", output);
    printf("  Source: %s\n", source);
}

// a call back function to run the source code
void run_fn(command_t *cmd) {
    puts("run_fn");
}

// a call back function to build an executable from the source code
void build_exe_fn(command_t *cmd) {
    char *output = cli_get_string(cmd, "output");
    char *source = cli_get_positional(cmd, "source");

    puts("build_exe_fn");
    printf("  Output: %s\n", output);
    printf("  Source: %s\n", source);
}

// a call back function to build an object file from the source code
void build_obj_fn(command_t *cmd) {
    char *output = cli_get_string(cmd, "output");
    char *source = cli_get_positional(cmd, "source");

    puts("build_obj_fn");
    printf("  Output: %s\n", output);
    printf("  Source: %s\n", source);
}

int main(int argc, char **argv) {
    // will use the default name, description
    cli_t *cli = cli_init(NULL, NULL);

    // will get the root command which is the main program itself
    command_t *root = cli_root(cli);

    // @note: we create a command build that will be bounded to a callback
    // function build_fn. It is supposed to build all of the source files as described.
    command_t *build = cli_command(root, "build", "Will build source files");
    cli_command_callback(build, build_fn);

    // @note: we create a command run that will be bounded to a callback
    // function run_fn. It will run the exeutable that we build theoretically.
    command_t *run = cli_command(root, "run", "Will run the final executable after build");
    cli_command_callback(run, run_fn);

    // @note: we created a sub flag for build that is named output which will
    // specify the output of the file as it is not a required flag. Meaning it
    // can be null though there should be an option to make it so that we can
    // accept required or not in boolean just like positional flag.
    char *out;
    cli_create_flag_string(build, "output", "Output executable name", &out);
    // we aliased it to just the char 'o' so users can just do -o instead of '--output'.
    cli_alias(build, "output", 'o');

    // @note: we create a flag source that is gonna provide the source code of
    // the file which is a positional meaning it can come before or after the
    // --flags however it is mandatory since it is marked as required with true.
    char *source;
    cli_positional(build, "source", "Source code path", &source, true);

    // @note: we ceate a sub command on top of 'build' exe and obj then bound
    // them to their own call back.
    command_t *build_exe = cli_command(build, "exe", "Build final executable");
    cli_command_callback(build_exe, build_exe_fn);

    command_t *build_obj = cli_command(build, "obj", "Build object file");
    cli_command_callback(build_obj, build_obj_fn);

    // @note: After setting all of our builds we will call cli_parse which will
    // parse and returns us the status. Implicitly we parse -h, help or --help
    // meaning that the status can be error, ok or help which will call
    // cli_usage or cli_command_usage based on the root command we called it from.

    cli_status_t status = cli_parse(cli, argc, argv);

    if (argc == 1) {
        cli_usage(cli);
        goto err;
    }

    if (status == CLI_HELP) {
        goto suc;
    }

    if (status == CLI_ERROR) {
        goto err;
    }

suc:
    cli_destroy(cli);
    return 0;

err:
    cli_destroy(cli);
    return 1;
}
