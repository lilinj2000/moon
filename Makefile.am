AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = moon
moon_SOURCES = \
	MoonConfig.cc \
	MDServiceCallbackImpl.cc \
	TraderServiceCallbackImpl.cc \
	MoonServer.cc \
	Strategy.cc \
	Main.cc

moon_CPPFLAGS =  \
		$(BOOST_CPPFLAGS) \
		$(GTEST_CPPFLAGS) \
		$(SOIL_CPPFLAGS) \
		$(ES_CTP_CPPFLAGS)


moon_LDADD = \
	$(BOOST_LIBADD) \
	$(THOST_LIBADD) \
	$(SOIL_LIBADD) \
	$(ES_CTP_LIBADD)

moon_LDFLAGS = $(LIBADD_DL) -Wl,--no-as-needed -pthread

# header files
noinst_HEADERS = MoonConfig.hh \
	MDServiceCallbackImpl.hh \
	TraderServiceCallbackImpl.hh \
	MoonServer.hh \
	Strategy.hh \
	MoonLog.hh

extra_files = env.sh.example moon.cfg.example log.cfg.example

EXTRA_DIST = $extra_files

moondir = $(bindir)
moon_DATA = $extra_files
