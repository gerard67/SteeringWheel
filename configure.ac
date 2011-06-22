dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(gobject_tutorial, 1.0, mathieu.petit@imag.fr)

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([-Wall -Werror])

AC_PROG_CC

PKG_CHECK_MODULES([GLIB], [glib-2.0 >= 2.0])
PKG_CHECK_MODULES([GOBJECT], [gobject-2.0 >= 2.0])

AC_CONFIG_FILES(Makefile)
AC_OUTPUT