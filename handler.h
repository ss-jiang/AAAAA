#ifndef HANDLER_DEFINED
#define HANDLER_DEFINED

class handler {
public:
    // pure virtual function for handling a request
    void handle_request() = 0;
}

class echo_handler : public handler {
public:
    void handle_request();
}

class static_handler : public handler {
public:
    void handle_request();

}




#endif
