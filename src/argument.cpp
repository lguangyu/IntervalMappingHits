#include "argument.h"


void Argument::print_usage(std::ostream& os, const char* program) const
{
	os << "usage:\n    " << program << Argument::usage;
	return;
}


void Argument::_set_error(Argument::status_enum_t status, std::string msg)
{
	if (this->err_status == Argument::status_enum_t::no_error)
	{
		this->err_status = status;
		this->err_msg = msg;
		//this->err_msg.assign(msg.begin(), msg.end());
	}
	return;
}


void Argument::parse(const int argc, const char* argv[], int _test_mode)
{
	this->_parse_argv(argc, argv);
	/* premature return */ if (_test_mode == 1) return;
	this->_verify_args(argc, argv);
	/* premature return */ if (_test_mode == 2) return;
	/* check if need to interrupt, show_help disregard potential errors of other
	 * argument(s) and always print help then exit if it is set */
	if (this->show_help)
	{
		this->print_usage(std::cout, argv[0]);
		exit(0);
	}
	if (this->has_error())
	{
		std::cerr << "error: " << this->err_msg << '\n';
		std::cerr << "use " << argv[0] << " -h to get help" << '\n';
		exit(1);
	}
	return;
}


/* a boost-free implementation */
void Argument::_parse_argv(const int argc, const char* argv[])
{
	bool set_bam = false, set_sam = false;
	for (int i = 1; i < argc; ++i)
	{
		auto c = argv[i];
		if ((*c == '-') && *(c + 1))
		{
			while (*(++c))
			/* inner loop, go through all chars in an argument */
			{
				switch (*c)
				{
				case 'h':
					this->show_help = true;
					break;
				case 'b':
					if (set_bam)
					{
						this->_set_error(Argument::status_enum_t::multiple_assign,
							"multiple assignments into -b");
						break;
					}
					else if (set_sam)
					{
						this->_set_error(Argument::status_enum_t::bam_sam_conflict,
							"-b is exclusive with -s");
						break;
					}
					else
					{
						this->is_bam = true;
						set_bam = true;
						break;
					}
				case 's':
					if (set_sam)
					{
						this->_set_error(Argument::status_enum_t::multiple_assign,
							"multiple assignments into -s");
						break;
					}
					else if (set_bam)
					{
						this->_set_error(Argument::status_enum_t::bam_sam_conflict,
							"-s is exclusive with -b");
						break;
					}
					else
					{
						this->is_bam = false;
						set_sam = true;
						break;
					}
				case 'i':
					if (this->interval)
					{
						this->_set_error(Argument::status_enum_t::multiple_assign,
							"multiple assignments into -i");
						break;
					}
					if (*(c + 1))
					{
						/* if in case -iinput */
						this->interval = c + 1;
						goto next_arg;
					}
					else if (i < argc - 1)
					{
						this->interval = argv[++i];
						goto next_arg;
					}
					else
						this->_set_error(Argument::status_enum_t::expect_param,
							"-i expects one parameter");
					break;
				case 'o':
					if (this->output)
					{
						this->_set_error(Argument::status_enum_t::multiple_assign,
							"multiple assignments into -o");
						break;
					}
					if (*(c + 1))
					{
						this->output = c + 1;
						goto next_arg;
					}
					else if (i < argc - 1)
					{
						this->output = argv[++i];
						goto next_arg;
					}
					else
						this->_set_error(Argument::status_enum_t::expect_param,
							"-o expects one parameter");
					break;
				case '-': /* not allow full-name style (--argument or so) */
					this->_set_error(Argument::status_enum_t::unknown_arg,
						std::string("unknown argument ") + argv[i]);
					goto next_arg; /* skip remaining chars in this argument */
				default:
					this->_set_error(Argument::status_enum_t::unknown_arg,
						std::string("unknown argument ") + argv[i]);
					break;
				}
			}
		}
		else if (*c)
		{
			/* encouters bare '-' or its a positional */
			if (this->mapping)
				this->_set_error(Argument::status_enum_t::multiple_assign,
					"multiple assignments into bam/sam");
			else
				this->mapping = c;
		}
		next_arg:
		continue;
	}
	return;
}


void Argument::_verify_args(const int argc, const char* argv[])
{
	/* set to use stdin */
	if (!this->mapping)
		this->mapping = "-";
	/* required field */
	if (!this->interval)
		this->_set_error(Argument::status_enum_t::required_missing,
			"argument '-i' is required");
	/* output default is stdout, but we also allow '-' */
	if (const char* c = this->output; (c) && (c[0] == '-') && (c[1] == '\0'))
		this->output = nullptr; // use nullptr as unified
	return;
}
