/*
 *                        Copyright (C) 2006, 2016 by Rafael Santiago
 *
 * This is free software. You can redistribute it and/or modify under
 * the terms of the GNU General Public License version 2.
 *
 */
#include "enigma.h"
#include "options.h"
#include <eel.h>
#include <accacia.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#define g_enigma_xbounds 400
#define g_enigma_ybounds 400

#define g_enigma_xbetween 3
#define g_enigma_ybetween 2
#define g_enigma_masterx 10
#define g_enigma_mastery 10

#define decl_enigma_lamp_xpos(l) static int g_enigma_lamp ## l ## x = 0

#define get_enigma_lamp_xpos(l) g_enigma_lamp ## l ## x

#define decl_enigma_lamp_ypos(l) static int g_enigma_lamp ## l ## y = 0

#define get_enigma_lamp_ypos(l) g_enigma_lamp ## l ## y

struct lamp_coords {
    int x, y;
};

struct lamp_coords g_lamp_coords_lt[26];

decl_enigma_lamp_xpos(Q);
decl_enigma_lamp_xpos(W);
decl_enigma_lamp_xpos(E);
decl_enigma_lamp_xpos(R);
decl_enigma_lamp_xpos(T);
decl_enigma_lamp_xpos(Z);
decl_enigma_lamp_xpos(U);
decl_enigma_lamp_xpos(I);
decl_enigma_lamp_xpos(O);
decl_enigma_lamp_xpos(A);
decl_enigma_lamp_xpos(S);
decl_enigma_lamp_xpos(D);
decl_enigma_lamp_xpos(F);
decl_enigma_lamp_xpos(G);
decl_enigma_lamp_xpos(H);
decl_enigma_lamp_xpos(J);
decl_enigma_lamp_xpos(K);
decl_enigma_lamp_xpos(P);
decl_enigma_lamp_xpos(Y);
decl_enigma_lamp_xpos(X);
decl_enigma_lamp_xpos(C);
decl_enigma_lamp_xpos(V);
decl_enigma_lamp_xpos(B);
decl_enigma_lamp_xpos(N);
decl_enigma_lamp_xpos(M);
decl_enigma_lamp_xpos(L);
decl_enigma_lamp_ypos(Q);
decl_enigma_lamp_ypos(W);
decl_enigma_lamp_ypos(E);
decl_enigma_lamp_ypos(R);
decl_enigma_lamp_ypos(T);
decl_enigma_lamp_ypos(Z);
decl_enigma_lamp_ypos(U);
decl_enigma_lamp_ypos(I);
decl_enigma_lamp_ypos(O);
decl_enigma_lamp_ypos(A);
decl_enigma_lamp_ypos(S);
decl_enigma_lamp_ypos(D);
decl_enigma_lamp_ypos(F);
decl_enigma_lamp_ypos(G);
decl_enigma_lamp_ypos(H);
decl_enigma_lamp_ypos(J);
decl_enigma_lamp_ypos(K);
decl_enigma_lamp_ypos(P);
decl_enigma_lamp_ypos(Y);
decl_enigma_lamp_ypos(X);
decl_enigma_lamp_ypos(C);
decl_enigma_lamp_ypos(V);
decl_enigma_lamp_ypos(B);
decl_enigma_lamp_ypos(N);
decl_enigma_lamp_ypos(M);
decl_enigma_lamp_ypos(L);

static void init_lamp_coords(const int x, const int y);

static void draw_enigma(libeel_enigma_ctx *enigma);

static void draw_lamps();

static void draw_keyboard();

static void refresh_rotors(libeel_enigma_ctx *enigma);

static void set_lamp_state(const char lamp, const int on);

static void set_key_state(const char key, const int down);

static libeel_reflector_t is_reflector(const char *data);

static libeel_rotor_t is_rotor(const char *number);

static int is_number(const char *number);

static int get_user_plugboard_setting(libeel_enigma_ctx *enigma, const char *plugboard);

static libeel_enigma_ctx *get_user_enigma_settings();

