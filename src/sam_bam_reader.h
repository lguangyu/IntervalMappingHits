/*
 *
 */
#ifndef __SAM_BAM_READER_H__
#define __SAM_BAM_READER_H__

#include "config.h"

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>

#include <htslib/hts.h>
#include <htslib/bgzf.h>
#include <htslib/sam.h>

#include "interval.h"


/* these are unified wrapper to operate bam/sam files */
/* sam reader methods */
struct _SamReadWrapper
{
	using stream_t = samFile;
	/* below two should not be changed */
	using hdr_t = bam_hdr_t;
	using aln_t = bam1_t;
	/* wrapped interface */
	static auto open(const char* fn)
	{
		return sam_open(fn, "r");
	};
	// close and release resources
	static void close(stream_t* fp, hdr_t* hdr, aln_t* aln)
	{
		/* for sam_close() [alias of hts_close], test nullptr is necessary
		 * segfault otherwise since dereferenced immediately in hts_close() */
		if (fp) sam_close(fp);
		bam_hdr_destroy(hdr);
		bam_destroy1(aln);
		return;
	};
	// allocate reader buffer
	static auto aln_alloc(void)
	{
		return bam_init1();
	};
	// read header
	static auto hdr_read(stream_t* fp)
	{
		return sam_hdr_read(fp);
	};
	// read next alignment
	static auto aln_read(stream_t* fp, hdr_t* hdr, aln_t* aln)
	{
		return sam_read1(fp, hdr, aln);
	};
	// get the raw interval of alignment, only converts start and end positions
	static auto aln_to_inteval(aln_t* aln)
	{
		auto pos = aln->core.pos - 1; // sam is 1-based
		auto len = aln->core.l_qseq;
		return Interval(pos, pos + len);
	};
};


/* bam reader methods */
struct _BamReadWrapper
{
	using stream_t = BGZF;
	/* below two should not be changed */
	using hdr_t = bam_hdr_t;
	using aln_t = bam1_t;
	/* wrapped interface */
	static auto open(const char* fn)
	{
		return bgzf_open(fn, "r");
	};
	// close and release resources
	static void close(stream_t* fp, hdr_t* hdr, aln_t* aln)
	{
		if (fp) bgzf_close(fp);
		bam_hdr_destroy(hdr);
		bam_destroy1(aln);
		return;
	};
	// allocate reader buffer
	static auto aln_alloc(void)
	{
		return bam_init1();
	};
	// read header
	static auto hdr_read(stream_t* fp)
	{
		return bam_hdr_read(fp);
	};
	// read next alignment
	static auto aln_read(stream_t* fp, hdr_t* hdr, aln_t* aln)
	{
		return bam_read1(fp, aln); // NOTE: bam does not req. header here
	};
	// get the raw interval of alignment, only converts start and end positions
	static auto aln_to_inteval(aln_t* aln)
	{
		auto pos = aln->core.pos;
		auto len = aln->core.l_qseq;
		return Interval(pos, pos + len);
	};
};


/* reader class of bam/sam praser */


class ReaderBase
{
public:
	ReaderBase(void) noexcept {};
	~ReaderBase(void) noexcept {};
	virtual void open(const char* argv) = 0;
	virtual void close(void) = 0;
	virtual int read(void) = 0;
	virtual Interval get_aln_interval(void) = 0;
	virtual bam_hdr_t* get_header(void) = 0;
	virtual bam1_t* get_alignment(void) = 0;
};


template< typename meth_wrapper >
class _ReaderTemplate : public ReaderBase
{
/* variables */
private:
	std::unique_ptr< typename meth_wrapper::stream_t > _fp;
public:
	std::unique_ptr< typename meth_wrapper::hdr_t > header;
	std::unique_ptr< typename meth_wrapper::aln_t > alignment;

/* methods */
public:
	explicit _ReaderTemplate(void) noexcept
	: _fp(nullptr)
	, header(nullptr)
	, alignment(nullptr)
	{};

	~_ReaderTemplate(void) noexcept {this->close();};
	// not allow to copy since file pointers maintained
	_ReaderTemplate(const _ReaderTemplate&) = delete;
	_ReaderTemplate(_ReaderTemplate&&) = default;

	/* open stream, read header and prepare reader buffer; check if succeeded 
	 * after each step, throw runtime_error upon failure;
	 * dealloc resources upon failure */
	virtual void open(const char* fn)
	{
		using error_t = std::runtime_error;
		// close old opened file
		if (this->_fp) this->close();
		try
		{
			// open stream
			if (this->_fp.reset(meth_wrapper::open(fn)); !this->_fp)
				throw error_t(std::string("failed to open file ") + fn);
			// read header
			if (this->header.reset(meth_wrapper::hdr_read(this->_fp.get()));
				!this->header)
				throw error_t(std::string("failed to read header from ") + fn);
			// allocate reader buffer
			if (this->alignment.reset(meth_wrapper::aln_alloc());
				!this->alignment) throw std::runtime_error("allocation failed");
		}
		catch (error_t& e)
		{
			this->close();
			throw e;
		};
		return;
	};
	/* close and release resources */
	virtual void close(void) noexcept
	{
		meth_wrapper::close(this->_fp.get(), this->header.get(),
			this->alignment.get());
		this->_fp.release();
		this->header.release();
		this->alignment.release();
		return;
	};
	/* read */
	virtual auto read(void) -> int
	{
		return meth_wrapper::aln_read(this->_fp.get(), this->header.get(),
			this->alignment.get());
	};
	/* get interval */
	virtual auto get_aln_interval(void) -> Interval
	{
		return meth_wrapper::aln_to_inteval(this->alignment.get());
	};
	/* get parser pointer */
	virtual auto get_header(void) -> bam_hdr_t*
	{
		return this->header.get();
	};
	virtual auto get_alignment(void) -> bam1_t*
	{
		return this->alignment.get();
	};
};


using SamReader = _ReaderTemplate< _SamReadWrapper >;
using BamReader = _ReaderTemplate< _BamReadWrapper >;


#endif /* __SAM_BAM_READER_H__ */
