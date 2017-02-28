#ifndef RESPONSE_PARSER_H
#define RESPONSE_PARSER_H

#include <string>

class ResponseParser {
public:
	enum ParseStatus {
		OK = 0,
		NO_STATUS = 1,
		NO_CONTENT_LENGTH = 2
	};

	ResponseParser();
	
	ParseStatus Parse(std::string response);

	int getStatus() { return m_status_code; }
	int getContentLength() { return m_content_length; }
	std::string getContentType() { return m_content_type; }
	std::string getRedirectPath() { return m_redirect_path; }

private:
	bool is_digits(const std::string &str);
	size_t find_line_ending(std::string input_string);

	int m_status_code;
	int m_content_length;
	std::string m_content_type;
	std::string m_redirect_path;
};

#endif