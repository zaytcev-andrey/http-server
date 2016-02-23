#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <vector>
#include "header.h"

struct request
{
  std::string method;
  std::string uri;
  int http_version_major;
  int http_version_minor;
  std::vector<header> headers;
};

#endif // REQUEST_H
