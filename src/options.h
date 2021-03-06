/*
 *                        Copyright (C) 2006, 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#ifndef ANSITERM_ENIGMA_OPTIONS_H
#define ANSITERM_ENIGMA_OPTIONS_H 1

void set_argc_argv(int argc, char **argv);

const char *get_option(const char *option, const char *default_value);

int get_bool_option(const char *option, const int default_value);

#endif
