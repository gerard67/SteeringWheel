bin_PROGRAMS=dashboard dashboardcli

dashboardcli_SOURCES=src/gobject_cli.c src/enginespeed.h src/enginespeed.c src/vehiclespeed.h src/vehiclespeed.c
dashboard_SOURCES=src/gobject_gui.c src/enginespeed.h src/enginespeed.c src/vehiclespeed.h src/vehiclespeed.c

dashboardextradir = $(datadir)/@PACKAGE@
dashboardextra_DATA = src/dashboard.glade src/dashboard_top.svg src/dashboard_back.svg src/dashboard_cursor_small.svg src/dashboard_cursor_big.svg

dashboardcli_CFLAGS = $(GOBJECT_CFLAGS) 
dashboard_CFLAGS = $(GOBJECT_CFLAGS) $(GTK_CFLAGS) $(CAIRO_CFLAGS) $(LIBRSVG_CFLAGS) -export-dynamic -DDATADIR=\"$(datadir)/@PACKAGE@\"
dashboardcli_LDADD = $(GOBJECT_LIBS)
dashboard_LDADD = $(GOBJECT_LIBS) $(GTK_LIBS) $(CAIRO_LIBS) $(LIBRSVG_LIBS)