#include "handler.h"
#include "http_response.h"
#include "http_request.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>


// Dynamic handler creation code
std::map<std::string, RequestHandler* (*)(void)>* request_handler_builders = nullptr;

RequestHandler* RequestHandler::CreateByName(const char* type) {
  const auto type_and_builder = request_handler_builders->find(type);
  if (type_and_builder == request_handler_builders->end()) {
    return nullptr;
  }
  return (*type_and_builder->second)();
}

//////////////////////////////////////////////////////////////////////////////////////
////////////////////////////  Not Found Handler  /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

RequestHandler::Status NotFoundHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    return RequestHandler::PASS;
}

RequestHandler::Status NotFoundHandler::HandleRequest(const Request& request, Response* response){
    std::cout << "\nNotFoundHandler::HandleRequest" << std::endl;

    std::string body = "404 NOT FOUND";
    response->SetStatus(Response::NOT_FOUND);
    response->AddHeader("Content-Length", std::to_string(body.size()));
    response->AddHeader("Content-Type", "text/plain");
    response->SetBody(body);
    return RequestHandler::PASS;
}

//////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Echo Handler /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

RequestHandler::Status EchoHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    return RequestHandler::PASS;
}

RequestHandler::Status EchoHandler::HandleRequest(const Request& request, Response* response) {
    std::cout << "\nEchoHandler::HandleRequest" << std::endl;

    response->SetStatus(Response::OK);
    response->AddHeader("Content-Length", std::to_string(request.raw_request().size() - 4));
    response->AddHeader("Content-Type", "text/plain");
    response->SetBody(request.raw_request());
    return RequestHandler::PASS;
}

//////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Static Handler /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

// Sets dir_from_config to directory specified after root in config
RequestHandler::Status StaticHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    for (unsigned int i = 0; i < config.statements_.size(); i++){
        // get the root from the child block
        if (config.statements_[i]->tokens_[0] == "root" && config.statements_[i]->tokens_.size() == 2){
            serve_path = config.statements_[i]->tokens_[1];
        }
    }

    this->uri_prefix = uri_prefix;

    //Code to add the / between the file path and serve_path properly
    if (serve_path.length() != 0 && serve_path[serve_path.length()-1] != '/'){
        if (uri_prefix.length() != 0 && uri_prefix[uri_prefix.length()-1] == '/'){
            serve_path += '/';
        }
    }else{
        if (uri_prefix.length() != 0 && uri_prefix[uri_prefix.length()-1] != '/'){
            if (serve_path.length() != 0){
                serve_path = serve_path.substr(0, serve_path.length() - 1);
            }
        }
    }

    return RequestHandler::PASS;
}

// Attempts to serve file that was requested by the request object
// If not possible, we return fail signal for outside class to handle
// calling not found handler
RequestHandler::Status StaticHandler::HandleRequest(const Request& request, Response* response) {
    std::cout << "\nStaticHandler::HandleRequest" << std::endl;

    std::string abs_path = serve_path;

    //get the file name/path from the request url
    std::string file_path = request.uri().substr(uri_prefix.length());
    abs_path += file_path;

    if(!is_regular_file(abs_path.c_str())) {
      std::cerr << "Error: " << abs_path << " does not exist" << std::endl;
      return RequestHandler::FAIL;
    }

    // save content_type header based on requested file extension
    std::string content_type = get_content_type(abs_path);

    // raw byte array
    std::string to_send = read_file(abs_path);
    std::cout << "Serving file from: " << abs_path << std::endl;

    response->SetStatus(Response::OK);
    response->AddHeader("Content-Length", std::to_string(to_send.size()));
    response->AddHeader("Content-Type", content_type);
    response->SetBody(to_send);
    return RequestHandler::PASS;
}

// checks if file exists and is regular file
bool StaticHandler::is_regular_file(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
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
std::string StaticHandler::read_file(std::string filename) {
    std::ifstream ifs(filename, std::ios::binary|std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    std::vector<char> result(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(&result[0], pos);

    std::string str_rep(result.begin(), result.end());

    return str_rep;
}


//////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Status Handler /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


RequestHandler::Status StatusHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    return RequestHandler::PASS;
}

RequestHandler::Status StatusHandler::addHandledRequest(std::string url, int c) {
    map_of_request_and_responses[url].push_back(c);
    return RequestHandler::PASS;
}

RequestHandler::Status StatusHandler::addNameToHandlerMap(std::map<std::string, std::string> m) {
    map_of_uri_to_handler = m;
    return RequestHandler::PASS;
}

RequestHandler::Status StatusHandler::HandleRequest(const Request& request, Response* response) {
    std::cout << "\nStatusHandler::HandleRequest" << std::endl;

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