#define turn_on_lamp(l) ( set_lamp_state(l, 1) )

#define turn_off_lamp(l) ( set_lamp_state(l, 0) )

#define key_down(k) ( set_key_state(k, 1) )

#define key_up(k) ( set_key_state(k, 0) )

libeel_enigma_ctx *get_user_enigma_settings();

int enigma(const int *abort) {
    libeel_enigma_ctx *enigma = NULL;
    char k = 0;
    int ttr = -1;

    enigma = get_user_enigma_settings();

    if (enigma == NULL) {
        printf("ERROR: invalid settings.\n");
        return 1;
    }

    init_lamp_coords(g_enigma_masterx, g_enigma_mastery);

    accacia_clrscr();

    draw_enigma(enigma);

    accacia_gotoxy(0,0);
    accacia_textcolor(AC_TCOLOR_BLACK);

    do
    {
        if (accacia_kbhit()) {
            k = toupper(accacia_getch());
        }

        if (isalpha(k)) {
            key_up(libeel_enigma_input(enigma));
            libeel_enigma_input(enigma) = k;
            k = 0;
            key_down(libeel_enigma_input(enigma));
            turn_off_lamp(libeel_enigma_output(enigma));
            libeel_type(enigma);
            refresh_rotors(enigma);
            turn_on_lamp(libeel_enigma_output(enigma));
            accacia_gotoxy(0,0);
            accacia_textcolor(AC_TCOLOR_BLACK);
            ttr = 30000;
        } else if (ttr > -1 && ttr-- == 0) {
            key_up(libeel_enigma_input(enigma));
            accacia_gotoxy(0,0);
            accacia_textcolor(AC_TCOLOR_BLACK);
            ttr = -1;
        }

    } while (k != 27 && (abort == NULL || !*abort));

    libeel_del_enigma_ctx(enigma);

    accacia_clrscr();
    accacia_screennormalize();

    return 0;
}

