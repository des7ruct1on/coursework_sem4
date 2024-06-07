
#ifndef COURSE_WORK_DATA_BASE_VERSION_SINGLETON_H
#define COURSE_WORK_DATA_BASE_VERSION_SINGLETON_H
#include <iostream>
#include <cstring>
#include <fstream>
#include <string>



class version_singleton
{
private:
    static version_singleton* _instance;
    std::ofstream _stream;
    std::string _file_name;

private:
    explicit version_singleton(const std::string& file_name) : _file_name(file_name), _stream(file_name)
    {
    }

public:
    static version_singleton* get_instance(const std::string& file_name)
    {
        if (_instance == nullptr)
        {
            _instance = new version_singleton(file_name);
        }
        return _instance;
    }

public:
    void write_to_file(const std::string& data)
    {
        if (_stream.is_open())
        {
            _stream << data << std::endl;
        }
    }

    ~version_singleton()
    {
        if (_stream.is_open())
        {
            _stream.close();
        }
    }

    version_singleton(const version_singleton&) = delete;
    version_singleton& operator=(const version_singleton&) = delete;

};

version_singleton* version_singleton::_instance = nullptr;

#endif //COURSE_WORK_DATA_BASE_VERSION_SINGLETON_H
