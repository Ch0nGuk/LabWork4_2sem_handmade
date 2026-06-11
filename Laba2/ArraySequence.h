#ifndef ARRAY_SEQUENCE_H
#define ARRAY_SEQUENCE_H

#include "Sequence.h"
#include "DynamicArray.h"
#include "ListSequence.h"
#include "IEnumerator.h"

template <typename T> 
class ArraySequence : public Sequence<T>
{
protected:
    DynamicArray<T> data;

    class ArrayEnumerator : public IEnumerator<T>
    {
    protected:
        const ArraySequence<T>* array;
        int index;
    
    public:
        ArrayEnumerator(const ArraySequence<T>* array_sequence) : array(array_sequence), index(-1) {}
        ArrayEnumerator(const ArrayEnumerator& other) : array(other.array), index(other.index) {}

        bool MoveNext() override
        {
            index++;
            if (index < array->GetLength()) return true;
            return false;
        }

        const T& Current() const override
        {
            return this->array->Get(index);
        }
    };

public:
    ArraySequence() : data() {}
    ArraySequence(const T* items, int size) : data(items, size) {}
    ArraySequence(const DynamicArray<T>& other) : data(other) {}

    virtual ArraySequence<T>* Instance() override = 0;

    const T& GetFirst() const override
    {
        return data.Get(0);
    }
    const T& GetLast() const override
    {
        return data.Get(data.GetSize() - 1);
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
        if (start_index < 0 || end_index < 0 || start_index > end_index || end_index >= this->data.GetSize())
        {
            throw std::out_of_range("Index out of range");
        }

        int sub_size = end_index - start_index + 1;
        DynamicArray<T> sub_data(sub_size, this->data.Get(start_index));
        for (int ind = 0; ind < sub_size; ind++)
        {
            sub_data.Set(ind, this->data.Get(ind + start_index));
        }

        return CreateSequence(sub_data);
    }

    Sequence<T>* Concat(const Sequence<T>& other) const override
    {
        int this_size = this->data.GetSize();
        int other_size = other.GetLength();
        int new_size = this_size + other_size;
        if (new_size == 0)
        {
            return CreateSequence(DynamicArray<T>());
        }

        DynamicArray<T> new_arr(new_size, (this_size > 0) ? this->data.Get(0) : other.Get(0));
        for (int ind = 0; ind < this_size; ind++)
        {
            new_arr.Set(ind, this->data.Get(ind));
        }
        for (int ind = 0; ind < other_size; ind++)
        {
            new_arr.Set(ind + this_size, other.Get(ind));
        }

        return CreateSequence(new_arr);
    }

    Sequence<T>* Slice(int start_index, int count) const override
    {
        int this_size = this->GetLength();
        if (start_index < 0 || count < 0 || start_index > this_size || start_index + count > this_size)
        {
            throw std::out_of_range("Index out of range");
        }

        int new_size = this_size - count;
        if (new_size == 0)
        {
            return CreateSequence(DynamicArray<T>());
        }

        DynamicArray<T> new_arr(
            new_size,
            (start_index > 0) ? this->Get(0) : this->Get(start_index + count));
        int new_index = 0;

        for (int index = 0; index < start_index; index++)
        {
            new_arr.Set(new_index, this->Get(index));
            new_index++;
        }

        for (int index = start_index + count; index < this_size; index++)
        {
            new_arr.Set(new_index, this->Get(index));
            new_index++;
        }

        return CreateSequence(new_arr);
    }

    Sequence<T>* Slice(int start_index, int count, const Sequence<T>& replacement) const override
    {
        int this_size = this->GetLength();
        if (start_index < 0 || count < 0 || start_index > this_size || start_index + count > this_size)
        {
            throw std::out_of_range("Index out of range");
        }

        int replacement_size = replacement.GetLength();
        int new_size = this_size - count + replacement_size;
        if (new_size == 0)
        {
            return CreateSequence(DynamicArray<T>());
        }

        DynamicArray<T> new_arr(
            new_size,
            (start_index > 0)
                ? this->Get(0)
                : ((replacement_size > 0) ? replacement.Get(0) : this->Get(start_index + count)));
        int new_index = 0;

        for (int index = 0; index < start_index; index++)
        {
            new_arr.Set(new_index, this->Get(index));
            new_index++;
        }

        for (int index = 0; index < replacement_size; index++)
        {
            new_arr.Set(new_index, replacement.Get(index));
            new_index++;
        }

        for (int index = start_index + count; index < this_size; index++)
        {
            new_arr.Set(new_index, this->Get(index));
            new_index++;
        }

        return CreateSequence(new_arr);
    }

    Sequence<T>* Where(bool (*predicate)(T)) const override
    {
        int new_size = 0;
        UniquePtr<IEnumerator<T>> enumerator(this->GetEnumerator());
        const T* first_match = nullptr;

        while (enumerator->MoveNext())
        {
            if (predicate(enumerator->Current()))
            {
                if (first_match == nullptr)
                {
                    first_match = &enumerator->Current();
                }
                new_size++;
            }
        }

        if (new_size == 0)
        {
            return CreateSequence(DynamicArray<T>());
        }

        DynamicArray<T> new_arr(new_size, *first_match);
        enumerator.reset(this->GetEnumerator());
        int new_index = 0;

        while (enumerator->MoveNext())
        {
            if (predicate(enumerator->Current()))
            {
                new_arr.Set(new_index, enumerator->Current());
                new_index++;
            }
        }

        return CreateSequence(new_arr);
    }

    IEnumerator<T>* GetEnumerator() const override
    {
        return new ArrayEnumerator(this);
    }

protected:
    Sequence<T>* AppendInternal(const T& item) override
    {
        int old_size = this->data.GetSize();
        this->data.Resize(old_size + 1, item);
        this->data.Set(old_size, item);

        return this;
    }

    Sequence<T>* PrependInternal(const T& item) override
    {
        int old_size = this->data.GetSize();
        DynamicArray<T> tmp_array = this->data;
        this->data.Resize(old_size + 1, item);
        for (int index = 0; index < old_size; index++)
        {
            this->data.Set(index + 1, tmp_array.Get(index));
        }
        this->data.Set(0, item);

        return this;
    }

    Sequence<T>* InsertAtInternal(int index, const T& item) override
    {
        if (index < 0 || index > this->data.GetSize())
        {
            throw std::out_of_range("Index out of range");
        }
        if (index == 0)
        {
            return PrependInternal(item);
        }
        if (index == this->data.GetSize())
        {
            return AppendInternal(item);
        }
        int old_size = this->data.GetSize();
        DynamicArray<T> tmp_array = this->data;
        this->data.Resize(old_size + 1, item);
        for (int ind = index; ind < old_size; ind++)
        {
            this->data.Set(ind + 1, tmp_array.Get(ind));
        }
        this->data.Set(index, item);

        return this;
    }

    virtual ArraySequence<T>* CreateSequence(const DynamicArray<T>& data) const = 0;

};

#endif // ARRAY_SEQUENCE_H
