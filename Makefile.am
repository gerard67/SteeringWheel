bin_PROGRAMS=dashboard

dashboard_SOURCES=src/gobject_gui.c src/locationdata.h src/locationdata.c src/enginespeed.h src/enginespeed.c src/vehiclespeed.h src/vehiclespeed.c

dashboardextradir = $(datadir)/@PACKAGE@
dashboardextra_DATA = src/dashboard.glade src/dashboard_top.svg src/dashboard_back.svg src/dashboard_cursor_small.svg src/dashboard_cursor_big.svg

dashboard_CFLAGS = $(MYSQL_CFLAGS) $(GOBJECT_CFLAGS) $(CLUTTERGTK_CFLAGS) $(GTK_CFLAGS) $(CAIRO_CFLAGS) $(LIBRSVG_CFLAGS) $(CHAMPLAINGTK_CFLAGS) -export-dynamic -DDATADIR=\"$(datadir)/@PACKAGE@\"
dashboard_LDADD = $(MYSQL_LIBS) $(GOBJECT_LIBS) $(CLUTTERGTK_LIBS) $(GTK_LIBS) $(CAIRO_LIBS) $(LIBRSVG_LIBS) $(CHAMPLAINGTK_LIBS)