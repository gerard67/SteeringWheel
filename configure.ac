dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(dashboard, 1.0, mathieu.petit@imag.fr)
LT_INIT
AC_CONFIG_MACRO_DIR([m4])

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL

PKG_CHECK_MODULES([GOBJECT], [gobject-2.0 >= 2.0])
PKG_CHECK_MODULES([GTK], [gtk+-3.0 >= 3.0])
PKG_CHECK_MODULES([CAIRO], [cairo >= 1.10])
PKG_CHECK_MODULES([CLUTTERGTK], [clutter-gtk-1.0 >= 1.0.0])
PKG_CHECK_MODULES([LIBRSVG], [librsvg-2.0 >= 2.32])
PKG_CHECK_MODULES([CHAMPLAINGTK], [champlain-gtk-0.10 >= 0.10.0])

#get mysql libraries and include locations, we use mysql-config.
AC_ARG_WITH(mysql-config,
  AS_HELP_STRING([--with-mysql-config=PATH],[The path to mysql-config if not in $PATH]), 
  [mysql_config_prog=$withval]
)
AC_PATH_PROG(MYSQL_CONFIG, mysql_config, $mysql_config_prog)

if test "x$MYSQL_CONFIG" = "x"; then
	AC_MSG_ERROR([Couldn't find mysql_config. Please verify that it is installed.])
fi
MYSQL_CFLAGS=`$MYSQL_CONFIG --cflags`
MYSQL_LIBS=`$MYSQL_CONFIG --libs_r`
AC_SUBST([MYSQL_CFLAGS])
AC_SUBST([MYSQL_LIBS])



AC_CONFIG_FILES(Makefile)
AC_OUTPUT