AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = moon
moon_SOURCES = \
	Config.cc \
	MDServiceCallbackImpl.cc \
	TraderServiceCallbackImpl.cc \
	MoonServer.cc \
	Strategy.cc \
	Main.cc

moon_CPPFLAGS =  \
		$(BOOST_CPPFLAGS) \
		$(GTEST_CPPFLAGS) \
		$(SOIL_CPPFLAGS) \
		$(CATA_CPPFLAGS)


moon_LDADD = \
	$(BOOST_LIBADD) \
	$(THOST_LIBADD) \
	$(SOIL_LIBADD) \
	$(CATA_LIBADD)

moon_LDFLAGS = $(LIBADD_DL) -Wl,--no-as-needed -pthread

# header files
noinst_HEADERS = Config.hh \
	MDServiceCallbackImpl.hh \
	TraderServiceCallbackImpl.hh \
	MoonServer.hh \
	Strategy.hh \
	Log.hh

EXTRA_DIST = env.sh moon.cfg log.cfg

moondir = $(bindir)
moon_DATA = $(EXTRA_DIST)
