#ifndef REQUEST_HANDLER_H_
#define REQUEST_HANDLER_H_

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>

struct reply;
struct request;

class request_handler : boost::noncopyable
{
public:
  explicit request_handler( const std::string& doc_root );

  void handle_request( const request& req, reply& rep );

private:
  boost::filesystem::path doc_root_;
};

#endif /* REQUEST_HANDLER_H_ */
