#pragma once

#include <cstddef> // для size_t

#include "Ordinal.h"
#include "Cardinal.h"

template <class T>
class Generator // Виртуальный класс, интерфейс всех генераторов. Должен уметь возвращать элемент по индексу
{               // возвращать ординальную и кардинальную длины последовательности, которую генерирует, и кол-во эл-тов в кэше
public:
    virtual ~Generator() {}

    virtual const T& Get(const Ordinal& index) const = 0;
    virtual Ordinal GetOrdinalLength() const = 0;
    virtual Cardinal GetCardinalLength() const = 0;
    virtual size_t GetMaterializedCount() const = 0;
};