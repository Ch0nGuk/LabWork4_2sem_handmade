#pragma once

#include "Generator.h"
#include "Ordinal.h"
#include "Cardinal.h"
#include "Laba2/DynamicArray.h"
#include "Laba2/Sequence.h"

#include <stdexcept> // для вброса исключений
#include <utility> // для std::move()
#include <cstddef> // для size_t

template <class T>
class FiniteGenerator : public Generator<T>
{
public:
    FiniteGenerator() : data() {}

    explicit FiniteGenerator(const DynamicArray<T>& items) : data(items) {} // explicit запрещает неявный вызов конструктора с одним аргументом. 

    FiniteGenerator(const T* items, int count) : data(items, count) {}      // Поэтому FiniteGenerator<int> gen = data; не скомпилируется, 

    FiniteGenerator(T* items, int count) : data(items, count) {}            // а явная запись FiniteGenerator<int> gen(data); скомпилируется. 

    explicit FiniteGenerator(const Sequence<T>* sequence) : data()          // Это предотвращает случайное преобразование DynamicArray<int> в FiniteGenerator<int>.   
    {
        if (sequence == nullptr)
        {
            throw std::invalid_argument("Source sequence is null");
        }

        int count = sequence->GetLength();
        if(count == 0) return;

        DynamicArray<T> tmp_arr(count, sequence->Get(0));

        for (int i = 1; i < count; i++)
        {
            tmp_arr[i] = sequence->Get(i);
        }

        data = std::move(tmp_arr);
    }                                                                    

    const T& Get(const Ordinal& index) const override
    {
        if (!index.IsFinite())
        {
            throw std::out_of_range("FiniteGenerator accepts only finite index");
        }

        size_t finite_index = index.GetFinitePart();

        if (finite_index >= static_cast<size_t>(data.GetSize()))
        {
            throw std::out_of_range("Index is out of range");
        }

        return data.Get(static_cast<int>(finite_index));
    }

    Ordinal GetOrdinalLength() const override
    {
        return Ordinal::Finite(static_cast<size_t>(data.GetSize()));
    }

    Cardinal GetCardinalLength() const override
    {
        return Cardinal::Finite(static_cast<size_t>(data.GetSize()));
    }

private:
    DynamicArray<T> data;
};
