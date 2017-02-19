#include "http_request.h"
#include <vector>
#include <iostream>

// TODO
std::unique_ptr<Request> Request::Parse(const std::string& raw_request) {
	return nullptr;
}
// TODO
std::string Request::raw_request() const {
	return nullptr;
}
// TODO
std::string Request::method() const {
	return nullptr;
}
// TODO
std::string Request::uri() const {
	return nullptr;
}
// TODO
std::string Request::version() const {
	return nullptr;
}
// TODO
using Headers = std::vector<std::pair<std::string, std::string>>;
Headers Request::headers() const {
	Headers headers;
	return headers;
}
// TODO
std::string Request::body() const {
	return nullptr;
}
