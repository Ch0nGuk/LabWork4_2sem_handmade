#pragma once

#include <cstddef>

template <typename T>
class UniquePtr
{
private:
    T* ptr;

public:
    UniquePtr() noexcept : ptr(nullptr) {}

    UniquePtr(std::nullptr_t) noexcept : ptr(nullptr) {}

    explicit UniquePtr(T* raw_ptr) noexcept : ptr(raw_ptr) {}

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr)
    {
        other.ptr = nullptr;
    }

    ~UniquePtr()
    {
        delete ptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept
    {
        if (this != &other)
        {
            reset(other.release());
        }

        return *this;
    }

    UniquePtr& operator=(std::nullptr_t) noexcept
    {
        reset();
        return *this;
    }

    T* get() const noexcept
    {
        return ptr;
    }

    T* release() noexcept
    {
        T* released = ptr;
        ptr = nullptr;
        return released;
    }

    void reset(T* new_ptr = nullptr) noexcept
    {
        if (ptr == new_ptr)
        {
            return;
        }

        T* old_ptr = ptr;
        ptr = new_ptr;
        delete old_ptr;
    }

    T& operator*() const noexcept
    {
        return *ptr;
    }

    T* operator->() const noexcept
    {
        return ptr;
    }

    explicit operator bool() const noexcept
    {
        return ptr != nullptr;
    }
};