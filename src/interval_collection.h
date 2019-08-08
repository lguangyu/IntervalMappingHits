/*
 *
 */
#ifndef __INTERVAL_COLLECTION_H__
#define __INTERVAL_COLLECTION_H__

#include "config.h"

#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include "reference_interval.h"


class IntervalCollection
{
/* types */
public:
	using unnamed_intv_list_t = std::vector< IntervalHits* >;
	using named_intv_list_t = std::vector< ReferenceInterval* >;
	using ref_hash_t = std::unordered_map< std::string, unnamed_intv_list_t >;

/* variables */
public:
	named_intv_list_t intervals;
	ref_hash_t ref_hash;

/* methods */
public:
	explicit IntervalCollection() noexcept
	: intervals(0)
	, ref_hash(0)
	{};
	IntervalCollection(const IntervalCollection&) = delete;
	IntervalCollection(IntervalCollection&&) = default;
	~IntervalCollection(void);

	/* add an interval to the collection */
	void add_interval(ReferenceInterval&);
	void add_interval(const std::string&);

	/* math hit with another interval;
	 * unlike IntervalHits::match_hit_with, this methods require the reference
	 * name of the query interval; */
	void match_intersection(const std::string&, const Interval&);

};


/* read input from stream, tsv format */
std::istream& operator>> (std::istream&, IntervalCollection&);

/* output results to ostream; operator<< keeps order as read by operator>> */
std::ostream& operator<< (std::ostream&, IntervalCollection&);


#endif /* __INTERVAL_COLLECTION_H__ */
