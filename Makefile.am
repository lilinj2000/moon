AUTOMAKE_OPTIONS = foreign

# noinst_PROGRAMS = unittest
# unittest_SOURCES = \
# 		Config.cc \
# 		ConfigTest.cc \
# 		MDServiceCallbackImpl.cc \
# 		TraderServiceCallbackImpl.cc \
# 		MoonServer.cc \
# 		MoonServerTest.cc \
# 		Strategy.cc

# unittest_CPPFLAGS = \
# 		$(BOOST_CPPFLAGS) \
# 		$(GTEST_CPPFLAGS) \
# 		$(SOIL_CPPFLAGS) \
# 		$(ES_CTP_CPPFLAGS)


# unittest_LDADD = \
# 	$(GTEST_LIBADD) \
# 	$(BOOST_LIBADD) \
# 	$(THOST_LIBADD) \
# 	$(SOIL_LIBADD) \
# 	$(ES_CTP_LIBADD)



# unittest_LDFLAGS = $(LIBADD_DL) -Wl,--no-as-needed -pthread

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

EXTRA_DIST = env.sh moon.cfg log.cfg

moondir = $(bindir)
moon_DATA = env.sh moon.cfg log.cfg
