#pragma once

#include "Generator.h"
#include "FiniteGenerator.h"
#include "RecurrenceGenerator.h"
#include "ConcatGenerator.h"
#include "Ordinal.h"
#include "Cardinal.h"
#include "Laba2/SharedPtr.h"

template <class T>
class LazySequence : Sequence<T>
{
public:

    explicit LazySequence(const SharedPtr<Generator<T>>& generator) : main_generator(generator) 
    {
        if (generator.IsNull())
        {
            throw std::invalid_argument("Generator is empty");
        }
    }

    Ordinal GetOrdinalLength() const 
    {
        return main_generator->GetOrdinalLength();
    }

    Cardinal GetCardinalLength() const
    {
        return main_generator->GetCardinalLength();
    }

    Sequence<T>* Concat(const Sequence<T>& other) const
    {
        const LazySequence<T>* other_lazy = dynamic_cast<LazySequence<T>*>(&other);
        
        SharedPtr<LazySequence<T>> right = (other_lazy == nullptr) ? FiniteGenerator(other) : other_lazy;

        return new LazySequence<T>(SharedPtr<Generator<T>(new ConcatGenerator(main_generator, right)));

    }

private:
    SharedPtr<Generator<T>> main_generator;
};