dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(heading, 1.0, mathieu.petit@imag.fr)

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([-Wall -Werror])

AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL

PKG_CHECK_MODULES([GOBJECT], [gobject-2.0 >= 2.0])
PKG_CHECK_MODULES([GTK], [gtk+-3.0 >= 3.0])
PKG_CHECK_MODULES([CAIRO], [cairo >= 1.10])


AC_CONFIG_FILES(Makefile)
AC_OUTPUT