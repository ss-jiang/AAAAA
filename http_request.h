#ifndef Http_Req
#define Http_Req

#include <string>
#include <vector>
#include <memory>

// Represents an HTTP Request.
//
// Usage:
//   auto request = Request::Parse(raw_request);
class Request
{
  public:
    static std::unique_ptr<Request> Parse(const std::string& raw_request);

    std::string raw_request() const;
    std::string method() const;
    std::string uri() const;
    std::string version() const;

    using Headers = std::vector<std::pair<std::string, std::string>>;
    Headers headers() const;

    std::string body() const;

  private:
    std::string m_raw_request;
    std::string m_method;
    std::string m_uri;
    std::string m_version;
    std::string m_body;
    Headers m_headers;
};


#endif
