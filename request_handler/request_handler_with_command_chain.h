

#ifndef COURSE_WORK_DATA_BASE_REQUEST_HANDLER_WITH_COMMAND_CHAIN_H
#define COURSE_WORK_DATA_BASE_REQUEST_HANDLER_WITH_COMMAND_CHAIN_H
#include "handler.h"
#include <iostream>
#include "../commands/command.h"
#include "request_handler_with_command.h"
class request_handler_with_command_chain
{
private:

    handler *_first_handler;
    handler *_last_handler;
public:

    request_handler_with_command_chain() : _first_handler(nullptr), _last_handler(nullptr)
    {

    }
    ~request_handler_with_command_chain()
    {
        while(_first_handler != nullptr)
        {
            _last_handler = _first_handler;
            _first_handler = _first_handler->_next_handler;
            delete _last_handler;
        }
    }

    request_handler_with_command_chain(request_handler_with_command_chain const&) = delete;
    request_handler_with_command_chain(request_handler_with_command_chain&&) = delete;
    request_handler_with_command_chain& operator=(request_handler_with_command_chain const&) = delete;
    request_handler_with_command_chain& operator=(request_handler_with_command_chain&&) = delete;

public:

    bool handle(std::string const& request) const
    {
        if(_first_handler == nullptr)
        {
            return false;
        }

        return _first_handler->handle(request);
    }

public:

    request_handler_with_command_chain& add_handler(command* cmd)
    {
        _last_handler = _first_handler == nullptr ? _first_handler = new request_handler_with_command(cmd) : _last_handler->_next_handler = new request_handler_with_command(cmd);
        return *this;
    }

};

#endif //COURSE_WORK_DATA_BASE_REQUEST_HANDLER_WITH_COMMAND_CHAIN_H
