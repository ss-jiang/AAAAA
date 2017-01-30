#include "utils.h"

namespace utils {

    ServerInfo setup_info_struct(NginxConfig config) {
    
        ServerInfo info;
        info.port = -1;
    
        int num_statements = config.statements_.size();
        
        // look for port num in config
        for (int i = 0; i < num_statements; i++) {
            // if curr statement : port <num>
            if (config.statements_[i]->tokens_[0] == "port" 
                && config.statements_[i]->tokens_.size() == 2) {
                
                // save away port
                info.port = std::atoi(config.statements_[i]->tokens_[1].c_str());
            }
        }
    
        return info;
    }

} // end utils namespace
