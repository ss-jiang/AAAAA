#include "utils.h"
#include "handler.h"

namespace utils {


ServerInfo setup_info_struct(NginxConfig config) {

    ServerInfo info;
    info.port = -1;

    int num_statements = config.statements_.size();

    // parse away our config file into info struct
    for (int i = 0; i < num_statements; i++) {

        std::string curr_statement = config.statements_[i]->tokens_[0];

        if (curr_statement == "port" && config.statements_[i]->tokens_.size() == 2) {
            info.port = std::atoi(config.statements_[i]->tokens_[1].c_str());
        }
        else if (curr_statement == "path" && config.statements_[i]->tokens_.size() == 3) {

            std::string uri_prefix = config.statements_[i]->tokens_[1];
            std::string handler_name = config.statements_[i]->tokens_[2];

            auto raw_handler_ptr = RequestHandler::CreateByName(handler_name.c_str());
            std::shared_ptr<RequestHandler> handler_ptr (raw_handler_ptr);

            handler_ptr->Init(uri_prefix, *(config.statements_[i]->child_block_));
            info.handler_map[uri_prefix] = std::move(handler_ptr);

        } else if (curr_statement == "default" && config.statements_[i]->tokens_.size() == 2) {
            std::string default_uri = "";
            std::string handler_name = config.statements_[i]->tokens_[1];

            auto raw_handler_ptr = RequestHandler::CreateByName(handler_name.c_str());
            std::shared_ptr<RequestHandler> handler_ptr (raw_handler_ptr);

            handler_ptr->Init(default_uri, *(config.statements_[i]->child_block_));
            info.handler_map[default_uri] = std::move(handler_ptr);
        }
    }
    //such that empty string is mapped to NotFoundHandler
    info.handler_map[""] = std::move(std::shared_ptr<RequestHandler>(new NotFoundHandler));
    return info;
}





} // end utils namespace
