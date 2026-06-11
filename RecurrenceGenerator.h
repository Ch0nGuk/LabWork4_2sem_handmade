#pragma once

#include "Ordinal.h"
#include "Cardinal.h"
#include "Laba2/Sequence.h"
#include "Generator.h"
#include "Laba2/ListSequence.h"

#include <functional> // для std::function
#include <stdexcept>  // для вброса исключений
#include <stddef.h>   // для size_t
#include <limits>     // для std::numeric_limits
#include <utility>    // для std::move

template <class T> 
class RecurrenceGenerator : public Generator<T>
{
public:
    RecurrenceGenerator(std::function<T(const Sequence<T>&)> rec_rule, const Sequence<T>& start_values) :
    rule(std::move(rec_rule)), cache()
    {
        if (!rule)
        {
            throw std::invalid_argument("Rule is empty");
        }

        if (start_values.GetLength() == 0)
        {
            throw std::invalid_argument("Start values are empty");
        }

        int count = start_values.GetLength();
        for (int index = 0; index < count; index++)
        {
            cache.Append(start_values.Get(index));
        }
    }

    const T& Get(const Ordinal& index) const override
    {
        if (!index.IsFinite())
        {
            throw std::out_of_range("RecurrenceGenerator accepts only finite index");
        }

        size_t finite_index = index.GetFinitePart();

        if (finite_index > static_cast<size_t>(std::numeric_limits<int>::max()))
        {
            throw std::overflow_error("Get index must be fit to int"); // Получаемый индекс должен соответствовать int
        }

        int int_index = static_cast<int>(finite_index);

        while (cache.GetLength() <= int_index) // если длина кэша меньше вызываемого индекса, то вычисляем индексы
        {
            T value = rule(cache);
            cache.Append(value);
        }

        return cache.Get(int_index);
    }

    Ordinal GetOrdinalLength() const override
    {
        return Ordinal::Omega();
    }

    Cardinal GetCardinalLength() const override
    {
        return Cardinal::Infinity();
    }

    size_t GetMaterializedCount() const override
    {
        return static_cast<size_t>(cache.GetLength());
    }


private:
    std::function<T(const Sequence<T>&)> rule; 
    mutable ListSequence<T> cache; // mutable значит что можно изменять это поле даже внутри const-метода
                                   // rule хранит функцию вида T SomeRule(const Sequence<T>& sequence);
                                   // const Sequence, потому что здесь эта последовательность только для чтения
};
