#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include <boost/logic/tribool.hpp>

#include "request.h"

class request_parser
{
public:
	boost::tribool parse(
		request& req
		, const std::vector< char >& data
		, size_t length );
	
private:
	boost::tribool handle_input_char(
		request& req
		, char input );

private:
	enum state
	{
		method_start,
		method,
		uri,
		http_version_h,
		http_version_t_1,
		http_version_t_2,
		http_version_p,
		http_version_slash,
		http_version_major_start,
		http_version_major,
		http_version_minor_start,
		http_version_minor,
		expecting_newline_1,
		header_line_start,
		header_lws,
		header_name,
		space_before_header_value,
		header_value,
		expecting_newline_2,
		expecting_newline_3
	} state_;
};

#endif // REQUEST_PARSER_H
