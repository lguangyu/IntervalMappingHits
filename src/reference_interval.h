/*
 *
 */
#ifndef __REFERENCE_INTERVAL_H__
#define __REFERENCE_INTERVAL_H__

#include "config.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include "interval_hits.h"


class ReferenceInterval : public IntervalHits
{
/* types */
public:
	struct invalid_format : std::runtime_error
	{
		invalid_format(const char* const message) throw()
		: std::runtime_error(message)
		{};
	};

/* variables */
public:
	std::string ref_name;

/* methods */
public:
	explicit ReferenceInterval(void) noexcept
	: IntervalHits(0, 0)
	, ref_name()
	{};
	explicit ReferenceInterval(const std::string& ref_name,
		IntervalHits::coords_t start, IntervalHits::coords_t end) noexcept
	: IntervalHits(start, end)
	, ref_name(ref_name)
	{};
	ReferenceInterval(const ReferenceInterval& other) noexcept
	: ReferenceInterval(other.ref_name, other.start, other.end)
	{};

	/* I/O as tab-delimited */
	void from_tab_delimited_line(const std::string&);
};


/* output to ostream */
std::ostream& operator<< (std::ostream&, const ReferenceInterval&);


#endif /* __REFERENCE_INTERVAL_H__ */
