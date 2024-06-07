

#ifndef COURSE_WORK_DATA_BASE_ASSOCIATIVE_CONTAINER_H
#define COURSE_WORK_DATA_BASE_ASSOCIATIVE_CONTAINER_H

#include <iostream>
#include <vector>

template<
        typename tkey,
        typename tvalue>
class associative_container
{

public:

    struct key_value_pair final
    {

    public:

        tkey key;
        tvalue value;

    public:

        key_value_pair(const tkey& key, const tvalue& value) : key(key), value(value)
        {

        }

    };

    struct key_value_ptr_pair
    {

    public:

        tkey key;
        tvalue *value_ptr;

    };

public:

    // TODO
    struct associative_container_iterator
    {

    public:

    };

public:

    virtual ~associative_container() noexcept = default;

public:

    virtual void insert(
            tkey const &key,
            tvalue const &value) = 0;

    virtual void insert(
            tkey const &key,
            tvalue &&value) = 0;

    virtual tvalue const &obtain(
            tkey const &key) = 0;

    virtual tvalue dispose(
            tkey const &key) = 0;

};

#endif //COURSE_WORK_DATA_BASE_ASSOCIATIVE_CONTAINER_H
