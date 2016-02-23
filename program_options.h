#ifndef PROGRAM_OPTIONS_H_
#define PROGRAM_OPTIONS_H_

#include <string>
#include <exception>
#include <sstream>
#include <boost/asio/ip/address.hpp>

class program_options
{
public:
	program_options()
		: ip_address_()
		, port_()
	    , doc_directory_()
		, run_as_application_()
	{
	}

	program_options( int argc, char* argv[] )
	{
		if ( ! ( argc == 7 || argc == 8 ) )
		{
			throw_exception( argv[ 0 ] );
		}

		opterr = 0;
		int opt = -1;
		while ( ( opt = getopt( argc, argv, "h:p:d:a::" ) ) != -1)
		{
			switch ( opt )
			{
			case 'h':
				ip_address_ = optarg;
				break;
			case 'p':
				port_ = optarg;
				break;
			case 'd':
				doc_directory_ = optarg;
				break;
			case 'a':
				run_as_application_ = true;
				break;
			default: /* '?' */
				throw_exception( argv[ 0 ] );
			}
		}
	}

	boost::asio::ip::address get_ip_appdress() const
	{
		return boost::asio::ip::address::from_string( ip_address_ );
	}

	unsigned short get_port() const
	{
		unsigned short port = 0;
		std::stringstream sstream;
		sstream << port_;
		sstream >> port;

		return port;
	}

	std::string get_doc_directory() const
	{
		return doc_directory_;
	}

	bool run_as_application() const
	{
		return run_as_application_;
	}

private:
	void throw_exception( char* prog_name )
	{
		std::stringstream sstream;
		sstream << "Usage: " << prog_name << "-h <ip> -p <port> -d <directory>";
		std::string usage;
		std::getline( sstream, usage );
		throw std::invalid_argument( usage );
	}

private:
	std::string ip_address_;
	std::string port_;
	std::string doc_directory_;
	bool run_as_application_;
};

#endif // PROGRAM_OPTIONS_H_
