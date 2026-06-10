#ifndef SEQUENCE_FACTORY_H
#define SEQUENCE_FACTORY_H

#include "Sequence.h"
#include "MutableArraySequence.h"
#include "ImmutableArraySequence.h"
#include "ListSequence.h"

template <typename T>
class SequenceFactory
{
public:
    virtual Sequence<T>* Create() const = 0;
    virtual ~SequenceFactory() = default;
};

template <typename T>
class MutableArraySequenceFactory : public SequenceFactory<T>
{
public:
    Sequence<T>* Create() const override
    {
        return new MutableArraySequence<T>();
    }
};

template <typename T>
class ImmutableArraySequenceFactory : public SequenceFactory<T>
{
public:
    Sequence<T>* Create() const override
    {
        return new ImmutableArraySequence<T>();
    }
};

template <typename T>
class ListSequenceFactory : public SequenceFactory<T>
{
public:
    Sequence<T>* Create() const override
    {
        return new ListSequence<T>();
    }
};

#endif // SEQUENCE_FACTORY_H
