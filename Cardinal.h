#pragma once

#include <stdexcept> // для вброса стандартных классов исключений 
#include <limits> // для 57 строки, где проверяется переполнение int 
#include <cstddef> // нужно для size_t

class Cardinal
{
public:
    // static перед методом значит что метод принадлежит не объекту класса, а самому классу и может вызываться так Cardinal::Finite
    static Cardinal Finite(size_t value) // конструктор из целого положительного числа в конечное кардинальное число
    {
        return Cardinal(false, value);
    }

    static Cardinal Finite(int value) // тот же конструктор для int value
    {
        if (value < 0)
        {
            throw std::out_of_range("Negative cardinal");
        }

        return Cardinal(false, static_cast<size_t>(value)); // static_cast -- явное преобразование типа (у нас это из int в size_t)
    }

    static Cardinal Infinity() // конструктор бесконечного счетного кардинального числа
    {
        return Cardinal(true, 0);
    }

    bool IsFinite() const
    {
        return !omega_;
    }

    bool IsInfinite() const
    {
        return omega_;
    }

    size_t ToSizeT() const // перевести кардинальное число в тип size_t, бросает исключение для бесконечных кардиналов
    {
        if (IsInfinite()) 
        {
            throw std::logic_error("Infinite cardinal cannot be converted to size_t");
        }

        return value_;
    }

    int ToInt() const // перевести кардинальное число в тип int
    {
        if (IsInfinite())
        {
            throw std::logic_error("Infinite cardinal cannot be converted to size_t");
        }

        if (value_ > static_cast<size_t>(std::numeric_limits<int>::max()))
        {
            throw std::out_of_range("Cardinal out of range int");
        }

        return static_cast<int>(value_);
    }

    bool operator==(const Cardinal& other) const
    {
        return (this->Compare(other) == 0);
    }
    
    bool operator!=(const Cardinal& other) const
    {
        return !(*this == other);
    }

    bool operator>=(const Cardinal& other) const
    {
        return (this->Compare(other) >= 0);
    }

    bool operator>(const Cardinal& other) const
    {
        return (this->Compare(other) == 1);
    }

    bool operator<=(const Cardinal& other) const
    {
        return (this->Compare(other) <= 0);
    }

    bool operator<(const Cardinal& other) const
    {
        return (this->Compare(other) == -1);
    }

private:
    bool omega_;
    size_t value_;
    Cardinal(bool omega, size_t value) : omega_(omega), value_(omega ? 0 : value) {} // конструктор приватный, чтобы нельзя было 
                                                                                     // сделать кардинальное число типа (w, 1)
    int Compare(const Cardinal& other) const // вспомогательный метод сравнения двух кардинальных чисел. 
    {                                        // Приватный, тк не требуется юзеру класса
        if (IsInfinite())
        {
            return other.IsInfinite() ? 0 : 1;
        }

        if (other.IsInfinite())
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