static void init_lamp_coords(const int x, const int y) {
    get_enigma_lamp_xpos(Q) = x;
    get_enigma_lamp_xpos(W) = get_enigma_lamp_xpos(Q) + g_enigma_xbetween;
    get_enigma_lamp_xpos(E) = get_enigma_lamp_xpos(W) + g_enigma_xbetween;
    get_enigma_lamp_xpos(R) = get_enigma_lamp_xpos(E) + g_enigma_xbetween;
    get_enigma_lamp_xpos(T) = get_enigma_lamp_xpos(R) + g_enigma_xbetween;
    get_enigma_lamp_xpos(Z) = get_enigma_lamp_xpos(T) + g_enigma_xbetween;
    get_enigma_lamp_xpos(U) = get_enigma_lamp_xpos(Z) + g_enigma_xbetween;
    get_enigma_lamp_xpos(I) = get_enigma_lamp_xpos(U) + g_enigma_xbetween;
    get_enigma_lamp_xpos(O) = get_enigma_lamp_xpos(I) + g_enigma_xbetween;

    get_enigma_lamp_xpos(A) = x + 1;
    get_enigma_lamp_xpos(S) = get_enigma_lamp_xpos(A) + g_enigma_xbetween;
    get_enigma_lamp_xpos(D) = get_enigma_lamp_xpos(S) + g_enigma_xbetween;
    get_enigma_lamp_xpos(F) = get_enigma_lamp_xpos(D) + g_enigma_xbetween;
    get_enigma_lamp_xpos(G) = get_enigma_lamp_xpos(F) + g_enigma_xbetween;
    get_enigma_lamp_xpos(H) = get_enigma_lamp_xpos(G) + g_enigma_xbetween;
    get_enigma_lamp_xpos(J) = get_enigma_lamp_xpos(H) + g_enigma_xbetween;
    get_enigma_lamp_xpos(K) = get_enigma_lamp_xpos(J) + g_enigma_xbetween;

    get_enigma_lamp_xpos(P) = x;
    get_enigma_lamp_xpos(Y) = get_enigma_lamp_xpos(P) + g_enigma_xbetween;
    get_enigma_lamp_xpos(X) = get_enigma_lamp_xpos(Y) + g_enigma_xbetween;
    get_enigma_lamp_xpos(C) = get_enigma_lamp_xpos(X) + g_enigma_xbetween;
    get_enigma_lamp_xpos(V) = get_enigma_lamp_xpos(C) + g_enigma_xbetween;
    get_enigma_lamp_xpos(B) = get_enigma_lamp_xpos(V) + g_enigma_xbetween;
    get_enigma_lamp_xpos(N) = get_enigma_lamp_xpos(B) + g_enigma_xbetween;
    get_enigma_lamp_xpos(M) = get_enigma_lamp_xpos(N) + g_enigma_xbetween;
    get_enigma_lamp_xpos(L) = get_enigma_lamp_xpos(M) + g_enigma_xbetween;

    get_enigma_lamp_ypos(Q) = y;
    get_enigma_lamp_ypos(W) = y;
    get_enigma_lamp_ypos(E) = y;
    get_enigma_lamp_ypos(R) = y;
    get_enigma_lamp_ypos(T) = y;
    get_enigma_lamp_ypos(Z) = y;
    get_enigma_lamp_ypos(U) = y;
    get_enigma_lamp_ypos(I) = y;
    get_enigma_lamp_ypos(O) = y;

    get_enigma_lamp_ypos(A) = y + g_enigma_ybetween;
    get_enigma_lamp_ypos(S) = y + g_enigma_ybetween;
    get_enigma_lamp_ypos(D) = y + g_enigma_ybetween;
    get_enigma_lamp_ypos(F) = y + g_enigma_ybetween;
    get_enigma_lamp_ypos(G) = y + g_enigma_ybetween;
    get_enigma_lamp_ypos(H) = y + g_enigma_ybetween;
    get_enigma_lamp_ypos(J) = y + g_enigma_ybetween;
    get_enigma_lamp_ypos(K) = y + g_enigma_ybetween;

    get_enigma_lamp_ypos(P) = y + g_enigma_ybetween + 2;
    get_enigma_lamp_ypos(Y) = y + g_enigma_ybetween + 2;
    get_enigma_lamp_ypos(X) = y + g_enigma_ybetween + 2;
    get_enigma_lamp_ypos(C) = y + g_enigma_ybetween + 2;
    get_enigma_lamp_ypos(V) = y + g_enigma_ybetween + 2;
    get_enigma_lamp_ypos(B) = y + g_enigma_ybetween + 2;
    get_enigma_lamp_ypos(N) = y + g_enigma_ybetween + 2;
    get_enigma_lamp_ypos(M) = y + g_enigma_ybetween + 2;
    get_enigma_lamp_ypos(L) = y + g_enigma_ybetween + 2;

    g_lamp_coords_lt[ 0].x = get_enigma_lamp_xpos(A);
    g_lamp_coords_lt[ 0].y = get_enigma_lamp_ypos(A);
    g_lamp_coords_lt[ 1].x = get_enigma_lamp_xpos(B);
    g_lamp_coords_lt[ 1].y = get_enigma_lamp_ypos(B);
    g_lamp_coords_lt[ 2].x = get_enigma_lamp_xpos(C);
    g_lamp_coords_lt[ 2].y = get_enigma_lamp_ypos(C);
    g_lamp_coords_lt[ 3].x = get_enigma_lamp_xpos(D);
    g_lamp_coords_lt[ 3].y = get_enigma_lamp_ypos(D);
    g_lamp_coords_lt[ 4].x = get_enigma_lamp_xpos(E);
    g_lamp_coords_lt[ 4].y = get_enigma_lamp_ypos(E);
    g_lamp_coords_lt[ 5].x = get_enigma_lamp_xpos(F);
    g_lamp_coords_lt[ 5].y = get_enigma_lamp_ypos(F);
    g_lamp_coords_lt[ 6].x = get_enigma_lamp_xpos(G);
    g_lamp_coords_lt[ 6].y = get_enigma_lamp_ypos(G);
    g_lamp_coords_lt[ 7].x = get_enigma_lamp_xpos(H);
    g_lamp_coords_lt[ 7].y = get_enigma_lamp_ypos(H);
    g_lamp_coords_lt[ 8].x = get_enigma_lamp_xpos(I);
    g_lamp_coords_lt[ 8].y = get_enigma_lamp_ypos(I);
    g_lamp_coords_lt[ 9].x = get_enigma_lamp_xpos(J);
    g_lamp_coords_lt[ 9].y = get_enigma_lamp_ypos(J);
    g_lamp_coords_lt[10].x = get_enigma_lamp_xpos(K);
    g_lamp_coords_lt[10].y = get_enigma_lamp_ypos(K);
    g_lamp_coords_lt[11].x = get_enigma_lamp_xpos(L);
    g_lamp_coords_lt[11].y = get_enigma_lamp_ypos(L);
    g_lamp_coords_lt[12].x = get_enigma_lamp_xpos(M);
    g_lamp_coords_lt[12].y = get_enigma_lamp_ypos(M);
    g_lamp_coords_lt[13].x = get_enigma_lamp_xpos(N);
    g_lamp_coords_lt[13].y = get_enigma_lamp_ypos(N);
    g_lamp_coords_lt[14].x = get_enigma_lamp_xpos(O);
    g_lamp_coords_lt[14].y = get_enigma_lamp_ypos(O);
    g_lamp_coords_lt[15].x = get_enigma_lamp_xpos(P);
    g_lamp_coords_lt[15].y = get_enigma_lamp_ypos(P);
    g_lamp_coords_lt[16].x = get_enigma_lamp_xpos(Q);
    g_lamp_coords_lt[16].y = get_enigma_lamp_ypos(Q);
    g_lamp_coords_lt[17].x = get_enigma_lamp_xpos(R);
    g_lamp_coords_lt[17].y = get_enigma_lamp_ypos(R);
    g_lamp_coords_lt[18].x = get_enigma_lamp_xpos(S);
    g_lamp_coords_lt[18].y = get_enigma_lamp_ypos(S);
    g_lamp_coords_lt[19].x = get_enigma_lamp_xpos(T);
    g_lamp_coords_lt[19].y = get_enigma_lamp_ypos(T);
    g_lamp_coords_lt[20].x = get_enigma_lamp_xpos(U);
    g_lamp_coords_lt[20].y = get_enigma_lamp_ypos(U);
    g_lamp_coords_lt[21].x = get_enigma_lamp_xpos(V);
    g_lamp_coords_lt[21].y = get_enigma_lamp_ypos(V);
    g_lamp_coords_lt[22].x = get_enigma_lamp_xpos(W);
    g_lamp_coords_lt[22].y = get_enigma_lamp_ypos(W);
    g_lamp_coords_lt[23].x = get_enigma_lamp_xpos(X);
    g_lamp_coords_lt[23].y = get_enigma_lamp_ypos(X);
    g_lamp_coords_lt[24].x = get_enigma_lamp_xpos(Y);
    g_lamp_coords_lt[24].y = get_enigma_lamp_ypos(Y);
    g_lamp_coords_lt[25].x = get_enigma_lamp_xpos(Z);
    g_lamp_coords_lt[25].y = get_enigma_lamp_ypos(Z);
}


