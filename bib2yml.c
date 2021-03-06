#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <btparse.h>

#include "arg.h"

#ifndef VERSION
#define VERSION "0"
#endif

char *argv0;

void
usage(void)
{
	fprintf(stderr, "usage: %s [-hv] [-brty] [FILE.bib]\n", argv0);
	exit(1);
}

void
version(void)
{
	fprintf(stderr, "%s, version %s\n", argv0, VERSION);
	exit(1);
}

void
print_entry_open(AST *entry)
{
	char *entry_key = bt_entry_key(entry);
	printf("- id: \"%s\"\n", entry_key);
}

void
print_entry_close()
{
}

void
print_field(char *field_name, char *field_text)
{
	if (!strncmp(field_name, "author", 6) || !strncmp(field_name, "title", 5))
		bt_purify_string(field_text, 0);

	printf("  %s: \"%s\"\n", field_name, field_text);
}

int
main(int argc, char *argv[])
{
	ARGBEGIN {
	case 'v':
		version();
	case 'h':
	default:
		usage();
	} ARGEND

	int bt_parsed;
	AST *entries, *entry = NULL, *field = NULL, *value = NULL;
	char *field_name, *field_text;
	bt_nodetype field_type;

	bt_initialize();
	bt_set_stringopts(BTE_REGULAR, BTO_PASTE);

	entries = bt_parse_file(argv[0], 0, &bt_parsed);

	if (bt_parsed) {
		while ((entry = bt_next_entry(entries, entry))) {
			print_entry_open(entry);
			while ((field = bt_next_field(entry, field, &field_name))) {
				while ((value = bt_next_value(field, value, &field_type, &field_text))) {
					/* switch (field_type) { */
					/* case BTAST_STRING: */
					/* case BTAST_MACRO: */
					/* case BTAST_NUMBER: */
					/* } */
					print_field(field_name, field_text);
				}
			}
			print_entry_close();
		}
	} else {
		printf("error parsing\n");
		return 1;
	}

	bt_cleanup();
	return 0;
}
