

#ifndef COURSE_WORK_DATA_BASE_SECOND_KEY_COMPARER_H
#define COURSE_WORK_DATA_BASE_SECOND_KEY_COMPARER_H

#include <iostream>
class second_key_comparer final
{

public:

    unsigned int operator() (Key first, Key second)
    {
        return  first._id_person - second._id_person;
    }

    int operator()(int const &first, int const &second)
    {
        if (first > second)
        {
            return 1;
        }
        else if (first == second)
        {
            return 0;
        }
        return -1;
    }

    int operator()(std::string const &first, std::string const &second)
    {
        if (first > second)
        {
            return 1;
        }
        else if (first == second)
        {
            return 0;
        }
        return -1;
    }

};


#endif //COURSE_WORK_DATA_BASE_SECOND_KEY_COMPARER_H
