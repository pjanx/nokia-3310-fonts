/*
 * nokia-generate-bdf.c: BDF generator for Nokia 3310 fonts
 *
 * Copyright (c) 2013, Přemysl Janouch <p.janouch@gmail.com>
 * All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <stdlib.h>
#include <string.h>
#include "nokia-fonts.c"

#define N_ELEMENTS(a) (sizeof (a) / sizeof (a)[0])

static void
generate_big_char (struct nokia_big_char *ch, wchar_t code)
{
	printf ("STARTCHAR U+%04X\n", code);
	printf ("ENCODING %d\n", code);
	printf ("SWIDTH %d 0\n", (int) (.5 + 1000. * ch->advance / 14.));
	printf ("DWIDTH %d\n", ch->advance);
	printf ("BBX %d 14 0 -2\n", ch->width);
	printf ("BITMAP\n");
	printf (ch->width > 8 ? "0000\n" : "00\n");

	unsigned i;
	for (i = 0; i < 13; i++)
	{
		unsigned short row = ch->rows[i];

		printf ("%02X", (row >> 8) & 0xFF);
		if (ch->width > 8)
			printf ("%02X", row & 0xFF);
		putchar ('\n');
	}

	printf ("ENDCHAR\n");
}

static void
generate_big_font (void)
{
	// Line height for this font = 14 = 1 above + 13

	printf ("STARTFONT 2.1\n");
	printf ("FONT -nokia-nokia-medium-r-normal--14-140-75-75-p-65-iso10646-1\n");
	printf ("SIZE 14 75 75\n");
	printf ("FONTBOUNDINGBOX 14 14 0 -2\n");
	printf ("STARTPROPERTIES 2\n");
	printf ("FONT_ASCENT 12\n");
	printf ("FONT_DESCENT 2\n");
	printf ("ENDPROPERTIES\n");

	printf ("CHARS %zu\n", N_ELEMENTS (nokia_big_data)
		+ N_ELEMENTS (nokia_big_wide_data));

	unsigned i;
	for (i = 0; i < N_ELEMENTS (nokia_big_data); i++)
		generate_big_char (&nokia_big_data[i], ' ' + i);

	for (i = 0; i < N_ELEMENTS (nokia_big_wide_data); i++)
	{
		struct nokia_big_char_wide *chw = &nokia_big_wide_data[i];
		generate_big_char (&chw->ch, chw->code);
	}

	printf ("ENDFONT\n");
}

static void
generate_small_char (struct nokia_small_char *ch, wchar_t code)
{
	printf ("STARTCHAR U+%04X\n", code);
	printf ("ENCODING %d\n", code);
	printf ("SWIDTH %d 0\n", (int) (.5 + 1000. * ch->advance / 10.));
	printf ("DWIDTH %d\n", ch->advance);
	printf ("BBX %d 10 0 -2\n", ch->width);
	printf ("BITMAP\n");
	printf ("00\n");

	unsigned i;
	for (i = 0; i < 8; i++)
		printf ("%02X\n", ch->rows[i] & 0xFF);

	printf ("00\n");
	printf ("ENDCHAR\n");
}

static void
generate_small_font (void)
{
	// Line height for this font = 10 = 1 above + 8 + 1 below

	printf ("STARTFONT 2.1\n");
	printf ("FONT -nokia-nokia-medium-r-normal--10-100-75-75-p-40-iso10646-1\n");
	printf ("SIZE 10 75 75\n");
	printf ("FONTBOUNDINGBOX 10 10 0 -2\n");
	printf ("STARTPROPERTIES 2\n");
	printf ("FONT_ASCENT 8\n");
	printf ("FONT_DESCENT 2\n");
	printf ("ENDPROPERTIES\n");

	printf ("CHARS %zu\n", N_ELEMENTS (nokia_small_data)
		+ N_ELEMENTS (nokia_small_wide_data));

	unsigned i;
	for (i = 0; i < N_ELEMENTS (nokia_small_data); i++)
		generate_small_char (&nokia_small_data[i], ' ' + i);

	for (i = 0; i < N_ELEMENTS (nokia_small_wide_data); i++)
	{
		struct nokia_small_char_wide *chw = &nokia_small_wide_data[i];
		generate_small_char (&chw->ch, chw->code);
	}

	printf ("ENDFONT\n");
}

static void
generate_small_bold_font (void)
{
	// Line height for this font = 10 = 1 above + 8 + 1 below

	printf ("STARTFONT 2.1\n");
	printf ("FONT -nokia-nokia-bold-r-normal--10-100-75-75-p-40-iso10646-1\n");
	printf ("SIZE 10 75 75\n");
	printf ("FONTBOUNDINGBOX 10 10 0 -2\n");
	printf ("STARTPROPERTIES 2\n");
	printf ("FONT_ASCENT 8\n");
	printf ("FONT_DESCENT 2\n");
	printf ("ENDPROPERTIES\n");

	printf ("CHARS %zu\n", N_ELEMENTS (nokia_small_bold_data)
		+ N_ELEMENTS (nokia_small_bold_wide_data));

	unsigned i;
	for (i = 0; i < N_ELEMENTS (nokia_small_bold_data); i++)
		generate_small_char (&nokia_small_bold_data[i], ' ' + i);

	for (i = 0; i < N_ELEMENTS (nokia_small_bold_wide_data); i++)
	{
		struct nokia_small_char_wide *chw = &nokia_small_bold_wide_data[i];
		generate_small_char (&chw->ch, chw->code);
	}

	printf ("ENDFONT\n");
}

static void
generate_menu_font (void)
{
	printf ("STARTFONT 2.1\n");
	printf ("FONT -nokia-nokia-medium-r-normal--6-60-75-75-p-40-iso10646-1\n");
	printf ("SIZE 6 75 75\n");
	printf ("FONTBOUNDINGBOX 6 6 0 0\n");
	printf ("STARTPROPERTIES 2\n");
	printf ("FONT_ASCENT 6\n");
	printf ("FONT_DESCENT 0\n");
	printf ("ENDPROPERTIES\n");

	printf ("CHARS %zu\n", N_ELEMENTS (nokia_menu_data));

	unsigned i, k;
	for (i = 0; i < N_ELEMENTS (nokia_menu_data); i++)
	{
		struct nokia_menu_char *ch = &nokia_menu_data[i];

		printf ("STARTCHAR U+%04X\n", '-' + i);
		printf ("ENCODING %d\n", '-' + i);
		printf ("SWIDTH %d 0\n", (int) (.5 + 1000. * ch->advance / 6.));
		printf ("DWIDTH %d\n", ch->advance);
		printf ("BBX %d 6 0 0\n", ch->width);
		printf ("BITMAP\n");

		for (k = 0; k < 6; k++)
			printf ("%02X\n", ch->rows[k] & 0xFF);

		printf ("ENDCHAR\n");
	}

	printf ("ENDFONT\n");
}

static void
usage (const char *program_name)
{
	fprintf (stderr, "Usage: %s { big | menu"
		" | small | small-bold }\n", program_name);
	exit (1);
}

int
main (int argc, char *argv[])
{
	if (argc != 2)
		usage (argv[0]);
	if (!strcmp (argv[1], "big"))
		generate_big_font ();
	else if (!strcmp (argv[1], "menu"))
		generate_menu_font ();
	else if (!strcmp (argv[1], "small"))
		generate_small_font ();
	else if (!strcmp (argv[1], "small-bold"))
		generate_small_bold_font ();
	else
		usage (argv[0]);

	return 0;
}

