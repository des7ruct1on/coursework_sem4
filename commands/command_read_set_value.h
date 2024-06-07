

#ifndef COURSE_WORK_DATA_BASE_COMMAND_READ_SET_VALUE_H
#define COURSE_WORK_DATA_BASE_COMMAND_READ_SET_VALUE_H

#include "../database.h"
#include "command.h"
//#include "../logger/logger_singleton.h"
#include <sstream>
#include "../value.h"
#include "../version_singleton.h"

class command_read_set_value final : public command
{
private:

    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;
    unsigned int _id_min;
    unsigned int _id_max;
    bool _is_inclusive_lower;
    bool _is_inclusive_upper;

public:

    bool can_execute(std::string const& request)
    {
        std::istringstream iss(request);
        std::string instruction;
        iss >> instruction;
        if(instruction == "READ_SET")
        {
            std::string pool_name, scheme_name, collection_name;
            unsigned int id_min, id_max;
            bool is_inclusive_lower, is_inclusive_upper;
            iss >> pool_name >> scheme_name >> collection_name >> id_min >> id_max >> is_inclusive_lower >> is_inclusive_upper;
            _pool_name = pool_name;
            _scheme_name = scheme_name;
            _collection_name = collection_name;
            _id_max = id_max;
            _id_min = id_min;
            _is_inclusive_lower = is_inclusive_lower;
            _is_inclusive_upper = is_inclusive_upper;
            return true;
        }
        else return false;
    }

    void execute(std::string const& request)
    {
        version_singleton::get_instance(" ")->write_to_file("READ_SET " + _pool_name + " " + _scheme_name + " " + _collection_name + " " + std::to_string(_id_min) + " " + std::to_string(_id_max) + " " + std::to_string(_is_inclusive_lower) + " " + std::to_string(_is_inclusive_upper));
        if(data_base::get_instance()->get_mode())
        {
            Key data_key_min(_id_min);
            Key data_key_max(_id_max);

            std::vector<Value*> vector = data_base::get_instance()->obtain_between_value(_pool_name, _scheme_name, _collection_name, data_key_min, data_key_max, _is_inclusive_lower, _is_inclusive_upper);
           
            for(Value* each : vector)
            {
                for(unsigned int i = _id_min; i < _id_max; ++i)
                {
                    File_value* val = reinterpret_cast<File_value*>(each);
                    Mem_value value_memory_cache;
                    
                    std::string file_path = _pool_name + "/" + _scheme_name + "/" + _collection_name + "/" + std::to_string(i) + ".txt";
                    deserialization(val->_position, val->_string_size, value_memory_cache, file_path);
                    data_base::get_instance()->get_logger()->information("id = " + std::to_string(value_memory_cache._id));
                }
            }
        }
        else
        {
            Key data_key_min(_id_min);
            Key data_key_max(_id_max);

            std::vector<Value*> vector = data_base::get_instance()->obtain_between_value(_pool_name, _scheme_name, _collection_name, data_key_min, data_key_max, _is_inclusive_lower, _is_inclusive_upper);
            for(Value* each : vector)
            {
                Mem_value* val = reinterpret_cast<Mem_value*>(each);
                data_base::get_instance()->get_logger()->information("id = " + std::to_string(val->_id) + ", value = " + std::string(val->_nickname));

            }
        }

    }
};

#endif //COURSE_WORK_DATA_BASE_COMMAND_READ_SET_VALUE_H
