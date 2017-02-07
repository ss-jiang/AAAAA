#include "handler.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

echo_handler::echo_handler(std::vector<char>& request) {
    to_send = request;
}

http_response echo_handler::handle_request() {
  http_response response;
  std::cout << "========= Handling Echo =========" << std::endl;
  
  std::string status = "200 OK";
  response.set_status(status);
  
  std::string length_header = "Content-Length: " + std::to_string(to_send.size());
  response.add_header(length_header);
  std::string type_header = "Content-Type: text/plain";
  response.add_header(type_header);

  response.set_body(to_send);
  return response;
}

static_handler::static_handler(std::string curr_url) {
    url = curr_url;
}

std::string static_handler::get_path_from_url(std::string url) {
  // Assumption: url must be prefixed with "/static/" at this point 
  //             get_function_from_url has already been called on url
  int second_slash_pos = url.find("/", 1);
  // from second slash to end of string is path
  std::string path = url.substr(second_slash_pos, std::string::npos);

  return path;
}

http_response static_handler::handle_request() {
    http_response response;
    std::cout << "========= Handling Static =========" << std::endl;
    
    std::string abs_path = get_exec_path() + "/public" + get_path_from_url(url);
    std::cout << "Serving file from: " << abs_path << std::endl;
    
    if (!file_exists(abs_path)) {
      // TODO: 404 Error
      std::cerr << "Error: " << abs_path << " does not exist" << std::endl;
      return response;
    } 
 
    // save content_type header based on requested file extension
    std::string content_type = get_content_type(abs_path);
    
    // raw byte array
    std::vector<char> to_send = read_file(abs_path);
    
    std::string status = "200 OK"; 
    response.set_status(status);
 
    std::string length_header = "Content-Length: " + std::to_string(to_send.size());
    response.add_header(length_header);
    std::string type_header = "Content-Type: " + content_type;
    response.add_header(type_header); 
 
    response.set_body(to_send); 
    return response;
}

// gets current path of executable
std::string static_handler::get_exec_path() {
    // max path is 2048 characters in file directory
    const int MAX_PATH = 2048;
    char buffer[MAX_PATH];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        return std::string(buffer);
    }    
    else {
        std::cerr << "Error: unable to find current working directory" << std::endl;
    }
    return "";
}

// checks if file exists
bool static_handler::file_exists(std::string filename) {
    struct stat buffer;   
    return (stat(filename.c_str(), &buffer) == 0); 
}

// gets content-type based on the file extension of requested file
std::string static_handler::get_content_type(std::string filename) {
    unsigned int i;
    
    // search for either last period or last slash in filename
    for (i = filename.size() - 1; i >= 0; i--) {
        // file with no extension type
        if (filename[i] == '/') {
            return "text/plain";
        } else if (filename[i] == '.') { // found pos of beginning of extension
            break;
        }
    }

    std::string ext = filename.substr(i + 1, std::string::npos);
    std::string content_type;
    
    // based on ext decide content_type header
    if (ext == "html") {
        content_type = "text/html";
    } else if (ext == "jpg") {
        content_type = "image/jpeg";
    } else if (ext == "pdf") {
        content_type = "application/pdf";
    } else {
        content_type = "text/plain";
    }
    return content_type;
}


// reads raw file into vector of characters
std::vector<char> static_handler::read_file(std::string filename)
{
    std::ifstream ifs(filename, std::ios::binary|std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    std::vector<char>  result(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(&result[0], pos);

    return result;
}
