#ifndef IMMUTABLE_ARRAY_SEQUENCE_H
#define IMMUTABLE_ARRAY_SEQUENCE_H

#include "ArraySequence.h"
#include "DynamicArray.h"

template <typename T>
class ImmutableArraySequence : public ArraySequence<T>
{
public:
    ImmutableArraySequence() : ArraySequence<T>() {}
    ImmutableArraySequence(const T* items, int size) : ArraySequence<T>(items, size) {}
    ImmutableArraySequence(const DynamicArray<T>& other) : ArraySequence<T>(other) {}

    ImmutableArraySequence<T>* Instance() override
    {
        return new ImmutableArraySequence(this->data);
    }

protected:
    ImmutableArraySequence<T>* CreateSequence(const DynamicArray<T>& data) const override
    {
        return new ImmutableArraySequence<T>(data);
    }
};

#endif // IMMUTABLE_ARRAY_SEQUENCE_H
