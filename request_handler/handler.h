

#ifndef COURSE_WORK_DATA_BASE_HANDLER_H
#define COURSE_WORK_DATA_BASE_HANDLER_H
#include <iostream>

class handler
{
    friend class request_handler_with_command_chain;

public:

    handler* _next_handler;

public:

    handler() : _next_handler(nullptr)
    {

    }

    virtual ~handler() = default;

public:

    virtual bool handle(std::string const &request) = 0;
};


#endif //COURSE_WORK_DATA_BASE_HANDLER_H
