#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdexcept>
#include <utility>
#include "IEnumerator.h"

template <typename T>
class LinkedList
{
private:
    template <typename U>
    struct Node
    {
        U data;
        Node<U>* next;

        Node(const U& data, Node<U>* next = nullptr) : data(data), next(next) {}
    };

    Node<T>* head;
    Node<T>* tail;
    int size;

    bool CheckIndex(int index) const
    {
        if (index < 0 || index >= size)
        {
            throw std::out_of_range("Index out of range");
        }

        return true;
    }

    bool Clear()
    {
        Node<T>* current = head;
        while (current != nullptr)
        {
            Node<T>* next = current->next;
            delete current;
            current = next;
        }

        head = nullptr;
        tail = nullptr;
        size = 0;
        return true;
    }

    Node<T>* GetNode(int index) const
    {
        CheckIndex(index);

        Node<T>* current = head;
        for (int i = 0; i < index; i++)
        {
            current = current->next;
        }

        return current;
    }

    class ListEnumerator : public IEnumerator<T>
    {
    protected:
        const Node<T>* current;
        const Node<T>* next;
    
    public:
        ListEnumerator(const Node<T>* head) : current(nullptr), next(head) {}
        ListEnumerator(const ListEnumerator& other) : current(other.current), next(other.next) {}


        bool MoveNext() override
        {
            if (next == nullptr) return false;
            this->current = this->next;
            this->next = this->next->next;
            return true;
        }

        const T& Current() const override
        {
            return current->data;
        }
    };

public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    LinkedList(const T* array, int count) : head(nullptr), tail(nullptr), size(0)
    {
        if (count < 0)
        {
            throw std::out_of_range("Negative size");
        }

        if (array == nullptr && count > 0)
        {
            throw std::invalid_argument("Source array is null");
        }

        for (int i = 0; i < count; i++)
        {
            Append(array[i]);
        }
    }

    LinkedList(const LinkedList<T>& other) : head(nullptr), tail(nullptr), size(0)
    {
        Node<T>* current = other.head;
        while (current != nullptr)
        {
            Append(current->data);
            current = current->next;
        }
    }

    ~LinkedList()
    {
        Clear();
    }

    LinkedList<T>& operator=(LinkedList<T> other)
    {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(size, other.size);
        return *this;
    }

    const T& GetFirst() const
    {
        if (size == 0)
        {
            throw std::out_of_range("List is empty");
        }

        return head->data;
    }

    const T& GetLast() const
    {
        if (size == 0)
        {
            throw std::out_of_range("List is empty");
        }

        return tail->data;
    }

    const T& Get(int index) const
    {
        return GetNode(index)->data;
    }

    T& Get(int index)
    {
        return GetNode(index)->data;
    }

    int GetSize() const
    {
        return size;
    }

    LinkedList<T>& Append(const T& item)
    {
        Node<T>* new_node = new Node<T>(item);

        if (size == 0)
        {
            head = new_node;
            tail = new_node;
        }
        else
        {
            tail->next = new_node;
            tail = new_node;
        }

        size++;
        return *this;
    }

    LinkedList<T>& Prepend(const T& item)
    {
        Node<T>* new_node = new Node<T>(item, head);
        head = new_node;

        if (size == 0)
        {
            tail = new_node;
        }

        size++;
        return *this;
    }

    LinkedList<T>& InsertAt(int index, const T& item)
    {
        if (index < 0 || index > size)
        {
            throw std::out_of_range("Index out of range");
        }

        if (index == 0)
        {
            Prepend(item);
            return *this;
        }

        if (index == size)
        {
            Append(item);
            return *this;
        }

        Node<T>* previous = GetNode(index - 1);
        Node<T>* new_node = new Node<T>(item, previous->next);
        previous->next = new_node;
        size++;
        return *this;
    }

    LinkedList<T> GetSubList(int start_index, int end_index) const
    {
        if (start_index < 0 || end_index < 0 || start_index >= size || end_index >= size || start_index > end_index)
        {
            throw std::out_of_range("Index out of range");
        }

        LinkedList<T> result;
        Node<T>* current = GetNode(start_index);

        for (int i = start_index; i <= end_index; i++)
        {
            result.Append(current->data);
            current = current->next;
        }

        return result;
    }

    LinkedList<T> Concat(const LinkedList<T>& other) const
    {
        LinkedList<T> result(*this);
        Node<T>* current = other.head;

        while (current != nullptr)
        {
            result.Append(current->data);
            current = current->next;
        }

        return result;
    }

    IEnumerator<T>* GetEnumerator() const
    {
        return new ListEnumerator(head);
    }
};

#endif // LINKED_LIST_H
