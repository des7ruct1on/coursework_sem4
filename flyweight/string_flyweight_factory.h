
#ifndef COURSE_WORK_DATA_BASE_STRING_FLYWEIGHT_FACTORY_H
#define COURSE_WORK_DATA_BASE_STRING_FLYWEIGHT_FACTORY_H
#include "string_flyweight.h"
#include <unordered_map>
#include <memory>
#include <string>

class string_flyweight_factory
{
private:

    string_flyweight_factory() {};

    std::unordered_map<std::string, std::shared_ptr<string_flyweight>> _string_flyweights;

public:

    static string_flyweight_factory& get_instance()
    {
        static string_flyweight_factory instance;
        return instance;
    }

    std::shared_ptr<string_flyweight> get_string_flyweight(const std::string& value)
    {
        auto it = _string_flyweights.find(value);
        if(it != _string_flyweights.end())
        {
            return it->second;
        }
        else
        {
            auto flyweight = std::make_shared<string_flyweight>();
            flyweight->set_value(value);
            _string_flyweights[value] = flyweight;

            return flyweight;
        }
    }
};

#endif //COURSE_WORK_DATA_BASE_STRING_FLYWEIGHT_FACTORY_H
