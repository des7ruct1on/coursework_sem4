

#ifndef COURSE_WORK_DATA_BASE_COMMAND_DELETE_COLLECTION_H
#define COURSE_WORK_DATA_BASE_COMMAND_DELETE_COLLECTION_H

#include "../database.h"
#include "command.h"
#include <sstream>
#include "../allocators/allocator_types.h"
#include "../allocators/allocator_with_fit_mode.h"
#include "../version_singleton.h"

class command_delete_collection final : public command
{
private:

    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;

public:

    bool can_execute(std::string const& request)
    {
        std::istringstream iss(request);
        std::string instruction;
        iss >> instruction;
        if(instruction == "DELETE_COLLECTION")
        {
            std::string pool_name, scheme_name, collection_name;
            iss >> pool_name >> scheme_name >> collection_name;
            _pool_name = pool_name;
            _scheme_name = scheme_name;
            _collection_name = collection_name;
            return true;
        }
        else return false;


    }

    void execute(std::string const& request)
    {
        version_singleton::get_instance(" ")->write_to_file("DELETE_COLLECTION " + _pool_name + " " + _scheme_name + " " + _collection_name);
        if(data_base::get_instance()->get_mode())
        {
            data_base::get_instance()->remove_collection(_pool_name, _scheme_name, _collection_name);
            std::string catalog_path = _pool_name + "/" + _scheme_name + "/" + _collection_name;
            //std::cout << catalog_path << std::endl;
            if(!std::filesystem::remove_all(catalog_path))
            {
                throw std::logic_error("can`t remove catalog");
            }
        }
        else
        {
            data_base::get_instance()->remove_collection(_pool_name, _scheme_name, _collection_name);
        }
    }
};

#endif //COURSE_WORK_DATA_BASE_COMMAND_DELETE_COLLECTION_H
