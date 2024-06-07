

#ifndef COURSE_WORK_DATA_BASE_COMMAND_ADD_SCHEME_H
#define COURSE_WORK_DATA_BASE_COMMAND_ADD_SCHEME_H
#include "../database.h"
#include "command.h"
#include <sstream>
#include "../version_singleton.h"

class command_add_scheme final : public command
{
private:

    std::string _pool_name;
    std::string _scheme_name;

public:

    bool can_execute(std::string const& request)
    {
        std::istringstream iss(request);
        std::string instruction;
        iss >> instruction;
        if(instruction == "ADD_SCHEME")
        {
            std::string pool_name;
            std::string scheme_name;
            iss >> pool_name >> scheme_name;
            _pool_name = pool_name;
            _scheme_name = scheme_name;
            return true;
        }
        else return false;
    }

    void execute(std::string const& request)
    {
        data_base::get_instance()->add_scheme(_pool_name, _scheme_name);
        version_singleton::get_instance(" ")->write_to_file("ADD_SCHEME " + _pool_name + " " + _scheme_name);
        if(data_base::get_instance()->get_mode())
        {
            std::string catalog_path = _pool_name + "/" + _scheme_name;
            if(mkdir(catalog_path.c_str(), 0777) != 0)
            
            {
                throw std::logic_error("can`t create catalog");

            }

        }
    }
};
#endif //COURSE_WORK_DATA_BASE_COMMAND_ADD_SCHEME_H
