#ifndef MUTABLE_ARRAY_SEQUENCE_H
#define MUTABLE_ARRAY_SEQUENCE_H

#include "ArraySequence.h"
#include "DynamicArray.h"


template <typename T> 
class MutableArraySequence : public ArraySequence<T>
{
public:
    MutableArraySequence() : ArraySequence<T>() {}
    MutableArraySequence(const T* items, int size) : ArraySequence<T>(items, size) {}
    MutableArraySequence(const DynamicArray<T>& other) : ArraySequence<T>(other) {}

    MutableArraySequence<T>* Instance() override
    {
        return this;
    }
    
protected:
    MutableArraySequence<T>* CreateSequence(const DynamicArray<T>& data) const override
    {
        return new MutableArraySequence<T>(data);
    }
};

#endif // MUTABLE_ARRAY_SEQUENCE_H
