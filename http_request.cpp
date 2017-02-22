#include "http_request.h"
#include <vector>
#include <iostream>

// TODO: add header parsing
std::unique_ptr<Request> Request::Parse(const std::string& raw_request) {
	std::unique_ptr<Request> req(new Request);
	req->m_raw_request = raw_request;

	int spaces = 0;

	std::string method;
	std::string url;
	std::string version;

	for (unsigned int i=0; i < raw_request.size(); i++)
	{
		if (raw_request[i] == '\r')
			break;

		if (raw_request[i] == ' ') {
		  if (i == 0)
		    req->m_error = 400;
		  if (raw_request[i-1] == ' ')
		    req->m_error = 400;
		     spaces++;
		     continue;
		}

		switch(spaces)
		{
		case 0:
			method += raw_request[i];
			break;
		case 1:
			url += raw_request[i];
			break;
		case 2:
			version += raw_request[i];
			break;
		}
	}

	if (method != "GET" && method != "POST") {
			std::cerr << "Invalid method in request line!" << std::endl;
			req->m_error = 501;
	}

	if (version != "HTTP/1.0" && version != "HTTP/1.1") {
		std::cerr << "Unsupported version in request line!" << std::endl;
		req->m_error = 505;
	}
	if (url.size() < 1)
	  req->m_error = 400;

	req->m_method = method;
	req->m_uri = url;
	req->m_version = version;
	return req;
}

std::string Request::raw_request() const {
	return m_raw_request;
}
std::string Request::method() const {
	return m_method;
}
std::string Request::uri() const {
	return m_uri;
}

//this method allows the uri to be set to the path for static file serving
void Request::setUri(const std::string& new_uri){
	m_uri = new_uri;
}

std::string Request::version() const {
	return m_version;
}
int Request::error() const {
	return m_error;
}
using Headers = std::vector<std::pair<std::string, std::string>>;
Headers Request::headers() const {
	return m_headers;
}
std::string Request::body() const {
	return m_body;
}
