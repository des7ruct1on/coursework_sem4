

#ifndef COURSE_WORK_DATA_BASE_COMMAND_ADD_VALUE_H
#define COURSE_WORK_DATA_BASE_COMMAND_ADD_VALUE_H
#include "../database.h"
#include "command.h"
#include <sstream>
#include "../database.h"
#include "../version_singleton.h"
#include "../persistence_singleton.h"

class command_add_value final : public command
{
private:

    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;
    std::string _first_name;
    unsigned int _id_key;
    unsigned int _id_order;
    double _salary;

public:

    bool can_execute(std::string const& request)
    {
        std::istringstream iss(request);
        std::string instruction;
        iss >> instruction;
        if(instruction == "ADD_VALUE")
        {
            std::string pool_name, scheme_name, collection_name, first_name;
            unsigned int id_key, id_order;
            double salary;
            iss >> pool_name >> scheme_name >> collection_name >> id_key >> id_order >> first_name >> salary;
            _pool_name = pool_name;
            _scheme_name = scheme_name;
            _collection_name = collection_name;
            _first_name = first_name;
            _id_key = id_key;
            _id_order = id_order;
            _salary = salary;
            return true;
        } else {
            return false;
        }
    }

    void execute(std::string const& request)
    {
        
        if(data_base::get_instance()->get_mode()) {
            std::string catalog_path = _pool_name + "/" + _scheme_name + "/" + _collection_name + "/" + std::to_string(_id_key) + ".txt";
            std::string file_path = std::to_string(_id_key) + ".txt";
            FILE* value = std::fopen(catalog_path.c_str(), "w");
            if(value)
            {
                std::pair<long, long> information = serialization(catalog_path, _first_name, _id_order, _salary);
                data_base::get_instance()->add_value(_pool_name, _scheme_name, _collection_name, _id_key, information.first, information.second);
                fclose(value);
            }
            else
            {
               throw std::logic_error("Failed open file");
            }

        }
        else
        {
            data_base::get_instance()->add_value(_pool_name, _scheme_name, _collection_name, _id_key, _id_order, _first_name, _salary);
        }

        Key data_key(_id_key);
        const Value* returned_value = data_base::get_instance()->obtain_value(_pool_name, _scheme_name, _collection_name, data_key);
        std::time_t current_time = persistence_singleton::get_instance()->add(const_cast<Value*>(returned_value));
        std::tm* time_info = std::localtime(&current_time);
        char buffer[80];
        std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", time_info);
        version_singleton::get_instance(" ")->write_to_file("ADD_VALUE " + _pool_name + " " + _scheme_name + " " + _collection_name + " " + std::to_string(_id_key) + " " + std::to_string(_id_order) + " " + _first_name + " " + std::to_string(_salary) + " " + buffer);


    }
};

#endif //COURSE_WORK_DATA_BASE_COMMAND_ADD_VALUE_H
