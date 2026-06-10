#ifndef IENUMERATOR_H
#define IENUMERATOR_H

template <typename T> 
class IEnumerator
{
public:
    virtual ~IEnumerator() = default;
    virtual bool MoveNext() = 0;
    virtual const T& Current() const = 0;
};

#endif // IENUMERATOR_H