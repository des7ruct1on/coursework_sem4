

#ifndef COURSE_WORK_DATA_BASE_COMMAND_FIND_VALUE_H
#define COURSE_WORK_DATA_BASE_COMMAND_FIND_VALUE_H

#include "../database.h"
#include "command.h"
#include <sstream>
#include "../value.h"
#include "../version_singleton.h"

class command_find_value final : public command
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
        if(instruction == "FIND_VALUE")
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
        if(data_base::get_instance()->get_mode())
        {
            std::string file_path = _pool_name + "/" + _scheme_name + "/" + _collection_name + "/" + std::to_string(_id_key) + ".txt";
            Key dk(_id_key);
            const Value* tmp = data_base::get_instance()->obtain_value(_pool_name, _scheme_name, _collection_name, dk);
            const File_value* found_value = static_cast<const File_value*>(tmp);
            Mem_value memory_cache_value;
            deserialization(found_value->_position, found_value->_string_size, &memory_cache_value, file_path);
            data_base::get_instance()->get_logger()->information("FOUND VALUE , key = " + std::to_string(_id_key));
        }
        else
        {
            Key data_key(_id_key);
            const Mem_value* val = reinterpret_cast<const Mem_value*>(data_base::get_instance()->obtain_value(_pool_name, _scheme_name, _collection_name, data_key));
            data_base::get_instance()->get_logger()->information("FOUND VALUE , key = " + std::to_string(_id_key) + ", value = " + std::string(val->_nickname));
        }

        version_singleton::get_instance(" ")->write_to_file("FIND_VALUE " + _pool_name + " " + _scheme_name + " " + _collection_name + " " + std::to_string(_id_key));
    }
};


#endif //COURSE_WORK_DATA_BASE_COMMAND_FIND_VALUE_H
