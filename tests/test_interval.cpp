/* testsuite for interval.cpp
 */
#include "gtest/gtest.h"
/* subject */
#include "interval.h"

namespace test_Interval
{

TEST(test_Interval, test_intersects)
{
	Interval subject(10, 20);
	Interval::coords_t start, end;
	/* query completely ahead */
	start = 0;
	for (end = start + 1; end < 10; ++end)
		ASSERT_FALSE(subject.intersects(Interval(start, end)));
	/* query may partially intersect from ahead */
	start = 0;
	for (end = start + 1; end < 20; ++end)
		ASSERT_EQ(subject.intersects(Interval(start, end)), end > 10);
	/* query completely inside */
	for (start = 10; start < 20; ++start)
		for (end = start + 1; end < 20; ++end)
			ASSERT_TRUE(subject.intersects(Interval(start, end)));
	/* query completely contains */
	for (start = 0; start < 11; ++start)
		for (end = 20; end < 21; ++end)
			ASSERT_TRUE(subject.intersects(Interval(start, end)));
	/* query may partially intersect from behind */
	end = 30;
	for (start = 10; start < end; ++start)
		ASSERT_EQ(subject.intersects(Interval(start, end)), start < 20);
	/* query completely hehind */
	end = 30;
	for (start = 20; start < end; ++start)
		ASSERT_FALSE(subject.intersects(Interval(start, end)));
};

};
