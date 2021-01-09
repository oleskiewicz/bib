include config.mk

BIN=bibfmt bib2ref bib2tsv bib2yml

all: $(BIN)

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin && install -m0755 $(BIN) $(DESTDIR)$(PREFIX)/bin
	# mkdir -p $(DESTDIR)$(MANPREFIX)/man/man1 && install -m0644 bib.1 $(DESTDIR)$(MANPREFIX)/man/man1

uninstall:
	for bin in $(BIN); do rm -f $(DESTDIR)$(PREFIX)/bin/$$bin; done
	# rm -f $(DESTDIR)$(MANPREFIX)/man/man1/bib.1

clean:
	rm -f *.o $(BIN)
