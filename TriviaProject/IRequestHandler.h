#pragma once
#include <memory>
#include <vector>
#include <ctime>

struct RequestResult;
struct RequestInfo;

class IRequestHandler
{
public:
    virtual ~IRequestHandler() = default;
    virtual bool isRequestRelevant(const RequestInfo& req) const = 0;
    virtual RequestResult handleRequest(const RequestInfo& req) = 0;
};

struct RequestResult
{
    std::vector<unsigned char> response;
    std::unique_ptr<IRequestHandler> newHandler;
};

struct RequestInfo
{
    unsigned int RequestId;
    std::time_t receivalTime;
    std::vector<unsigned char> buffer;
};


