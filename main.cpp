#include <algorithm>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <set>
#include <exception>
#include <unistd.h>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "program_options.h"
#include "http_server.h"
#include "app_manager.h"

int main( int argc, char* argv[] )
try
{
	program_options options( argc, argv );

	if ( !options.run_as_application() )
	{
		app_manager app( argv[ 0 ] );
		if ( !app.demonize() )
		{
			return 0;
		}
	}

	boost::asio::ip::tcp::endpoint endpoint(
		options.get_ip_appdress()
		, options.get_port() );

	http_server server( endpoint, options.get_doc_directory() );
	server.run();

	return 0;
}
catch( std::exception& error )
{
	std::cout << "error: " << error.what() << std::endl;

	return -1;
}
