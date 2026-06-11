#pragma once

#include "Generator.h"
#include "Cardinal.h"
#include "Ordinal.h"
#include "Laba2/SharedPtr.h"

#include <stdexcept> // для вброса исключений

template <class T>
class ConcatGenerator : public Generator<T>
{
public:
    ConcatGenerator(const SharedPtr<Generator<T>>& left, const SharedPtr<Generator<T>>& right) : left_(left), right_(right)
    {
        if (!left || !right)
        {
            throw std::invalid_argument("Part cannot be empty");
        }
    }

    const T& Get(const Ordinal& index) const override
    {
        if (index < left_->GetOrdinalLength()) // если индекс в левой части, то возвращаем Get из нее
        {
            return left_->Get(index);
        }

        Ordinal residual = index.RemovePrefix(left_->GetOrdinalLength()); // убираем у индекса длину левой части

        if (residual > right_->GetOrdinalLength())
        {
            throw std::out_of_range("Index out of range");
        }

        return right_->Get(residual);
    }

    Ordinal GetOrdinalLength() const override
    {
        return left_->GetOrdinalLength().Add(right_->GetOrdinalLength());
    }

    Cardinal GetCardinalLength() const override
    {
        return GetOrdinalLength().ToCardinal();
    }

private:
    SharedPtr<Generator<T>> left_;
    SharedPtr<Generator<T>> right_;
};
