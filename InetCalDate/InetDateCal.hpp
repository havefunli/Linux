#pragma once

#include "Date.h"
#include "Protocol.hpp"

#include <memory>

class InetCalDate
{
public:
    std::shared_ptr<Response> Calculate(std::shared_ptr<Request> req)
    {
        int result = 0;
        result = req->GetLeft() - req->GetRight();

        return std::make_shared<Response>(result);
    }
};