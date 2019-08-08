/*
 *
 */
#ifndef __ARGUMENT_H__
#define __ARGUMENT_H__

#include "config.h"

#include <iostream>
#include <string>
#include <utility>


class Argument
{
/* types */
public:
	using status_enum_t = enum {
		no_error = 0,
		unknown_arg,
		multiple_assign,
		bam_sam_conflict,
		expect_param,
		required_missing
	};

/* variables */
public:
	bool show_help;
	bool is_bam;
	/* parameters points into argv */
	const char* interval;
	const char* mapping;
	const char* output;
	status_enum_t err_status;
	std::string err_msg;
public:
	static constexpr const char* const usage =
" [-h] [-b|-s] -i interval [-o output] [bam/sam]\n\n"
"positional:\n"
"    bam/sam\n"
"         input bam/sam file\n"
"options:\n"
"    -h   show this help and exit\n"
"    -b   input is BAM (exclusive with -s) (default)\n"
"    -s   input is SAM (exclusive with -b)\n"
"    -i interval\n"
"         intervals list, tab-delimited (required)\n"
"    -o output\n"
"         write output to this file instead of stdout\n";

/* methods */
public:
	explicit Argument(void) noexcept
	: show_help(false)
	, is_bam(true)
	, interval(nullptr)
	, mapping(nullptr)
	, output(nullptr)
	// errors
	, err_status(Argument::status_enum_t::no_error)
	, err_msg()
	{};
	Argument(const Argument&) = delete;
	Argument(Argument&&) = delete;

	void print_usage(std::ostream&, const char* program) const;
	/* parse args from argc and argv, _test_mode should only be used during
	 * test; non-zero _test_mode cause parse() to return prematurely, enabling
	 * tracking down raw argument errors during parsing; _test_mode accepts
	 * value int 0-2, default (0) is not running under test mode */
	void parse(const int argc, const char* argv[], int _test_mode = 0);
	auto has_error(void)
	{
		return (this->err_status != Argument::status_enum_t::no_error);
	};

private:
	/* set internal error status during parsing; though errors are recognized
	 * but will not directly reported since '-h' will ignore all errors; the
	 * recognized error will be checked/processed later in _verify_args() */
	void _set_error(Argument::status_enum_t, std::string);
	/* main parser of argv values, recognize potential error associated with
	 * each argument; note required argument(s) are not identified here; */
	void _parse_argv(const int, const char*[]);
	/* verify parsed args, check missing required args (if any) and process
	 * '-h' or recognized errors; */
	void _verify_args(const int, const char*[]);
};


#endif /* __ARGUMENT_H__ */
