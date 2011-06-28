bin_PROGRAMS=heading headingcli

headingcli_SOURCES=src/gobject_cli.c src/heading.h src/heading.c src/speed.h src/speed.c
heading_SOURCES=src/gobject_gui.c src/heading.h src/heading.c src/speed.h src/speed.c

headingextradir = $(datadir)/@PACKAGE@
headingextra_DATA = src/heading.glade

headingcli_CFLAGS = $(GOBJECT_CFLAGS)
heading_CFLAGS = $(GOBJECT_CFLAGS) $(GTK_CFLAGS) $(CAIRO_CFLAGS) -export-dynamic -DDATADIR=\"$(datadir)/@PACKAGE@\"
headingcli_LDADD = $(GOBJECT_LIBS)
heading_LDADD = $(GOBJECT_LIBS) $(GTK_LIBS) $(CAIRO_LIBS)