static void draw_enigma(libeel_enigma_ctx *enigma) {
    accacia_drawbox(get_enigma_lamp_xpos(Q) - 1, get_enigma_lamp_ypos(Q) - 8, 27, 22, AC_BCOLOR_YELLOW);
    accacia_drawbox(get_enigma_lamp_xpos(Q), get_enigma_lamp_ypos(Q) - 7, 25, 20, AC_BCOLOR_BLACK);
    draw_lamps();
    draw_keyboard();
    refresh_rotors(enigma);
}

static void draw_lamps() {
    accacia_textcolor(AC_TCOLOR_WHITE);
    accacia_textstyle(AC_TSTYLE_BOLD);
    accacia_gotoxy(get_enigma_lamp_xpos(A), get_enigma_lamp_ypos(A));printf("A");
    accacia_gotoxy(get_enigma_lamp_xpos(B), get_enigma_lamp_ypos(B));printf("B");
    accacia_gotoxy(get_enigma_lamp_xpos(C), get_enigma_lamp_ypos(C));printf("C");
    accacia_gotoxy(get_enigma_lamp_xpos(D), get_enigma_lamp_ypos(D));printf("D");
    accacia_gotoxy(get_enigma_lamp_xpos(E), get_enigma_lamp_ypos(E));printf("E");
    accacia_gotoxy(get_enigma_lamp_xpos(F), get_enigma_lamp_ypos(F));printf("F");
    accacia_gotoxy(get_enigma_lamp_xpos(G), get_enigma_lamp_ypos(G));printf("G");
    accacia_gotoxy(get_enigma_lamp_xpos(H), get_enigma_lamp_ypos(H));printf("H");
    accacia_gotoxy(get_enigma_lamp_xpos(I), get_enigma_lamp_ypos(I));printf("I");
    accacia_gotoxy(get_enigma_lamp_xpos(J), get_enigma_lamp_ypos(J));printf("J");
    accacia_gotoxy(get_enigma_lamp_xpos(K), get_enigma_lamp_ypos(K));printf("K");
    accacia_gotoxy(get_enigma_lamp_xpos(L), get_enigma_lamp_ypos(L));printf("L");
    accacia_gotoxy(get_enigma_lamp_xpos(M), get_enigma_lamp_ypos(M));printf("M");
    accacia_gotoxy(get_enigma_lamp_xpos(N), get_enigma_lamp_ypos(N));printf("N");
    accacia_gotoxy(get_enigma_lamp_xpos(O), get_enigma_lamp_ypos(O));printf("O");
    accacia_gotoxy(get_enigma_lamp_xpos(P), get_enigma_lamp_ypos(P));printf("P");
    accacia_gotoxy(get_enigma_lamp_xpos(Q), get_enigma_lamp_ypos(Q));printf("Q");
    accacia_gotoxy(get_enigma_lamp_xpos(R), get_enigma_lamp_ypos(R));printf("R");
    accacia_gotoxy(get_enigma_lamp_xpos(S), get_enigma_lamp_ypos(S));printf("S");
    accacia_gotoxy(get_enigma_lamp_xpos(T), get_enigma_lamp_ypos(T));printf("T");
    accacia_gotoxy(get_enigma_lamp_xpos(U), get_enigma_lamp_ypos(U));printf("U");
    accacia_gotoxy(get_enigma_lamp_xpos(V), get_enigma_lamp_ypos(V));printf("V");
    accacia_gotoxy(get_enigma_lamp_xpos(W), get_enigma_lamp_ypos(W));printf("W");
    accacia_gotoxy(get_enigma_lamp_xpos(X), get_enigma_lamp_ypos(X));printf("X");
    accacia_gotoxy(get_enigma_lamp_xpos(Y), get_enigma_lamp_ypos(Y));printf("Y");
    accacia_gotoxy(get_enigma_lamp_xpos(Z), get_enigma_lamp_ypos(Z));printf("Z");
    accacia_screennormalize();
}

