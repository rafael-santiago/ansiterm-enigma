/*
 *                        Copyright (C) 2006, 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "options.h"
#include <stdio.h>
#include <string.h>

static int g_argc = 0;

static char **g_argv = NULL;

void set_argc_argv(int argc, char **argv) {
    g_argc = argc;
    g_argv = argv;
}

const char *get_option(const char *option, const char *default_value) {
    char opt[255] = "";
    int a = 0;

    if (option == NULL || g_argv == NULL) {
        return default_value;
    }

    sprintf(opt, "--%s=", option);

    while (a < g_argc) {

        if (strstr(g_argv[a], opt) == g_argv[a]) {
            return (g_argv[a] + strlen(opt));
        }

        a++;

    }

    return default_value;
}

int get_bool_option(const char *option, const int default_value) {
    char opt[255] = "";
    int a = 0;

    if (option == NULL || g_argv == NULL) {
        return default_value;
    }

    sprintf(opt, "--%s", option);

    while (a < g_argc) {

        if (strcmp(g_argv[a], opt) == 0) {
            return 1;
        }

        a++;
    }

    return default_value;
}
