#ifndef APP_MANAGER_H_
#define APP_MANAGER_H_

#include <boost/noncopyable.hpp>
#include <exception>
#include <string>

#include <signal.h>
#include <unistd.h>
#include <syslog.h>
#include <error.h>

#define MAXFD 64

class app_manager : boost::noncopyable
{
public:

	app_manager( const std::string& app_name )
		: app_name_( app_name )
	{
	}

	bool demonize()
	{
		pid_t pid = -1;
		if ( ( pid = fork() ) == 0 )
		{
			if ( setsid() < 0 )
			{
				throw std::runtime_error( "setsid failed" );
			}

			signal( SIGHUP, SIG_IGN );

			pid = -1;
			if ( ( pid = fork() ) == 0 )
			{
				for ( int i = 0; i < MAXFD; i++ )
				{
					close( i );
				}

				int fd_in = open( "/dev/null", O_RDONLY );
				dup2( fd_in, STDIN_FILENO );
				close( fd_in );
				int fd_out = open( "/dev/null", O_RDWR );
				dup2( fd_out, STDOUT_FILENO );
				close( fd_out );
				int fd_err = open( "/dev/null", O_RDWR );
				dup2( fd_err, STDERR_FILENO );
				close( fd_err );

				openlog( app_name_.c_str(), LOG_PID, 0 );

				return true;
			}

			return false;
		}
		else
		{
			return false;
		}
	}

private:
	std::string app_name_;
};



#endif /* APP_MANAGER_H_ */