static void draw_keyboard() {
    accacia_textcolor(AC_TCOLOR_WHITE);
    accacia_textstyle(AC_TSTYLE_INVERS);
    accacia_gotoxy(get_enigma_lamp_xpos(A), get_enigma_lamp_ypos(A) + 7);printf("A");
    accacia_gotoxy(get_enigma_lamp_xpos(B), get_enigma_lamp_ypos(B) + 7);printf("B");
    accacia_gotoxy(get_enigma_lamp_xpos(C), get_enigma_lamp_ypos(C) + 7);printf("C");
    accacia_gotoxy(get_enigma_lamp_xpos(D), get_enigma_lamp_ypos(D) + 7);printf("D");
    accacia_gotoxy(get_enigma_lamp_xpos(E), get_enigma_lamp_ypos(E) + 7);printf("E");
    accacia_gotoxy(get_enigma_lamp_xpos(F), get_enigma_lamp_ypos(F) + 7);printf("F");
    accacia_gotoxy(get_enigma_lamp_xpos(G), get_enigma_lamp_ypos(G) + 7);printf("G");
    accacia_gotoxy(get_enigma_lamp_xpos(H), get_enigma_lamp_ypos(H) + 7);printf("H");
    accacia_gotoxy(get_enigma_lamp_xpos(I), get_enigma_lamp_ypos(I) + 7);printf("I");
    accacia_gotoxy(get_enigma_lamp_xpos(J), get_enigma_lamp_ypos(J) + 7);printf("J");
    accacia_gotoxy(get_enigma_lamp_xpos(K), get_enigma_lamp_ypos(K) + 7);printf("K");
    accacia_gotoxy(get_enigma_lamp_xpos(L), get_enigma_lamp_ypos(L) + 7);printf("L");
    accacia_gotoxy(get_enigma_lamp_xpos(M), get_enigma_lamp_ypos(M) + 7);printf("M");
    accacia_gotoxy(get_enigma_lamp_xpos(N), get_enigma_lamp_ypos(N) + 7);printf("N");
    accacia_gotoxy(get_enigma_lamp_xpos(O), get_enigma_lamp_ypos(O) + 7);printf("O");
    accacia_gotoxy(get_enigma_lamp_xpos(P), get_enigma_lamp_ypos(P) + 7);printf("P");
    accacia_gotoxy(get_enigma_lamp_xpos(Q), get_enigma_lamp_ypos(Q) + 7);printf("Q");
    accacia_gotoxy(get_enigma_lamp_xpos(R), get_enigma_lamp_ypos(R) + 7);printf("R");
    accacia_gotoxy(get_enigma_lamp_xpos(S), get_enigma_lamp_ypos(S) + 7);printf("S");
    accacia_gotoxy(get_enigma_lamp_xpos(T), get_enigma_lamp_ypos(T) + 7);printf("T");
    accacia_gotoxy(get_enigma_lamp_xpos(U), get_enigma_lamp_ypos(U) + 7);printf("U");
    accacia_gotoxy(get_enigma_lamp_xpos(V), get_enigma_lamp_ypos(V) + 7);printf("V");
    accacia_gotoxy(get_enigma_lamp_xpos(W), get_enigma_lamp_ypos(W) + 7);printf("W");
    accacia_gotoxy(get_enigma_lamp_xpos(X), get_enigma_lamp_ypos(X) + 7);printf("X");
    accacia_gotoxy(get_enigma_lamp_xpos(Y), get_enigma_lamp_ypos(Y) + 7);printf("Y");
    accacia_gotoxy(get_enigma_lamp_xpos(Z), get_enigma_lamp_ypos(Z) + 7);printf("Z");
    accacia_screennormalize();
}

