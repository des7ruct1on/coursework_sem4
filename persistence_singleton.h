

#ifndef COURSE_WORK_DATA_BASE_PERSISTENCE_SINGLETON_H
#define COURSE_WORK_DATA_BASE_PERSISTENCE_SINGLETON_H
#include <vector>
#include <iostream>
#include <ctime>
#include "value.h"
#include <algorithm>
class persistence_singleton
{
private:

    static persistence_singleton* _instance;

public:

    std::vector<std::pair<std::time_t, Value*>> _persistence_entrypoint;

public:

    static persistence_singleton* get_instance()
    {
        if(_instance == nullptr)
        {
            _instance = new persistence_singleton();
        }

        return _instance;
    }

public: 

    std::time_t add(Value* value_data)
    {
        std::time_t now = std::time(nullptr);
        _persistence_entrypoint.emplace_back(now, value_data);
        std::sort(_persistence_entrypoint.begin(), _persistence_entrypoint.end());
        return now;
    }

    Value* get_on_time(const std::time_t &time)
    {
        std::cout << "SIZE: " << _persistence_entrypoint.size() << std::endl;
        for(auto it = _persistence_entrypoint.rbegin(); it != _persistence_entrypoint.rend(); ++it)
        {
            std::pair<std::time_t, Value*>& need = *it;
            std::cout << time << need.first << std::endl;
            if(need.first >= time)
            {
                return need.second;
            }
            else
            {
                continue;
            }

        }
        return nullptr;
    }


private:

    persistence_singleton()
    {
        _instance = this;
    }

public:

    ~persistence_singleton() = default;
    persistence_singleton(persistence_singleton const&) = delete;
    persistence_singleton(persistence_singleton&&) = delete;
    persistence_singleton& operator=(persistence_singleton const&) = delete;
    persistence_singleton& operator=(persistence_singleton&&) = delete;



};

persistence_singleton* persistence_singleton::_instance = nullptr;

#endif //COURSE_WORK_DATA_BASE_PERSISTENCE_SINGLETON_H
