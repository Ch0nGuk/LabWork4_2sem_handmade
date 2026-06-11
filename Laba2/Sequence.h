#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <utility>

#include "UniquePtr.h"
#include "IEnumerator.h"

template <typename T>
class SequenceFactory;

template <typename T>
class Sequence
{
public:
    virtual ~Sequence() = default;

    virtual const T& GetFirst() const = 0;
    virtual const T& GetLast() const = 0;
    virtual const T& Get(int index) const = 0;
    virtual int GetLength() const = 0;

    virtual Sequence<T>* Append(const T& item) = 0;
    virtual Sequence<T>* Prepend(const T& item) = 0;
    virtual Sequence<T>* InsertAt(int index, const T& item) = 0;
    virtual Sequence<T>* GetSubsequence(int start_index, int end_index) const = 0;
    virtual Sequence<T>* Concat(const Sequence<T>& other) const = 0;
    virtual Sequence<T>* Slice(int start_index, int count) const = 0;
    virtual Sequence<T>* Slice(int start_index, int count, const Sequence<T>& replacement) const = 0;
    virtual IEnumerator<T>* GetEnumerator() const = 0;

    virtual Sequence<T>* Instance() = 0;

    virtual Sequence<T>* Where(bool (*predicate)(T)) const = 0;
    T Reduce(T (*func)(T, T), T start_value) const
    {
        T result = start_value;
        UniquePtr<IEnumerator<T>> enumerator(GetEnumerator());

        while (enumerator->MoveNext())
        {
            result = func(result, enumerator->Current());
        }

        return result;
    }

    template <typename T2, typename Mapper>
    Sequence<T2>* Map(Mapper mapper, const SequenceFactory<T2>& factory) const
    {
        UniquePtr<Sequence<T2>> result(factory.Create());
        UniquePtr<IEnumerator<T>> enumerator(GetEnumerator());

        while (enumerator->MoveNext())
        {
            Sequence<T2>* next = result->Append(mapper(enumerator->Current()));
            if (next != result.get())
            {
                result.reset(next);
            }
        }

        return result.release();
    }

    template <typename T2>
    Sequence<std::pair<T, T2>>* Zip(
        const Sequence<T2>& other,
        const SequenceFactory<std::pair<T, T2>>& factory) const
    {
        UniquePtr<Sequence<std::pair<T, T2>>> result(factory.Create());
        UniquePtr<IEnumerator<T>> first_enumerator(GetEnumerator());
        UniquePtr<IEnumerator<T2>> second_enumerator(other.GetEnumerator());

        while (first_enumerator->MoveNext() && second_enumerator->MoveNext())
        {
            Sequence<std::pair<T, T2>>* next = result->Append(
                std::make_pair(first_enumerator->Current(), second_enumerator->Current()));

            if (next != result.get())
            {
                result.reset(next);
            }
        }

        return result.release();
    }

    template <typename T1, typename T2>
    std::pair<Sequence<T1>*, Sequence<T2>*> Unzip(
        const SequenceFactory<T1>& first_factory,
        const SequenceFactory<T2>& second_factory) const
    {
        UniquePtr<Sequence<T1>> first_result(first_factory.Create());
        UniquePtr<Sequence<T2>> second_result(second_factory.Create());
        UniquePtr<IEnumerator<T>> enumerator(GetEnumerator());

        while (enumerator->MoveNext())
        {
            const T& value = enumerator->Current();

            Sequence<T1>* next_first = first_result->Append(value.first);
            if (next_first != first_result.get())
            {
                first_result.reset(next_first);
            }

            Sequence<T2>* next_second = second_result->Append(value.second);
            if (next_second != second_result.get())
            {
                second_result.reset(next_second);
            }
        }

        return std::make_pair(first_result.release(), second_result.release());
    }

    
protected:
    virtual Sequence<T>* AppendInternal(const T& item) = 0;
    virtual Sequence<T>* PrependInternal(const T& item) = 0;
    virtual Sequence<T>* InsertAtInternal(int index, const T& item) = 0;
};

#endif // SEQUENCE_H