static void refresh_rotors(libeel_enigma_ctx *enigma) {
    accacia_textbackground(AC_BCOLOR_WHITE);
    accacia_textcolor(AC_TCOLOR_BLACK);
    accacia_gotoxy(get_enigma_lamp_xpos(Q) +  9, get_enigma_lamp_ypos(Q) - 2);printf("%c", (enigma != NULL) ? libeel_display(enigma, l) : '?');
    accacia_gotoxy(get_enigma_lamp_xpos(Q) + 12, get_enigma_lamp_ypos(Q) - 2);printf("%c", (enigma != NULL) ? libeel_display(enigma, m) : '?');
    accacia_gotoxy(get_enigma_lamp_xpos(Q) + 15, get_enigma_lamp_ypos(Q) - 2);printf("%c", (enigma != NULL) ? libeel_display(enigma, r) : '?');
    accacia_textbackground(AC_BCOLOR_BLACK);
    accacia_screennormalize();
}

static void set_lamp_state(const char lamp, const int on) {
    struct lamp_coords l_point;
    char n_lamp;

    if (!isalpha(lamp)) {
        return;
    }

    n_lamp = toupper(lamp);

    accacia_textcolor((on) ? AC_TCOLOR_YELLOW : AC_TCOLOR_WHITE);

    accacia_textstyle(AC_TSTYLE_BOLD);

    l_point = g_lamp_coords_lt[n_lamp - 'A'];
    accacia_gotoxy(l_point.x, l_point.y);printf("%c", n_lamp);

    accacia_screennormalize();
}

