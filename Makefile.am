bin_PROGRAMS=ecodrive

ecodrive_SOURCES=src/ecodrive_main.c src/views/ecodrivemap.h src/views/ecodrivemap.c src/models/models/locationdata.h src/models/locationdata.c src/models/enginespeed.h src/models/enginespeed.c src/models/vehiclespeed.h src/models/vehiclespeed.c

ecodriveextradir = $(datadir)/@PACKAGE@
ecodriveextra_DATA = src/ecodrive_ui.glade src/data/dashboard_top.svg src/data/dashboard_back.svg src/data/dashboard_cursor_small.svg src/data/dashboard_cursor_big.svg

ecodrive_CFLAGS = $(MYSQL_CFLAGS) $(GOBJECT_CFLAGS) $(CLUTTERGTK_CFLAGS) $(GTK_CFLAGS) $(CAIRO_CFLAGS) $(LIBRSVG_CFLAGS) $(CHAMPLAINGTK_CFLAGS) -export-dynamic -DDATADIR=\"$(datadir)/@PACKAGE@\"
ecodrive_LDADD = $(MYSQL_LIBS) $(GOBJECT_LIBS) $(CLUTTERGTK_LIBS) $(GTK_LIBS) $(CAIRO_LIBS) $(LIBRSVG_LIBS) $(CHAMPLAINGTK_LIBS)