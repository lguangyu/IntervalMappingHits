#include "reference_interval.h"


void ReferenceInterval::from_tab_delimited_line(const std::string& line)
{
	std::string::const_iterator separators[4];
	/* find first 3 separators '\t' in input line */
	separators[0] = line.begin();
	int i = 0;
	for (auto ii = line.begin(); ii != line.end(); ++ii)
	{
		switch (*ii)
		{
			case '\t':
			case '\n':
				i++;
				separators[i] = ii;
		}
		if (i == 3) /* no need to go futher with 3 columns already */
			break;
	}
	/* add the last column if ended with '\0' */
	if ((i < 3) && (separators[i] != line.end()))
	{
		i += 1;
		separators[i] = line.end();
	}
	/* check if enough fields; the minimal input should be 3 columns: (1) name
	 of reference; (2) start position (on reference); (3) end position; start
	 position by principal need to be less than end position, however it will
	 not be tested here -> look into IntervalCollection instead */
	if (i != 3)
		throw ReferenceInterval::invalid_format(
			"input must be at least 3-columns: (1) reference sequence name; "
			"(2) start position (int); (3) end position (int)");
	/* load data */
	this->ref_name = std::string(separators[0], separators[1]);
	//std::copy(separators[0], separators[1], this->ref_name.begin());
	//ref_name[separators[1] - separators[0]] = '\0'; /* mark a null-char */
	this->start = std::stoul(std::string(separators[1], separators[2]));
	this->end = std::stoul(std::string(separators[2], separators[3]));
	return;
}


std::ostream& operator<< (std::ostream& lhs, const ReferenceInterval& rhs)
{
	lhs << rhs.ref_name << '\t'
		<< rhs.start << '\t'
		<< rhs.end << '\t'
		<< rhs.get_num_hits();
	return lhs;
}
