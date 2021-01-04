#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <btparse.h>

#include "arg.h"

#ifndef VERSION
#define VERSION "0"
#endif

typedef enum {BIBTEX, REFER, TSV, YAML} FORMAT;
typedef enum {ID, AUTHOR, TITLE, YEAR, URL, JOURNAL, LOC, ISBN} FIELDS;
const char *FIELD_NAMES[] = {"id", "author", "title", "year", "url", "journal", "loc", "isbn"};
const char REFER_KEYS[] = {'K', 'A', 'T', 'D', 'U', 'J', 'O', 'O'};

struct CONFIG {
	FORMAT format; size_t fields_size; FIELDS fields[];
} c = {
	BIBTEX, 2, {AUTHOR, TITLE}
};
char *argv0;

void
usage(void)
{
	fprintf(stderr, "usage: %s [-hv] [-brty] FILE.bib\n", argv0);
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
	char *entry_type = bt_entry_type(entry);
	char *entry_key = bt_entry_key(entry);
	switch (c.format) {
	case REFER:
		printf("%%K %s\n", entry_key);
		break;
	case BIBTEX:
		printf("@%s{%s\n", entry_type, entry_key);
		break;
	case TSV:
		break;
	case YAML:
		printf("- id: \"%s\"\n", entry_key);
		break;
	}
}

void
print_entry_close()
{
	switch (c.format) {
	case BIBTEX:
		printf("}\n");
		break;
	case REFER:
		printf("\n");
		break;
	case TSV:
		printf("\n");
		break;
	case YAML:
		break;
	}
}

void
print_field(char *field_name, char *field_text)
{
	if (!strncmp(field_name, "author", 6) || !strncmp(field_name, "title", 5))
		bt_purify_string(field_text, 0);

	switch (c.format) {
	case BIBTEX:
		printf("\t%s = {%s},\n", field_name, field_text);
		break;
	case REFER:
		for (int field = ID; field != ISBN; field++) {
			if (!strcmp(field_name, FIELD_NAMES[field])) {
				printf("%%%c %s\n", REFER_KEYS[field], field_text);
				break;
			}
		}
		break;
	case TSV:
		for (int i = 0; i < c.fields_size; i++) {
			if (!strcmp(field_name, FIELD_NAMES[c.fields[i]]))
				printf("%s\t", field_text);
		}
		/* if (!strncmp(field_name, "author", 6) || !strncmp(field_name, "title", 5)) */
		/* 	printf("%s\t", field_text); */
		break;
	case YAML:
		printf("  %s: \"%s\"\n", field_name, field_text);
		break;
	}
}

int
main(int argc, char *argv[])
{
	ARGBEGIN {
	case 'b':
		c.format = BIBTEX;
		break;
	case 'r':
		c.format = REFER;
		break;
	case 't':
		c.format = TSV;
		break;
	case 'y':
		c.format = YAML;
		break;
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
