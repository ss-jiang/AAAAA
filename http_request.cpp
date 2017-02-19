#include "http_request.h"
#include <vector>
#include <iostream>

// TODO
std::unique_ptr<Request> Request::Parse(const std::string& raw_request) {
	return nullptr;
}

std::string Request::raw_request() const {
	return raw_request;
}
std::string Request::method() const {
	return method;
}
std::string Request::uri() const {
	return uri;
}
std::string Request::version() const {
	return version;
}
using Headers = std::vector<std::pair<std::string, std::string>>;
Headers Request::headers() const {
	return headers;
}
std::string Request::body() const {
	return body;
}
