include config.mk

all: bib

install: bib
	mkdir -p $(DESTDIR)$(PREFIX)/bin && install -m0755 bib $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(MANPREFIX)/man/man1 && install -m0644 bib.1 $(DESTDIR)$(MANPREFIX)/man/man1

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/bib
	rm -f $(DESTDIR)$(MANPREFIX)/man/man1/bib.1.gz

bib: bib.o
	$(LD) -o $@ $< $(LDLIBS) $(LDFLAGS)

bib.o: bib.c arg.h

clean:
	rm -f *.o bib
