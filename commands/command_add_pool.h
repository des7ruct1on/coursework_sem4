
#ifndef COURSE_WORK_DATA_BASE_COMMAND_ADD_POOL_H
#define COURSE_WORK_DATA_BASE_COMMAND_ADD_POOL_H
#include "../database.h"
#include "command.h"
#include <sstream>
#include "../version_singleton.h"

class data_base;

class command_add_pool final : public command
{
private:

    std::string _pool_name;

public:

    command_add_pool() = default;

    bool can_execute(std::string const& request)
    {
        std::istringstream iss(request);
        std::string instruction;
        iss >> instruction;
        if(instruction == "ADD_POOL")
        {
            std::string pool_name;
            iss >> pool_name;
            _pool_name = pool_name;
            return true;
        }
        else return false;
    }

    void execute(std::string const& request)
    {
        
        data_base::get_instance()->add_pool(_pool_name);
        
        version_singleton::get_instance(" ")->write_to_file("ADD_POOL " + _pool_name);
        if(data_base::get_instance()->get_mode())
        {
            struct stat buffer;
            if(stat(_pool_name.c_str(), &buffer) != 0)
            {   //creating new catalog
                if(mkdir(_pool_name.c_str(), 0777) != 0)
                {
                    throw std::logic_error("can`t create catalog");

                }
            }
        }
    }
};

#endif //COURSE_WORK_DATA_BASE_COMMAND_ADD_POOL_H
