
#ifndef COURSE_WORK_DATA_BASE_ALLOCATOR_SORTED_LIST_H
#define COURSE_WORK_DATA_BASE_ALLOCATOR_SORTED_LIST_H


#include "allocator_guardant.h"
#include "allocator_test_utils.h"
#include "allocator_with_fit_mode.h"
#include "../logger/logger_guardant.h"
#include "allocator.h"
#include "typename_holder.h"
#include <iostream>
#include <chrono>
#include <mutex>
#include <thread>



class allocator_sorted_list final:
    private allocator_guardant,
    public allocator_test_utils,
    public allocator_with_fit_mode,
    private logger_guardant,
    private typename_holder
{

private:
    
    void *_trusted_memory;

public:
    
    ~allocator_sorted_list() override;
    
    allocator_sorted_list(allocator_sorted_list const &other);
    
    allocator_sorted_list &operator=(allocator_sorted_list const &other) = delete;
    
    allocator_sorted_list(allocator_sorted_list &&other) noexcept;
    
    allocator_sorted_list &operator=(allocator_sorted_list &&other) noexcept = delete;

public:
    
    explicit allocator_sorted_list(size_t space_size, allocator *parent_allocator = nullptr, logger *logger = nullptr, allocator_with_fit_mode::fit_mode allocate_fit_mode = allocator_with_fit_mode::fit_mode::first_fit);

public:
    
    [[nodiscard]] void *allocate(size_t value_size, size_t values_count) override;
    
    void deallocate(void *at) override;

public:
    
    inline void set_fit_mode(allocator_with_fit_mode::fit_mode mode) override;

    inline allocator_with_fit_mode::fit_mode get_fit_mode();

private:
    
    inline allocator *get_allocator() const override;

public:
    
    std::vector<allocator_test_utils::block_info> get_blocks_info() const noexcept;

private:
    
    inline logger *get_logger() const override;

private:
    
    inline std::string get_typename() const noexcept;

    void* get_first_available_block() const noexcept;

    allocator::block_size_t get_available_block_size(void* block_address) const noexcept;

    void* get_available_block_next_block_address(void* block_address) const noexcept;

    allocator::block_size_t get_occupied_block_size(void* block_address) const noexcept;

    void* find_block(allocator_with_fit_mode::fit_mode fit_mode, size_t requested_size);

    void set_first_available_block(void* first_available_block) const noexcept;

    void* get_first_block() const noexcept;

    void clear_available_block(void* block) const noexcept;

    void merge_blocks(int status_free, void* first, void* second) noexcept;

    allocator* get_occupied_block_allocator(void * block) const noexcept;

    void print_blocks_info(std::vector<allocator_test_utils::block_info> blocks_info) const noexcept;
    
    std::string get_block_info(void* block) const noexcept;
    
    std::mutex & get_mutex() const noexcept;
};


#endif //COURSE_WORK_DATA_BASE_ALLOCATOR_SORTED_LIST_H
