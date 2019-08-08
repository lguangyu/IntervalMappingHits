#include "interval_collection.h"


IntervalCollection::~IntervalCollection(void)
{
	for (auto&& intv_p : this->intervals)
		delete intv_p;
	return;
}


void IntervalCollection::add_interval(ReferenceInterval& interval)
{
	if (interval.start >= interval.end)
		throw std::invalid_argument("interval 'start' must be less than "
			"'end'");
	(this->intervals).push_back(&interval);
	(this->ref_hash)[interval.ref_name].push_back(&interval);
	return;
}


void IntervalCollection::add_interval(const std::string& tsv_line)
{
	/* create interval */
	auto* intv_p = new ReferenceInterval();
	intv_p->from_tab_delimited_line(tsv_line);
	this->add_interval(*intv_p);
	return;
}


void IntervalCollection::match_intersection(const std::string& ref_name,
	const Interval& query)
{
	if (this->ref_hash.find(ref_name) == this->ref_hash.end())
		/* no need to do anything if no such reference in interest */
		return;
	/* check each interval with the query ref_name */
	auto& ref_intervals = (this->ref_hash)[ref_name];
	for (auto&& ref_intv_p : ref_intervals)
		ref_intv_p->match_intersection(query);
	return;
}

std::istream& operator>> (std::istream& lhs, IntervalCollection& rhs)
{
	std::string line;
	while (std::getline(lhs, line))
		/* this contains some repetitive work, but fine... it's fasta comparing
		 * to loop through the bam/sam file */
		rhs.add_interval(line);
	return lhs;
}

std::ostream& operator<< (std::ostream& lhs, IntervalCollection& rhs)
{
	for (auto& intv_ptr : rhs.intervals)
		lhs << (*intv_ptr) << '\n';
	return lhs;
}
