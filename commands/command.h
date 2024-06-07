
#ifndef COURSE_WORK_DATA_BASE_COMMAND_H
#define COURSE_WORK_DATA_BASE_COMMAND_H
#include <iostream>
#include "../version_singleton.h"

class command
{
public:
    
    virtual ~command() = default;

public:

    virtual bool can_execute(std::string const&) = 0;
    virtual void execute(std::string const&) = 0;
};

#endif //COURSE_WORK_DATA_BASE_COMMAND_H
