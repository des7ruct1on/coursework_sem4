

#ifndef COURSE_WORK_DATA_BASE_COMMAND_DELETE_POOL_H
#define COURSE_WORK_DATA_BASE_COMMAND_DELETE_POOL_H

#include "../database.h"
#include "command.h"
#include <sstream>
#include "../version_singleton.h"

class data_base;

class command_delete_pool final : public command
{
private:

    std::string _pool_name;

public:

    command_delete_pool() = default;

    bool can_execute(std::string const& request)
    {
        std::istringstream iss(request);
        std::string instruction;
        iss >> instruction;
        if(instruction == "DELETE_POOL")
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
        version_singleton::get_instance(" ")->write_to_file("DELETE_POOL " + _pool_name);
        if(data_base::get_instance()->get_mode())
        {
            data_base::get_instance()->remove_pool(_pool_name);
            std::string catalog_path = _pool_name;
            //std::cout << catalog_path << std::endl;
            if(!std::filesystem::remove_all(catalog_path))
            {
                throw std::logic_error("can`t remove catalog");
            }
        }
        else
        {
            data_base::get_instance()->remove_pool(_pool_name);
        }
    }
};

#endif //COURSE_WORK_DATA_BASE_COMMAND_DELETE_POOL_H