static void set_key_state(const char key, const int down) {
    struct lamp_coords k_point;
    char n_key;
    int add, add_1;

    if (!isalpha(key)) {
        return;
    }

    n_key = toupper(key);

    add = (down) ? 8 : 7;
    add_1 = (down) ? 7 : 8;

    k_point = g_lamp_coords_lt[n_key - 'A'];

    accacia_gotoxy(k_point.x, k_point.y + add_1);
    accacia_textcolor(AC_TCOLOR_BLACK);
    printf("%c", n_key);

    accacia_gotoxy(k_point.x, k_point.y + add);

    accacia_textcolor(AC_TCOLOR_WHITE);
    accacia_textstyle(AC_TSTYLE_INVERS);
    printf("%c", n_key);

    accacia_screennormalize();
}

static int is_number(const char *number) {
    const char *np = number;

    if (np == NULL) {
        return 0;
    }

    while (*np != 0) {
        if (!isdigit(*np)) {
            return 0;
        }
        np++;
    }

    return 1;
}

static libeel_rotor_t is_rotor(const char *number) {
    char *rotor[] = {
        "i",
        "ii",
        "iii",
        "iv",
        "v",
        "vi",
        "vii",
        "viii"
    };
    const size_t rotor_nr = sizeof(rotor) / sizeof(rotor[0]);
    size_t r;
    char temp[255];
    char *tp = &temp[0];

    if (number == NULL) {
        return -1;
    }

    sprintf(temp, "%s", number);

    while (*tp != 0) {
        *tp = tolower(*tp);
        tp++;
    }

    for (r = 0; r < rotor_nr; r++) {
        if (strcmp(rotor[r], temp) == 0) {
            return r;
        }
    }

    return -1;
}

static libeel_reflector_t is_reflector(const char *data) {
    char reflector[] = {
        'b',
        'c'
    };
    size_t reflector_nr = sizeof(reflector) / sizeof(reflector[0]);
    size_t r = 0;
    char rf = 0;

    if (data == NULL || strlen(data) > 1) {
        return -1;
    }

    rf = tolower(*data);

    while (r < reflector_nr) {
        if (reflector[r] == rf) {
            return r;
        }
        r++;
    }

    return -1;
}

static int get_user_plugboard_setting(libeel_enigma_ctx *enigma, const char *plugboard) {
    const char *p = plugboard, *p_end = NULL;
    int pi = 1;

    if (p == NULL) {
        return 1;
    }

    p_end = p + strlen(p);

    while (p < p_end) {

        while (*p == ',' || *p == ' ') {
            p++;
        }

        if (*p == 0 || *(p + 1) == 0 || *(p + 2) == 0) {
            return 0;
        }

        if (!isalpha(*p) || *(p + 1) != '/' || !isalpha(*(p + 2))) {
            return 0;
        }

        switch (pi) {
            case 1:
                libeel_plugboard(enigma, 1).l = toupper(*p);
                libeel_plugboard(enigma, 1).r = toupper(*(p + 2));
                break;

            case 2:
                libeel_plugboard(enigma, 2).l = toupper(*p);
                libeel_plugboard(enigma, 2).r = toupper(*(p + 2));
                break;

            case 3:
                libeel_plugboard(enigma, 3).l = toupper(*p);
                libeel_plugboard(enigma, 3).r = toupper(*(p + 2));
                break;

            case 4:
                libeel_plugboard(enigma, 4).l = toupper(*p);
                libeel_plugboard(enigma, 4).r = toupper(*(p + 2));
                break;

            case 5:
                libeel_plugboard(enigma, 5).l = toupper(*p);
                libeel_plugboard(enigma, 5).r = toupper(*(p + 2));
                break;

            case 6:
                libeel_plugboard(enigma, 6).l = toupper(*p);
                libeel_plugboard(enigma, 6).r = toupper(*(p + 2));
                break;

            default:
                return 0;
        }

        p  += 3;
        pi += 1;
    }

    return 1;
}

