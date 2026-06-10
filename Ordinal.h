#pragma once 

#include <cstddef> // для size_t
#include <stdexcept> // для вброса стандартных классов исключений
#include <limits> // для проверки переполнения в Add

#include "Cardinal.h"

class Ordinal
{
public:
    // static перед методом значит что метод принадлежит не объекту класса, а самому классу и может вызываться так Cardinal::Finite
    static Ordinal Zero() // 0
    {
        return Ordinal(0, 0);
    }

    static Ordinal Finite(size_t value) // обычное конечное число в форме ordinal
    {
        return Ordinal(0, value);
    }

    static Ordinal Omega() // просто w
    {
        return Ordinal(1, 0);
    }

    static Ordinal OmegaPlus(size_t value) // w + n
    {
        return Ordinal(1, value);
    }

    static Ordinal OmegaTimes(size_t value) // w*k
    {
        return Ordinal(value, 0);
    }

    static Ordinal OmegaTimesPlus(size_t omega_value, size_t finite_value) // w*k + n
    {
        return Ordinal(omega_value, finite_value);
    }

    bool IsZero() const
    {
        return (omega_ == 0 && value_ == 0);
    }

    bool IsFinite() const
    {
        return (omega_ == 0);
    }

    bool IsLimit() const // нет конечного элемента
    {
        return omega_ != 0 && value_ == 0;
    }

    bool IsCompleted() const // есть конечный элемент
    {
        return value_ != 0;
    }

    size_t GetFinitePart() const
    {
        return value_;
    }

    size_t GetOmegaPart() const
    {
        return omega_;
    }

    Ordinal Add(const Ordinal& right) const // сложение 2х ординалов
    {
        size_t new_omega = omega_ + right.omega_;
        size_t new_value;

        if (right.IsFinite())
        {
            if (right.value_ > std::numeric_limits<size_t>::max() - value_)
            {
                throw std::overflow_error("Ordinal addition overflow");
            }
            new_value = value_ + right.value_;
        }
        else
        {
            new_value = right.value_;
        }

        return Ordinal(new_omega, new_value);
    }

    Cardinal ToCardinal() const
    {
        if (IsFinite())
        {
            return Cardinal::Finite(value_);
        }

        return Cardinal::Infinity();
    }

    bool operator==(const Ordinal& other) const
    {
        return Compare(other) == 0;
    }

    bool operator!=(const Ordinal& other) const
    {
        return Compare(other) != 0;
    }

    bool operator<(const Ordinal& other) const
    {
        return Compare(other) < 0;
    }

    bool operator<=(const Ordinal& other) const
    {
        return Compare(other) <= 0;
    }

    bool operator>(const Ordinal& other) const
    {
        return Compare(other) > 0;
    }

    bool operator>=(const Ordinal& other) const
    {
        return Compare(other) >= 0;
    }

private:
    size_t value_;
    size_t omega_;

    Ordinal(size_t omega, size_t value) : omega_(omega), value_(value) {} // приватный конструктор, чтобы юзер работал через фабрики 

    int Compare(const Ordinal& other) const // вспомогательный метод сравнения двух ординальных чисел. 
    {                                        // Приватный, тк не требуется юзеру класса
        if (omega_ > other.omega_)
        {
            return 1;
        }

        if (omega_ < other.omega_)
        {
            return -1;
        }

        if (value_ < other.value_)
        {
            return -1;
        }

        if (value_ > other.value_)
        {
            return 1;
        }

        return 0;
    }
};