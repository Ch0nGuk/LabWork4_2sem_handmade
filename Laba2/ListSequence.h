#ifndef LIST_SEQUENCE_H
#define LIST_SEQUENCE_H

#include "Sequence.h"
#include "LinkedList.h"
#include "IEnumerator.h"

template <typename T> 
class ListSequence : public Sequence<T>
{
protected:
    LinkedList<T> data;

public:
    ListSequence() : data() {}
    ListSequence(const T* items, int size) : data(items, size) {}
    ListSequence(const LinkedList<T>& other) : data(other) {}

    ListSequence<T>* Instance() override
    {
        return this;
    }

    const T& GetFirst() const override
    {
        return data.GetFirst();
    }

    const T& GetLast() const override
    {
        return data.GetLast();
    }

    const T& Get(int index) const override
    {
        return data.Get(index);
    }

    int GetLength() const override
    {
        return data.GetSize();
    }

    Sequence<T>* Append(const T& item) override
    {
        return Instance()->AppendInternal(item);
    }

    Sequence<T>* Prepend(const T& item) override
    {
        return Instance()->PrependInternal(item);
    }

    Sequence<T>* InsertAt(int index, const T& item) override
    {
        return Instance()->InsertAtInternal(index, item);
    }

    Sequence<T>* GetSubsequence(int start_index, int end_index) const override
    {
        if (start_index < 0 || end_index < 0 || start_index > end_index || end_index >= data.GetSize())
        {
            throw std::out_of_range("Index out of range");
        }

        return new ListSequence<T>(data.GetSubList(start_index, end_index));
    }

    Sequence<T>* Concat(const Sequence<T>& other) const override
    {
        LinkedList<T> other_list;
        UniquePtr<IEnumerator<T>> enumerator(other.GetEnumerator());

        while (enumerator->MoveNext())
        {
            other_list.Append(enumerator->Current());
        }

        return new ListSequence<T>(data.Concat(other_list));
    }

    Sequence<T>* Slice(int start_index, int count) const override
    {
        int this_size = this->GetLength();
        if (start_index < 0 || count < 0 || start_index > this_size || start_index + count > this_size)
        {
            throw std::out_of_range("Index out of range");
        }

        LinkedList<T> new_list;
        UniquePtr<IEnumerator<T>> enumerator(this->GetEnumerator());
        int index = 0;

        while (enumerator->MoveNext())
        {
            if (index < start_index || index >= start_index + count)
            {
                new_list.Append(enumerator->Current());
            }
            index++;
        }

        return new ListSequence<T>(new_list);
    }

    Sequence<T>* Slice(int start_index, int count, const Sequence<T>& replacement) const override
    {
        int this_size = this->GetLength();
        if (start_index < 0 || count < 0 || start_index > this_size || start_index + count > this_size)
        {
            throw std::out_of_range("Index out of range");
        }

        LinkedList<T> new_list;
        UniquePtr<IEnumerator<T>> source_enumerator(this->GetEnumerator());
        UniquePtr<IEnumerator<T>> replacement_enumerator(replacement.GetEnumerator());
        bool inserted_replacement = false;
        int index = 0;

        while (source_enumerator->MoveNext())
        {
            if (!inserted_replacement && index == start_index)
            {
                while (replacement_enumerator->MoveNext())
                {
                    new_list.Append(replacement_enumerator->Current());
                }
                inserted_replacement = true;
            }

            if (index < start_index || index >= start_index + count)
            {
                new_list.Append(source_enumerator->Current());
            }

            index++;
        }

        if (!inserted_replacement)
        {
            while (replacement_enumerator->MoveNext())
            {
                new_list.Append(replacement_enumerator->Current());
            }
        }

        return new ListSequence<T>(new_list);
    }


    Sequence<T>* Where(bool (*predicate)(T)) const override
    {
        LinkedList<T> new_list;
        UniquePtr<IEnumerator<T>> enumerator(this->GetEnumerator());

        while (enumerator->MoveNext())
        {
            if (predicate(enumerator->Current())) 
            {
                new_list.Append(enumerator->Current());
            }
        }

        return new ListSequence<T>(new_list);
    }

    IEnumerator<T>* GetEnumerator() const override
    {
        return data.GetEnumerator();
    }


protected:
    Sequence<T>* AppendInternal(const T& item) override
    {
        data.Append(item);
        return this;
    }

    Sequence<T>* PrependInternal(const T& item) override
    {
        data.Prepend(item);
        return this;
    }

    Sequence<T>* InsertAtInternal(int index, const T& item) override
    {
        data.InsertAt(index, item);
        return this;
    }
};

#endif // LIST_SEQUENCE_H
