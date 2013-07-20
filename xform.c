/*
 * Trivial ASCII to C source transformation tool
 * Public domain
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define H_MAX 32
#define W_MAX 32

int
main (int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf (stderr, "Usage: %s font_height font_width < input\n", argv[0]);
		exit (1);
	}

	char *p;
	int font_height = strtol (argv[1], &p, 10);
	if (*p || p == argv[1] || font_height <= 0 || font_height > H_MAX)
	{
		fprintf (stderr, "invalid font height\n");
		exit (1);
	}

	int bit_width = strtol (argv[2], &p, 10);
	if (*p || p == argv[2] || bit_width   <= 0 || bit_width   > W_MAX)
	{
		fprintf (stderr, "invalid font width\n");
		exit (1);
	}

	// Width in pixels to width in bits, rounded up to bytes
	bit_width = (bit_width + 7) / 8 * 8;

	int c, irow = 0, icol = 0, charw = 0;
	char row_data[H_MAX * W_MAX] = { 0 };

	printf ("struct\n");
	printf ("{\n");
	printf ("\tint width;\n");
	printf ("\tint advance;\n");
	printf ("\tint rows[%d];\n", font_height);
	printf ("}\n");
	printf ("font_data[] =\n");
	printf ("{\n");

	while ((c = getchar ()) != EOF)
	{
		if (c == '\n')
		{
			if (irow == 0)
			{
				// We expect a new character definition
				if (icol != 0)
				{
					charw = icol;

					icol = 0;
					irow++;
				}
			}
			else if (irow < font_height)
			{
				// Just checking on consistency
				if (icol != charw)
				{
					fprintf (stderr, "wrong line width\n");
					exit (1);
				}

				icol = 0;
				irow++;
			}
			else
			{
				// Flushing the character after a newline
				if (icol != 0)
				{
					fprintf (stderr, "character too high\n");
					exit (1);
				}

				// Character width, horizontal advance
				printf ("\t{ %2d, %2d, { ", charw, charw + 1);

				int y, x;
				for (y = 0; ; )
				{
					unsigned bits = 0;
					for (x = 0; x < charw; x++)
						bits = (bits << 1) | row_data[y * W_MAX + x];
					if (charw < bit_width)
						bits <<= (bit_width - charw);

					printf ("0x%0*x", (bit_width + 3) / 4, bits);
					if (++y != font_height)
						printf (", ");
					else
						break;
				}

				printf (" } },\n");

				memset (row_data, 0, sizeof row_data);
				icol = irow = charw = 0;
			}
		}
		else if (c == '#')
		{
			// Comments
			while ((c = getchar ()) != '\n' && c != EOF)
				;
			ungetc ('\n', stdin);
		}
		else
		{
			if (icol == W_MAX)
			{
				fprintf (stderr, "character too wide\n");
				exit (1);
			}

			if (c != '0' && c != '1')
			{
				fprintf (stderr, "invalid input '%c'\n", c);
				exit (1);
			}
			row_data[irow * W_MAX + icol++] = 1 - (c - '0');
		}
	}

	if (icol != 0)
	{
		fprintf (stderr, "unexpected EOF\n");
		exit (1);
	}

	printf ("};\n");
	return 0;
}

