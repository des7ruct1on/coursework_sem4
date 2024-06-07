//
// Created by 79257 on 19.05.2024.
//

#ifndef COURSE_WORK_DATA_BASE_ALLOCATOR_GLOBAL_HEAP_H
#define COURSE_WORK_DATA_BASE_ALLOCATOR_GLOBAL_HEAP_H


#include "allocator.h"
#include "../logger/logger.h"
#include "../logger/logger_guardant.h"
//#include <typename_holder.h>
#include "../logger/logger_builder.h"

class allocator_global_heap final:
        public allocator,
        private logger_guardant
{

private:


            logger *_log_allocator = nullptr;

public:

    explicit allocator_global_heap(logger* log_allocator);

    ~allocator_global_heap() override;

    allocator_global_heap(allocator_global_heap const &other) = default;

    allocator_global_heap &operator=(allocator_global_heap const &other) = delete;

    allocator_global_heap(allocator_global_heap &&other) noexcept = default;

    allocator_global_heap &operator=(allocator_global_heap &&other) noexcept = default;

    allocator_global_heap();

public:

    [[nodiscard]] void *allocate(size_t value_size,size_t values_count) override;

    void deallocate(void *at) override;

//    logger *_logger;

//public:
//
//    explicit allocator_global_heap(
//            logger *logger = nullptr);
//
//    ~allocator_global_heap() override;
//
//    allocator_global_heap(
//            allocator_global_heap const &other) = delete;
//
//    allocator_global_heap &operator=(
//            allocator_global_heap const &other) = delete;
//
//    allocator_global_heap(
//            allocator_global_heap &&other) noexcept;
//
//    allocator_global_heap &operator=(
//            allocator_global_heap &&other) noexcept;

//public:
//
//    [[nodiscard]] void *allocate(
//            size_t value_size,
//            size_t values_count) override;
//
//    void deallocate(
//            void *at) override;


public:

    void foo()
    {};

private:

    inline logger *get_logger() const override;

private:

    inline std::string get_typename() const noexcept;

    std::string get_memory_state(void* at) const;



public:

};


#endif //COURSE_WORK_DATA_BASE_ALLOCATOR_GLOBAL_HEAP_H
