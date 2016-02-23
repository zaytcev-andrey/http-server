#ifndef CONNECTION_H
#define CONNECTION_H

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include <vector>

#include "request_parser.h"
#include "request_handler.h"
#include "request.h"
#include "reply.h"

class connection
	: public boost::enable_shared_from_this<connection>
	, private boost::noncopyable
{
public:
	typedef boost::shared_ptr< connection > ptr;

public:
	connection(
		boost::asio::io_service& io_service
		, request_handler& req_handler )
		: connected_socket_( io_service )
		, read_buffer_( buffer_len )
		, write_buffer_( buffer_len )
		, request_parser_()
		, request_handler_( req_handler )
	{
		std::cout << "connection constructed" << std::endl;
	}

	~connection()
	{
		std::cout << "connection destroyed" << std::endl;
	}

	void start()
	{
		do_read();
	}

	void stop()
	{
		connected_socket_.close();
	}

	boost::asio::ip::tcp::socket& connected_socket()
	{
		return connected_socket_;
	}

private:
	void do_read()
	{
		connected_socket_.async_receive(
			boost::asio::buffer( read_buffer_.data(), read_buffer_.size() )
			, boost::bind(
					&connection::handle_read, shared_from_this()
					, boost::asio::placeholders::error
					, boost::asio::placeholders::bytes_transferred ) );
	}

	void handle_read( const boost::system::error_code& err, size_t bytes_transferred )
	{
		std::cout << "received:" << std::endl;
		std::cout.write( read_buffer_.data(), bytes_transferred ) << std::endl;

		if ( !err )
		{
			boost::tribool res = request_parser_.parse(
				request_
				, read_buffer_
				, bytes_transferred );

			if ( res )
			{
				request_handler_.handle_request( request_, reply_ );

				do_write();
			}
			else if ( !res )
			{
				reply_ = reply::stock_reply(reply::bad_request);

				do_write();
			}
			else // need some data
			{
				do_read();
			}
		}
		else
		{
			stop();

		}
	}

	void do_write()
	{
		async_write(
			connected_socket_
			, reply_.to_buffers()
			, boost::bind(
				&connection::handle_write, shared_from_this()
				, boost::asio::placeholders::error ) );
	}

	void handle_write( const boost::system::error_code& err )
	{
		if ( !err )
		{
			// graceful connection closure.
			boost::system::error_code non_err_code;
			connected_socket_.shutdown(
				boost::asio::ip::tcp::socket::shutdown_both
				, non_err_code );
		}

		if ( err != boost::asio::error::operation_aborted )
		{
			stop();
		}
	}

private:
	enum { buffer_len = 8192 };
	boost::asio::ip::tcp::socket connected_socket_;
	std::vector< char > read_buffer_;
	std::vector< char > write_buffer_;
	request_parser request_parser_;
	request_handler& request_handler_;
	request request_;
	reply reply_;
};

#endif // CONNECTION_H
