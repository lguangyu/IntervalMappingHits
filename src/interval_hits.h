/*
 *
 */
#ifndef __INTERVAL_HITS_H__
#define __INTERVAL_HITS_H__

#include "config.h"

#include <iostream>
#include "interval.h"


class IntervalHits : public Interval
{
/* types */
public:
	using hits_t = unsigned long;

/* variables */
private:
	/* number of hits found */
	hits_t _num_hits;

/* methods */
public:
	explicit IntervalHits(Interval::coords_t start,
		Interval::coords_t end) noexcept
	: Interval(start, end)
	, _num_hits(0)
	{};

	/* get current number of hits on this interval */
	auto get_num_hits(void) const noexcept
	{
		return this->_num_hits;
	};

	/* match hits with another interval (query); if an hit is found, increment
	 * hits counter by 1 */
	void match_intersection(const Interval& query)
	{
		if (this->intersects(query))
			this->_num_hits++;
		return;
	};
};


#endif /* __INTERVAL_HITS_H__ */
