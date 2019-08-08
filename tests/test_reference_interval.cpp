/* testsuite for reference_interval.cpp
 */
#include "gtest/gtest.h"
/* subject */
#include "reference_interval.h"

namespace test_ReferenceInterval
{

TEST(test_ReferenceInterval, test_from_tab_delimited_line)
{
	ReferenceInterval subject;
	/* ok with three columns, ended with '\0' */
	subject.from_tab_delimited_line("ref\t1\t2");
	ASSERT_STREQ(subject.ref_name.c_str(), "ref");
	ASSERT_EQ(subject.start, 1);
	ASSERT_EQ(subject.end, 2);

	/* ok with three columns, ended with '\t' */
	subject.from_tab_delimited_line("ref\t4\t55\t");
	ASSERT_STREQ(subject.ref_name.c_str(), "ref");
	ASSERT_EQ(subject.start, 4);
	ASSERT_EQ(subject.end, 55);

	/* ok with three columns, ended with '\t' */
	/* end < start should not be problematic here */
	/* excessive field(s) shoule be fine */
	subject.from_tab_delimited_line("ref\t4\t0\0a");
	ASSERT_STREQ(subject.ref_name.c_str(), "ref");
	ASSERT_EQ(subject.start, 4);
	ASSERT_EQ(subject.end, 0);

	/* not enough columns */
	ASSERT_THROW(subject.from_tab_delimited_line(""),
		ReferenceInterval::invalid_format);
	ASSERT_THROW(subject.from_tab_delimited_line("ref\t4"),
		ReferenceInterval::invalid_format);
	ASSERT_THROW(subject.from_tab_delimited_line("\t"),
		ReferenceInterval::invalid_format);

	/* invalid stoi arguments */
	ASSERT_THROW(subject.from_tab_delimited_line("\t\t"),
		std::invalid_argument);
};

};
