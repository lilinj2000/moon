#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(moon, 0.1, lilinj2000@gmail.com)
AC_CONFIG_SRCDIR([])
#AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE

#AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
LT_INIT
AC_PROG_RANLIB
# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
#AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
LT_RELEASE=0.1
LT_CURRENT=0
LT_REVISION=1
LT_AGE=0
AC_SUBST(LT_RELEASE)
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

home_3rd=`pwd`/../../3rd
home_es=`pwd`/../../es

dnl soil lib
AC_ARG_WITH([soil],
    [AS_HELP_STRING([--with-soil],
        [home path of the soil, defaults to $home_es/soil])],
    [SOIL_HOME="$withval"],
    [SOIL_HOME="$home_es/soil"])
AC_SUBST([SOIL_HOME])

SOIL_CPPFLAGS="-I$SOIL_HOME/include"
SOIL_LIBADD="$SOIL_HOME/lib/libsoil.la"
AC_SUBST(SOIL_CPPFLAGS)
AC_SUBST(SOIL_LIBADD)

dnl boost lib
AC_ARG_WITH([boost],
    [AS_HELP_STRING([--with-boost],
        [home path of the boost, defaults to $home_3rd/boost])],
    [BOOST_HOME="$withval"],
    [BOOST_HOME="$home_3rd/boost"])
AC_SUBST([BOOST_HOME])

BOOST_CPPFLAGS="-I$BOOST_HOME/include -DBOOST_ALL_DYN_LINK"
BOOST_LIBADD="$BOOST_HOME/lib/libboost_log_setup.so \
              $BOOST_HOME/lib/libboost_log.so \
	      $BOOST_HOME/lib/libboost_filesystem.so \
  	      $BOOST_HOME/lib/libboost_program_options.so \
	      $BOOST_HOME/lib/libboost_regex.so \
	      $BOOST_HOME/lib/libboost_thread.so \
	      $BOOST_HOME/lib/libboost_chrono.so \
	      $BOOST_HOME/lib/libboost_date_time.so \
	      $BOOST_HOME/lib/libboost_system.so"
AC_SUBST(BOOST_CPPFLAGS)
AC_SUBST(BOOST_LIBADD)

dnl thost api lib
CTP_HOME=$home_3rd/v6.3.6_20141230traderapi_linux64
AC_ARG_WITH([ctp],
    [AS_HELP_STRING([--with-ctp],
        [path of the ctp api, defaults to $CTP_HOME])],
    [CTP_HOME="$withval"],
    [CTP_HOME="$CTP_HOME"])
AC_SUBST([CTP_HOME])

CTP_CPPFLAGS=-I$CTP_HOME
CTP_LIBADD="-L$CTP_HOME -lthostmduserapi -lthosttraderapi"
AC_SUBST(CTP_CPPFLAGS)
AC_SUBST(CTP_LIBADD)

dnl es-ctp lib
ES_CTP_HOME=$home_es/ctp
AC_ARG_WITH([es-ctp],
    [AS_HELP_STRING([--with-es-ctp],
        [home path of the es-ctp, defaults to $ES_CTP_HOME])],
    [ES_CTP_HOME="$withval"],
    [ES_CTP_HOME="$ES_CTP_HOME"])
AC_SUBST([ES_CTP_HOME])

ES_CTP_CPPFLAGS="-I$ES_CTP_HOME/include"
ES_CTP_LIBADD="$ES_CTP_HOME/lib/libctp.la"
AC_SUBST(ES_CTP_CPPFLAGS)
AC_SUBST(ES_CTP_LIBADD)

dnl gtest lib
AC_ARG_WITH([gtest],
    [AS_HELP_STRING([--with-gtest],
        [home path of the gtest, defaults to $home_3rd/gtest])],
    [GTEST_HOME="$withval"],
    [GTEST_HOME="$home_3rd/gtest"])
AC_SUBST([GTEST_HOME])

GTEST_CPPFLAGS=-I$GTEST_HOME/include
GTEST_LIBADD="$GTEST_HOME/lib/libgtest.a $GTEST_HOME/lib/libgtest_main.a"
AC_SUBST(GTEST_CPPFLAGS)
AC_SUBST(GTEST_LIBADD)

AC_OUTPUT([
	Makefile])

