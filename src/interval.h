/*
 *
 */
#ifndef __INTERVAL_H__
#define __INTERVAL_H__

#include "config.h"

#include <string>


class Interval
{
/* types */
public:
	using coords_t = unsigned long;

/* variables */
public:
	/* start/end position on the parent sequence, 0-based */
	/* start is assumed to be less than end; undefined if intendedly violated;
	 * NOTE: 0-based system uses [a, b) intervals; for more information, see
	 * SAM/BAM format maunual: https://samtools.github.io/hts-specs/SAMv1.pdf */
	coords_t start;
	coords_t end;

/* methods */
public:
	explicit Interval(coords_t start, coords_t end) noexcept
	: start(start)
	, end(end)
	{};

	auto get_length(void) noexcept
	{
		return (this->end - this->start);
	}

	/* check if <this> has has non-empty intersection with another interval,
	 * return true if so; */
	inline bool intersects(const Interval& other) noexcept
	{
		return ((this->start < other.end) && (other.start < this->end));
	};
};


#endif /* __INTERVAL_H__ */
