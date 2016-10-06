/*
 *                        Copyright (C) 2006, 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "options.h"
#include "enigma.h"

int main(int argc, char **argv) {
    set_argc_argv(argc, argv);
    return enigma();
}
