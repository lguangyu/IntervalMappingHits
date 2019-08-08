AUTOMAKE_OPTIONS =

# main binary
bin_PROGRAMS = src/interval_mapping_hits
src_interval_mapping_hits_SOURCES = \
	src/argument.cpp \
	src/interval_collection.cpp \
	src/main.cpp \
	src/reference_interval.cpp
src_interval_mapping_hits_CXXFLAGS = -std=c++17 -Wall \
	-I./src
src_interval_mapping_hits_LDADD = -lhts

# gtest framework
check_LIBRARIES = tests/gtest/libgtest.a
tests_gtest_libgtest_a_SOURCES = \
	tests/gtest/src/gtest-all.cc
tests_gtest_libgtest_a_CXXFLAGS = -std=c++17 -Wall -pthread \
	-I$(top_srcdir)/tests/gtest -I$(top_srcdir)/tests/gtest/include

# test targtes
check_PROGRAMS = tests/test_interval_mapping_hits
tests_test_interval_mapping_hits_SOURCES = \
	src/argument.cpp \
	src/interval_collection.cpp \
	src/reference_interval.cpp \
	tests/test_argument.cpp \
	tests/test_interval.cpp \
	tests/test_reference_interval.cpp \
	tests/test_main.cpp
tests_test_interval_mapping_hits_CXXFLAGS = -std=c++17 -Wall -pthread \
	-I$(top_srcdir)/src -I$(top_srcdir)/tests \
	-I$(top_srcdir)/tests/gtest/include
tests_test_interval_mapping_hits_LDFLAGS = -pthread -L./tests/gtest
tests_test_interval_mapping_hits_LDADD = -lhts -lgtest
tests_test_interval_mapping_hits_DEPENDENCIES = tests/gtest/libgtest.a

TESTS = tests/test_interval_mapping_hits
