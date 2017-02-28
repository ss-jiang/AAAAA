#include "response_parser.h"
#include <iostream>

ResponseParser::ResponseParser() {
	m_redirect_path = "";
	m_content_type = "";
}

ResponseParser::ParseStatus ResponseParser::Parse(std::string response) {
	//std::cout << response << std::endl;
	auto first_space = response.find(" ");
	std::string rest_of_first_line = response.substr(first_space + 1);

	// Find the response code
	auto second_space = rest_of_first_line.find(" ");
	std::string status_code = rest_of_first_line.substr(0, second_space);

	// Status code should be all digits
	if (!status_code.empty() && is_digits(status_code))
		m_status_code = std::stoi(status_code);
	else 
		return ResponseParser::ParseStatus::NO_STATUS;

	// Find content-length
	std::string content_length_ = "Content-Length: ";
	std::string content_type_ = "Content-Type: ";
	std::string location_ = "Location: ";
	auto c_length_start = response.find(content_length_);

	// If could not find the content length
	if (c_length_start == std::string::npos) {
		m_content_length = 0;
	}
	else {
		std::string rest_of_cl = response.substr(c_length_start + content_length_.length());
		size_t c_length_end = find_line_ending(rest_of_cl); // Finds the end of the content length string
		rest_of_cl = rest_of_cl.substr(0, c_length_end);

		if (rest_of_cl == "")
			m_content_length = 0;
		else if (is_digits(rest_of_cl))
			m_content_length = std::stoi(rest_of_cl);
		else
			return ResponseParser::ParseStatus::NO_CONTENT_LENGTH;
	}

	if (m_status_code == 302) {
		// Find location for redirect
		auto location_start = response.find(location_);

		if (location_start == std::string::npos)
			m_redirect_path = "";
		else {
			std::string rest_of_location = response.substr(location_start + location_.length());
			size_t location_end = find_line_ending(rest_of_location);

			m_redirect_path = rest_of_location.substr(0, location_end);
		}
	}
	else {
		// Find content type of the file
		auto type_start = response.find(content_type_);

		if (type_start == std::string::npos)
			m_content_type = "";
		else {
			std::string rest_of_type = response.substr(type_start + content_type_.length());
			size_t type_end = find_line_ending(rest_of_type);

			m_content_type = rest_of_type.substr(0, type_end);
		}
	}

	return ResponseParser::ParseStatus::OK;	
}

// Function that determines whether every element of a string is a digit
bool ResponseParser::is_digits(const std::string &str) 
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}

// Looks for the end of a line in the response using \r
size_t ResponseParser::find_line_ending(std::string input_string)
{
	for (size_t i = 0; i < input_string.length(); i++)
		if (input_string[i] == '\r')
			return i;

	return -1;
}