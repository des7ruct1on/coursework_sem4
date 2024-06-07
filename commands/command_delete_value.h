

#ifndef COURSE_WORK_DATA_BASE_COMMAND_DELETE_VALUE_H
#define COURSE_WORK_DATA_BASE_COMMAND_DELETE_VALUE_H

#include "../database.h"
#include "command.h"
#include <sstream>
#include "../value.h"
#include "../version_singleton.h"

class command_delete_value final : public command
{
private:

    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;
    unsigned int _id_key;

public:

    bool can_execute(std::string const& request)
    {
        std::istringstream iss(request);
        std::string instruction;
        iss >> instruction;
        if(instruction == "DELETE_VALUE")
        {

            std::string pool_name, scheme_name, collection_name;
            unsigned int id_key;
            iss >> pool_name >> scheme_name >> collection_name >> id_key;
            _pool_name = pool_name;
            _scheme_name = scheme_name;
            _collection_name = collection_name;
            _id_key = id_key;
            return true;
        }
        else return false;
    }

    void execute(std::string const& request)
    {
        version_singleton::get_instance(" ")->write_to_file("DELETE_VALUE " + _pool_name + " "  + _scheme_name + " " + _collection_name +  " " + std::to_string(_id_key));
        if(data_base::get_instance()->get_mode())
        {
            Key data_key(_id_key);
            data_base::get_instance()->remove_value(_pool_name, _scheme_name, _collection_name, data_key);
            std::string catalog_path = _pool_name + "/" + _scheme_name + "/" + _collection_name + "/"  + std::to_string(_id_key) + ".txt";
            //std::cout << catalog_path << std::endl << std::endl;
            if(!std::filesystem::remove_all(catalog_path))
            {
                throw std::logic_error("cant remove a catalog");
            }

        }
        else
        {
            Key data_key(_id_key);
            data_base::get_instance()->remove_value(_pool_name, _scheme_name, _collection_name, data_key);
        }

    }
};


#endif //COURSE_WORK_DATA_BASE_COMMAND_DELETE_VALUE_H
