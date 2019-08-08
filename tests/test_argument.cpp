/* testsuite for argument.cpp
 */
#include "gtest/gtest.h"
/* subject */
#include "argument.h"

namespace test_Argument
{

TEST(test_Argument, general)
{
	/* Argument::status_enum_t::no_error should be boolean false */
	ASSERT_FALSE(bool(Argument::status_enum_t::no_error));
};

TEST(test_parse, test_empty_args)
{
	/* empty arguments will be fine */
	{
		Argument subject;
		const char* argv[] = {};
		subject.parse(0, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
	};
	{
		Argument subject;
		const char* argv[] = {"prog"};
		subject.parse(1, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
	};
};

TEST(test_parse, test_flag_h)
{
	/* default should be false */
	{
		Argument subject;
		const char* argv[] = {"prog"};
		subject.parse(1, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_FALSE(subject.show_help);
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-h"};
		subject.parse(2, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_TRUE(subject.show_help);
	};
	/* combined */
	{
		Argument subject;
		const char* argv[] = {"prog", "-bh"};
		subject.parse(2, argv, 1);
		ASSERT_TRUE(subject.show_help);
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-bh"};
		subject.parse(2, argv, 1);
		ASSERT_TRUE(subject.show_help);
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-bsh"};
		subject.parse(2, argv, 1);
		ASSERT_TRUE(subject.show_help);
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-bhs"};
		subject.parse(2, argv, 1);
		ASSERT_TRUE(subject.show_help);
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-bshi"};
		subject.parse(2, argv, 1);
		ASSERT_TRUE(subject.show_help);
	};
	/* random string */
	{
		Argument subject;
		const char* argv[] = {"prog", "-adfaurjad"};
		subject.parse(2, argv, 1);
		ASSERT_FALSE(subject.show_help);
	};
	/* error should not affect '-h' */
	{
		Argument subject;
		const char* argv[] = {"prog", "-hadfaurjad@"};
		subject.parse(2, argv, 1);
		ASSERT_NE(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_TRUE(subject.show_help);
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-adfahurjad@"};
		subject.parse(2, argv, 1);
		ASSERT_NE(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_TRUE(subject.show_help);
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-adfaurjad@h"};
		subject.parse(2, argv, 1);
		ASSERT_NE(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_TRUE(subject.show_help);
	};
};

TEST(test_parse, test_flag_b_and_s)
{
	/* default should be bam */
	{
		Argument subject;
		const char* argv[] = {"prog"};
		subject.parse(1, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_TRUE(subject.is_bam);
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-b"};
		subject.parse(2, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_TRUE(subject.is_bam);
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-s"};
		subject.parse(2, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_FALSE(subject.is_bam);
	};
	/* multiple set */
	{
		Argument subject;
		const char* argv[] = {"prog", "-bb"};
		subject.parse(2, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::multiple_assign);
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-ss"};
		subject.parse(2, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::multiple_assign);
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-b", "-b"};
		subject.parse(3, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::multiple_assign);
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-s", "-s"};
		subject.parse(3, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::multiple_assign);
	};
	/* set both should be error */
	{
		Argument subject;
		const char* argv[] = {"prog", "-bs"};
		subject.parse(2, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::bam_sam_conflict);
	};
	/* no matter the order */
	{
		Argument subject;
		const char* argv[] = {"prog", "-sb"};
		subject.parse(2, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::bam_sam_conflict);
	};
	/* or splitted */
	{
		Argument subject;
		const char* argv[] = {"prog", "-b", "-s"};
		subject.parse(3, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::bam_sam_conflict);
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-s", "-b"};
		subject.parse(3, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::bam_sam_conflict);
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-hb", "-s"};
		subject.parse(3, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::bam_sam_conflict);
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-hs", "-b"};
		subject.parse(3, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::bam_sam_conflict);
	};
};

TEST(test_parse, test_flag_i)
{
	/* default is nullptr */
	{
		Argument subject;
		const char* argv[] = {"prog"};
		subject.parse(1, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_EQ(subject.interval, nullptr);
	};
	/* expect parameter */
	{
		Argument subject;
		const char* argv[] = {"prog", "-i"};
		subject.parse(2, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::expect_param);
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-hbi"};
		subject.parse(2, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::expect_param);
	};
	/* parse file name */
	{
		Argument subject;
		const char* argv[] = {"prog", "-iaaa"};
		subject.parse(2, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_STREQ(subject.interval, "aaa");
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-biaaa"};
		subject.parse(2, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_STREQ(subject.interval, "aaa");
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-ih"};
		subject.parse(2, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_STREQ(subject.interval, "h");
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-h", "-biaaa"};
		subject.parse(3, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_STREQ(subject.interval, "aaa");
	};
	/* with space in between */
	{
		Argument subject;
		const char* argv[] = {"prog", "-i", "aaa"};
		subject.parse(3, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_STREQ(subject.interval, "aaa");
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-hi", "aaa"};
		subject.parse(3, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_STREQ(subject.interval, "aaa");
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-ih", "aaa"};
		subject.parse(3, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_STREQ(subject.interval, "h");
	};
	/* multiple assignment */
	{
		Argument subject;
		const char* argv[] = {"prog", "-hi1", "-ih2"};
		subject.parse(3, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::multiple_assign);
	};
};

TEST(test_parse, test_flag_o)
{
	/* default is nullptr */
	{
		Argument subject;
		const char* argv[] = {"prog"};
		subject.parse(1, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_EQ(subject.output, nullptr);
	};
	/* expect parameter */
	{
		Argument subject;
		const char* argv[] = {"prog", "-o"};
		subject.parse(2, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::expect_param);
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-hbo"};
		subject.parse(2, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::expect_param);
	};
	/* parse file name */
	{
		Argument subject;
		const char* argv[] = {"prog", "-oaaa"};
		subject.parse(2, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_STREQ(subject.output, "aaa");
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-boaaa"};
		subject.parse(2, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_STREQ(subject.output, "aaa");
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-oh"};
		subject.parse(2, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_STREQ(subject.output, "h");
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-h", "-boaaa"};
		subject.parse(3, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_STREQ(subject.output, "aaa");
	};
	/* with space in between */
	{
		Argument subject;
		const char* argv[] = {"prog", "-o", "aaa"};
		subject.parse(3, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_STREQ(subject.output, "aaa");
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-ho", "aaa"};
		subject.parse(3, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_STREQ(subject.output, "aaa");
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-oh", "aaa"};
		subject.parse(3, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_STREQ(subject.output, "h");
	};
	/* multiple assignment */
	{
		Argument subject;
		const char* argv[] = {"prog", "-ho1", "-oh2"};
		subject.parse(3, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::multiple_assign);
	};
};

TEST(test_parse, test_positional)
{
	/* default is nullptr */
	{
		Argument subject;
		const char* argv[] = {"prog"};
		subject.parse(1, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_EQ(subject.mapping, nullptr);
	};
	/* use positional */
	{
		Argument subject;
		const char* argv[] = {"prog", "aaa"};
		subject.parse(2, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_STREQ(subject.mapping, "aaa");
	};
	/* and combinations */
	{
		Argument subject;
		const char* argv[] = {"prog", "-b", "aaa"};
		subject.parse(3, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_STREQ(subject.mapping, "aaa");
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-h", "aaa"};
		subject.parse(3, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_STREQ(subject.mapping, "aaa");
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "aaa", "-i", "bbb"};
		subject.parse(4, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_STREQ(subject.mapping, "aaa");
		ASSERT_STREQ(subject.interval, "bbb");
	};
	/* combinations with error */
	{
		Argument subject;
		const char* argv[] = {"prog", "-bs", "aaa"};
		subject.parse(3, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::bam_sam_conflict);
		ASSERT_STREQ(subject.mapping, "aaa");
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-ia", "aaa", "-ii"};
		subject.parse(4, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::multiple_assign);
		ASSERT_STREQ(subject.mapping, "aaa");
	};
	/* use '-' */
	{
		Argument subject;
		const char* argv[] = {"prog", "-"};
		subject.parse(2, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_STREQ(subject.mapping, "-");
	};
	/* and combinations */
	{
		Argument subject;
		const char* argv[] = {"prog", "-b", "-"};
		subject.parse(3, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_STREQ(subject.mapping, "-");
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-h", "-"};
		subject.parse(3, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_STREQ(subject.mapping, "-");
	};
	{
		Argument subject;
		const char* argv[] = {"prog", "-", "-i", "bbb"};
		subject.parse(4, argv, 1);
		ASSERT_EQ(subject.err_status, Argument::status_enum_t::no_error);
		ASSERT_STREQ(subject.mapping, "-");
		ASSERT_STREQ(subject.interval, "bbb");
	};
};

};
