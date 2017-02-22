#include "handler.h"
#include "http_response.h"
#include "http_request.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

RequestHandler::Status NotFoundHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    return RequestHandler::PASS;
}

RequestHandler::Status NotFoundHandler::HandleRequest(const Request& request, Response* response){
    response->SetStatus(Response::NOT_FOUND);
    response->AddHeader("Content-Length", std::to_string(request.raw_request().size() - 4));
    response->AddHeader("Content-Type", "text/plain");
    response->SetBody("404 NOT FOUND");
    return RequestHandler::PASS; //should this be FAIL actually?
}

RequestHandler::Status EchoHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    return RequestHandler::PASS;
}

RequestHandler::Status EchoHandler::HandleRequest(const Request& request, Response* response) {
    std::cout << "In EchoHandler" << std::endl;
    response->SetStatus(Response::OK);
    response->AddHeader("Content-Length", std::to_string(request.raw_request().size() - 4));
    response->AddHeader("Content-Type", "text/plain");
    response->SetBody(request.raw_request());

    return RequestHandler::PASS;
}

// Sets dir_from_config to directory specified after root in config
RequestHandler::Status StaticHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    for (unsigned int i = 0; i < config.statements_.size(); i++){
      if (config.statements_[i]->tokens_[0] == "root"){
        dir_from_config = config.statements_[i]->tokens_[1];
      }
    }
    return RequestHandler::PASS;
}


RequestHandler::Status StaticHandler::HandleRequest(const Request& request, Response* response) {
    std::cout << "========= Handling Static =========" << std::endl;
    url = request.uri();
    std::string abs_path = get_exec_path() + dir_from_config + "/" + url;
    std::cout << "Serving file from: " << abs_path << std::endl;
    if (!file_exists(abs_path)) {
      // TODO: 404 Error
      std::cerr << "Error: " << abs_path << " does not exist" << std::endl;
      return RequestHandler::FAIL;
    }
    // save content_type header based on requested file extension
    std::string content_type = get_content_type(abs_path);
    // raw byte array
    std::vector<char> to_send = read_file(abs_path);
    response->SetStatus(Response::OK);
    response->AddHeader("Content-Length", std::to_string(to_send.size()));
    response->AddHeader("Content-Type", content_type);
    std::string to_send_string(to_send.begin(), to_send.end());
    response->SetBody(to_send_string);
    return RequestHandler::PASS;
}



// gets current path of executable
std::string StaticHandler::get_exec_path() {
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
bool StaticHandler::file_exists(std::string filename) {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

// gets content-type based on the file extension of requested file
std::string StaticHandler::get_content_type(std::string filename) {
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
std::vector<char> StaticHandler::read_file(std::string filename)
{
    std::ifstream ifs(filename, std::ios::binary|std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    std::vector<char>  result(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(&result[0], pos);

    return result;
}


RequestHandler::Status StatusHandler::Init(const std::string& uri_prefix, const NginxConfig& config)
{
    return RequestHandler::PASS;
}

RequestHandler::Status StatusHandler::addHandledRequest(std::string url, int c)
{
    map_of_request_and_responses[url].push_back(c);
    return RequestHandler::PASS;
}

RequestHandler::Status StatusHandler::addNameToHandlerMap(std::map<std::string, std::string> m)
{
    map_of_uri_to_handler = m;
    return RequestHandler::PASS;
}

RequestHandler::Status StatusHandler::HandleRequest(const Request& request, Response* response)
{
    std::cout << "Handling Status Request" << std::endl;

    std::string to_send;

    to_send += "These are the handlers available and their URIs:\n";
    for (auto const& x : map_of_uri_to_handler){
        to_send += x.first;
        to_send += " -> ";
        to_send += x.second;
        to_send += "\n";
    }  

    to_send += "\nThese are the requests received and their corresponding response codes\n";
    for (auto const& x : map_of_request_and_responses){
        for(auto const& c: x.second){
            to_send += x.first;
            to_send += " -> ";
            to_send += std::to_string(c);
            to_send += "\n";
        }

    }    
    response->SetStatus(Response::OK);
    response->AddHeader("Content-Length", std::to_string(to_send.length()));
    response->AddHeader("Content-Type", "text/plain");
    response->SetBody(to_send);

    return RequestHandler::PASS;
}


