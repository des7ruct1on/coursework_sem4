
#ifndef COURSE_WORK_DATA_BASE_STRING_FLYWEIGHT_H
#define COURSE_WORK_DATA_BASE_STRING_FLYWEIGHT_H
#include <string>

class string_flyweight
{
private:

    std::string _value;

public:

    char* get_value()
    {
        return const_cast<char*>(_value.c_str());
    }

    void set_value(const std::string& value)
    {
        _value = value;
    }
};

#endif //COURSE_WORK_DATA_BASE_STRING_FLYWEIGHT_H
