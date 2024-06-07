

#ifndef COURSE_WORK_DATA_BASE_COMMAND_FIND_VALUE_IN_TIME_H
#define COURSE_WORK_DATA_BASE_COMMAND_FIND_VALUE_IN_TIME_H

#include "../database.h"
#include "command.h"
#include <sstream>
#include "../value.h"
#include "../version_singleton.h"
#include <ctime>
#include <iomanip>
#include "../persistence_singleton.h"

class command_find_value_in_time final : public command
{
private:

    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;
    unsigned int _id_key;
    std::time_t _time;


public:

    bool can_execute(std::string const& request)

    {
        std::cout << std::endl <<"REQUEST " << request << std::endl << std::endl;
        std::istringstream iss(request);
        std::string instruction;
        iss >> instruction;
        if(instruction == "IN_TIME")
        {

            std::string pool_name, scheme_name, collection_name, time_str1, time_str2;
            unsigned int id_key;
            iss >> pool_name >> scheme_name >> collection_name >> id_key >> time_str1 >> time_str2;
            time_str1 += " ";
            time_str1 += time_str2;
            std::cout << "TIME\n" << time_str1 << "\n";
            _pool_name = pool_name;
            _scheme_name = scheme_name;
            _collection_name = collection_name;
            _id_key = id_key;
            tm tm = {};
            std::istringstream ss(time_str1);
            ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
            std::time_t t = std::mktime(&tm);
            _time = t;
            return true;
        }
        else return false;
    }

    void execute(std::string const& request)
    {
        std::cout << std::endl << request << std::endl << std::endl;
        if(data_base::get_instance()->get_mode())
        {
            std::string file_path = _pool_name + "/" + _scheme_name + "/" + _collection_name + "/" + std::to_string(_id_key) + ".txt";
            Key dk(_id_key);
            const Value* tmp = data_base::get_instance()->obtain_value(_pool_name, _scheme_name, _collection_name, dk);
            const File_value* found_value = static_cast<const File_value*>(tmp);
            Mem_value memory_cache_value;
            deserialization(found_value->_position, found_value->_string_size, memory_cache_value, file_path);
            data_base::get_instance()->get_logger()->information("FOUND VALUE , key = " + std::to_string(_id_key));
        } else {
            Key data_key(_id_key);
            const Mem_value* val = reinterpret_cast<const Mem_value*>(data_base::get_instance()->obtain_value(_pool_name, _scheme_name, _collection_name, data_key));
            persistence_singleton* pers = persistence_singleton::get_instance();
            Mem_value* mval = reinterpret_cast<Mem_value *>(pers->get_on_time(_time));
            data_base::get_instance()->get_logger()->information("FOUND VALUE , key = " + std::to_string(_id_key) + ", value = " + std::string(val->_nickname));
        }

        version_singleton::get_instance(" ")->write_to_file("IN_TIME " + _pool_name + " " + _scheme_name + " " + _collection_name + " " + std::to_string(_id_key));
    }
};


#endif //COURSE_WORK_DATA_BASE_COMMAND_FIND_VALUE_IN_TIME_H
