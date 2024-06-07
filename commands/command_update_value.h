
#ifndef COURSE_WORK_DATA_BASE_COMMAND_UPDATE_VALUE_H
#define COURSE_WORK_DATA_BASE_COMMAND_UPDATE_VALUE_H

#include "../database.h"
#include "command.h"
#include <sstream>
#include "../version_singleton.h"

class command_update_value final : public command
{
private:

    std::string _pool_name;
    std::string _scheme_name;
    std::string _collection_name;
    std::string _first_name;
    unsigned int _id_key;
    unsigned int _id_order;
    double _rating;

public:

    bool can_execute(std::string const& request)
    {
        std::istringstream iss(request);
        std::string instruction;
        iss >> instruction;
        if(instruction == "UPDATE_VALUE")
        {
            std::string pool_name, scheme_name, collection_name, first_name;
            unsigned int id_key, id_order;
            double rating;
            iss >> pool_name >> scheme_name >> collection_name >> id_key >> id_order >> first_name >> rating;
            _pool_name = pool_name;
            _scheme_name = scheme_name;
            _collection_name = collection_name;
            _first_name = first_name;
            _id_key = id_key;
            _id_order = id_order;
            _rating = rating;
            return true;
        }
        else return false;
    }

    void execute(std::string const& request)
    {
        version_singleton::get_instance(" ")->write_to_file("UPDATE_VALUE " + _pool_name + " " + _scheme_name + " " + _collection_name + " " + std::to_string(_id_key) + " " + std::to_string(_id_order) + " " + _first_name + " " + std::to_string(_rating));
        if(data_base::get_instance()->get_mode())
        {
            std::string catalog_path = _pool_name + "/" + _scheme_name + "/" + _collection_name + "/"  + std::to_string(_id_key) + ".txt";
            if(remove(catalog_path.c_str()) != 0)
            {
                throw std::logic_error("cant remove file");

            }
            FILE* value = std::fopen(catalog_path.c_str(), "w");
            if(value)
            {
                //logger_singleton::get_instance()->get_logger()->trace("file: " + catalog_path + " created");
                std::pair<long, long> information = serialization(catalog_path, _first_name, _id_order, _rating);//
                data_base::get_instance()->update_value(_pool_name, _scheme_name, _collection_name, _id_key, information.first, information.second);
                fclose(value);
            }
            else
            {
                throw std::logic_error("cant create file");
            }
        }
        else
        {
            data_base::get_instance()->update_value(_pool_name, _scheme_name, _collection_name, _id_key, _id_order, _first_name, _rating);
        }

    }
};

#endif //COURSE_WORK_DATA_BASE_COMMAND_UPDATE_VALUE_H
