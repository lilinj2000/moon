AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = moon
moon_SOURCES = \
	Config.cc \
	Tick.cc \
	Order.cc \
	State.cc \
	Context.cc \
	Server.cc \
	Main.cc

moon_CPPFLAGS =  \
		$(BOOST_CPPFLAGS) \
		$(GTEST_CPPFLAGS) \
		$(SOIL_CPPFLAGS) \
		$(SUBJECT_CPPFLAGS) \
		$(RAPIDJSON_CPPFLAGS) \
		$(JSON_CPPFLAGS) \
		$(ZOD_CPPFLAGS)


moon_LDADD = \
	$(BOOST_LIBADD) \
	$(SOIL_LIBADD) \
	$(SUBJECT_LIBADD) \
	$(JSON_LIBADD) \
	$(ZOD_LIBADD)

moon_LDFLAGS = $(LIBADD_DL) -Wl,--no-as-needed -pthread

# header files
noinst_HEADERS = Config.hh \
	State.hh \
	Order.hh \
	Context.hh \
	Tick.hh \
	Server.hh \
	Log.hh

EXTRA_DIST = env.sh moon.cfg log.cfg

moondir = $(bindir)
moon_DATA = $(EXTRA_DIST)
