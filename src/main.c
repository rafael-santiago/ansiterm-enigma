/*
 *                        Copyright (C) 2006, 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "options.h"
#include "enigma.h"
#include <accacia.h>
#include <stdio.h>
#include <signal.h>

static int g_aborted = 0;

void sigint_watchdog(int signum) {
    g_aborted = 1;
}

int version() {
    printf("enigma v0.0.1\n");
    return 0;
}

int help() {
    printf("usage: enigma --l-rotor=<rotor> --m-rotor=<rotor> --r-rotor=<rotor> --l-rotor-at=<pos> "
           "--m-rotor-at=<pos> --r-rotor-at=<pos> [--l-ring=<offset> --m-ring=<offset> --r-ring=<offset> --plugboard=<swap pairs>]\n\n"
           "*** This usage helping did not helped me: man enigma\n\n"
           "enigma is Copyright (C) 2006-2016 by Rafael Santiago.\n\n"
           "Bug reports, feedback, etc: <voidbrainvoid@gmail.com> or <https://github.com/rafael-santiago/ansiterm-enigma/issues>\n");
    return 0;
}

int main(int argc, char **argv) {
    int exit_code = 0;

    set_argc_argv(argc, argv);

    if (argc == 1 || get_bool_option("help", 0)) {
        return help();
    }

    if (get_bool_option("version", 0)) {
        return version();
    }

    signal(SIGINT, sigint_watchdog);
    signal(SIGTERM, sigint_watchdog);

    exit_code = enigma(&g_aborted);

    if (g_aborted) {
        accacia_clrscr();
        accacia_screennormalize();
    }

    return exit_code;
}
