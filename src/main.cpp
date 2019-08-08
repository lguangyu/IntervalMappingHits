#include "config.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

#include <htslib/sam.h>

#include "argument.h"
#include "interval_collection.h"
#include "sam_bam_reader.h"


int main(const int argc, const char* argv[])
{
	Argument args;
	args.parse(argc, argv);
	/* load intervals */
	IntervalCollection intervals;
	try
	{
		std::ifstream intv_file(args.interval, std::ios_base::in);
		intv_file >> intervals;
		intv_file.close();
	}
	catch (std::exception& e)
	{
		std::cerr << "error: " << e.what() << '\n';
		return 1;
	}

	/* open reader */
	std::unique_ptr< ReaderBase > reader;
	try
	{
		reader.reset(args.is_bam ?
			static_cast< ReaderBase* >(new BamReader) :
			static_cast< ReaderBase* >(new SamReader));
		reader->open(args.mapping);
	}
	catch (std::exception& e)
	{
		std::cerr << "error: " << e.what() << '\n';
		return 1;
	}

	/* count hits */
	while (reader->read() >= 0)
	{
		auto hdr = reader->get_header();
		auto aln = reader->get_alignment();
		if (!(aln->core.flag & BAM_FUNMAP)) // only want mapped
			intervals.match_intersection(
				std::string(hdr->target_name[aln->core.tid]),
				reader->get_aln_interval());
	}

	/* output */
	if (args.output)
	{
		std::ofstream os(args.output, std::ios_base::out);
		os << intervals;
		os.close();
	}
	else
	{
		std::cout << intervals;
	}
	return 0;
}
