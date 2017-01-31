#ifndef UTILS_DEFINED
#define UTILS_DEFINED

#include "config_parser/config_parser.h"

// server info struct
struct ServerInfo {
    short port;  
};

namespace utils {

    // returns a struct containing all config info nicely parsed
    ServerInfo setup_info_struct(NginxConfig config);

}

#endif 