static libeel_enigma_ctx *get_user_enigma_settings() {
    libeel_enigma_ctx *enigma = NULL;
    const char *option = NULL;

    enigma = libeel_new_enigma_ctx();

    if (enigma == NULL) {
        return NULL;
    }

    option = get_option("reflector", NULL);

    if (option == NULL) {
        printf("ERROR: --reflector option is missing.\n");
        goto ___cleanup_and_null;
    }

    if ((enigma->reflector = is_reflector(option)) == -1) {
        printf("ERROR: invalid reflector.\n");
        goto ___cleanup_and_null;
    }

    option = get_option("l-ring", "1");

    if (!is_number(option)) {
        printf("ERROR: left ring has invalid position.\n");
        goto ___cleanup_and_null;
    } else {
        libeel_ring(enigma, l) = atoi(option);
    }

    option = get_option("m-ring", "1");

    if (!is_number(option)) {
        printf("ERROR: middle ring has invalid position.\n");
        goto ___cleanup_and_null;
    } else {
        libeel_ring(enigma, m) = atoi(option);
    }

    option = get_option("r-ring", "1");

    if (!is_number(option)) {
        printf("ERROR: right ring has invalid position.\n");
        goto ___cleanup_and_null;
    } else {
        libeel_ring(enigma, r) = atoi(option);
    }

    option = get_option("l-rotor-at", NULL);

    if (option == NULL || !isalpha(*option)) {
        printf("ERROR: left rotor has invalid setting.\n");
        goto ___cleanup_and_null;
    } else {
        libeel_rotor_at(enigma, l) = toupper(*option);
    }

    option = get_option("m-rotor-at", NULL);

    if (option == NULL || !isalpha(*option)) {
        printf("ERROR: middle rotor has invalid setting.\n");
        goto ___cleanup_and_null;
    } else {
        libeel_rotor_at(enigma, m) = toupper(*option);
    }

    option = get_option("r-rotor-at", NULL);

    if (option == NULL || !isalpha(*option)) {
        printf("ERROR: right rotor has invalid setting.\n");
        goto ___cleanup_and_null;
    } else {
        libeel_rotor_at(enigma, r) = toupper(*option);
    }

    option = get_option("l-rotor", NULL);

    if ((enigma->left_rotor = is_rotor(option)) == -1) {
        printf("ERROR: l-rotor has an unknown setting.\n");
        goto ___cleanup_and_null;
    }

    option = get_option("m-rotor", NULL);

    if ((enigma->middle_rotor = is_rotor(option)) == -1) {
        printf("ERROR: m-rotor has an unknown setting.\n");
        goto ___cleanup_and_null;
    }

    option = get_option("r-rotor", NULL);

    if ((enigma->right_rotor = is_rotor(option)) == -1) {
        printf("ERROR: r-rotor has an unknown setting.\n");
        goto ___cleanup_and_null;
    }

    option = get_option("plugboard", NULL);
    if (option != NULL && get_user_plugboard_setting(enigma, option) == 0) {
        printf("ERROR: invalid plugboard setting.\n");
        goto ___cleanup_and_null;
    }

    if (libeel_init_machine(enigma) == 0) {
        libeel_del_enigma_ctx(enigma);
        return NULL;
    }

    return enigma;

___cleanup_and_null:
    libeel_del_enigma_ctx(enigma);
    return NULL;
}
