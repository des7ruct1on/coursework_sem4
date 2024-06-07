
#ifndef COURSE_WORK_DATA_BASE_WB_H
#define COURSE_WORK_DATA_BASE_WB_H

#include <iostream>
#include <cstring>
#include <fstream>

class Value
{
    virtual void foo() {}
};

class Mem_value : public Value
{
public:

    unsigned int _id;
    char* _nickname;
    double _salary;

    Mem_value(unsigned int id, std::string name, double salary) : _id(id), _salary(salary)
    {
        _nickname = new char[name.size() + 1];
        strcpy(_nickname, name.c_str());
    }

    Mem_value() = default;

    ~Mem_value() = default;

    Mem_value(const Mem_value& other) : _id(other._id), _salary(other._salary)
    {
        _nickname = new char[strlen(other._nickname) + 1];
        strcpy(_nickname, other._nickname);
    }

    Mem_value& operator=(const Mem_value& other)
    {
        if(this != &other)
        {
            delete[] _nickname;
            _id = other._id;
            _salary = other._salary;
            _nickname = new char[strlen(other._nickname) + 1];
            strcpy(_nickname, other._nickname);

        }
        return *this;

    }
    Mem_value(Mem_value&& other) noexcept : _id(std::move(other._id)), _salary(std::move(other._salary))
    {
        other._nickname = nullptr;
    }

    Mem_value& operator=(Mem_value&& other) noexcept
    {
        if (this != &other) {
            delete[] _nickname;
            _id = std::move(other._id);
            _salary = std::move(other._salary);
            _nickname = std::move(other._nickname);
            other._nickname = nullptr;

        }
        return *this;
    }


};

struct Key
{
    unsigned int _id_person;

    Key(unsigned int id) : _id_person(id) {}
};

class File_value : public Value
{
public:

    long _string_size;
    long _position;

    File_value(long position, long string_size) : _position(position), _string_size(string_size) {}
    File_value() = default;
    ~File_value() = default;

    File_value& operator=(const File_value& other)
    {
        if(this != &other) {
            _string_size = other._string_size;
            _position = other._position;
        }
        return *this;

    }
    File_value(File_value&& other) noexcept : _string_size(other._string_size), _position(other._position)
    {

    }
    File_value& operator=(File_value&& other) noexcept
    {
        if(this != &other) {
            _string_size = other._string_size;
            _position = other._position;
        }
        return *this;
    }


};

std::pair<long, long> serialization(std::string file_path, std::string const& nickname, unsigned int id, double salary)
{
    std::pair<long, long> information;
    std::ofstream  file(file_path, std::ios::app);

    file << id << " ";

    long position = file.tellp();
    information.first = position;

    long size = nickname.size();
    information.second = size;
    file.write(nickname.c_str(), size);
    file << " ";

    file << salary;
    return information;
}

void deserialization(long position, long str_size, Mem_value& value, std::string file_path)
{
    std::ifstream file(file_path, std::ios::in); // Открываем файл в режиме чтения
    file >> value._id;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Пропускаем символ новой строки
    value._nickname = new char[str_size + 1]; // Выделяем память для _nickname
    file.getline(value._nickname, str_size + 1); // Считываем строку (учитывая размер)
    file >> value._salary;
}


void deserialization(long position, long str_size, Mem_value* value, std::string file_path)
{
    std::ifstream file(file_path, std::ios::in); // Открываем файл в режиме чтения
    file >> value->_id;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Пропускаем символ новой строки
    value->_nickname = new char[str_size + 1]; // Выделяем память для _nickname
    file.getline(value->_nickname, str_size + 1); // Считываем строку (учитывая размер)
    file >> value->_salary;
}




#endif //COURSE_WORK_DATA_BASE_WB_H