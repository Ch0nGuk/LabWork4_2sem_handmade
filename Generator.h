#pragma once

#include "Ordinal.h"
#include "Cardinal.h"

template <class T>
class Generator // Виртуальный класс, интерфейс всех генераторов. Должен уметь возвращать элемент по индексу
{               // и возвращать ординальную и кардинальную длины последовательности, которую генерирует
public:
    virtual ~Generator() {}

    virtual const T& Get(const Ordinal& index) const = 0;
    virtual Ordinal GetOrdinalLength() const = 0;
    virtual Cardinal GetCardinalLength() const = 0;
};
