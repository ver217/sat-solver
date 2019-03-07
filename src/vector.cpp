#include "../include/vector.h"
#include <stdlib.h>
#include <string.h>

template <typename T>
Vector<T>::Vector() : _size(0), _capacity(INIT_CAPACITY), data(nullptr) {
    data = new T[_capacity];
    memset(data, 0, sizeof(T) * _capacity);
}

template <typename T>
Vector<T>::Vector(size_t n) : _size(n), _capacity(n), data(nullptr) {
    data = new T[_capacity];
    memset(data, 0, sizeof(T) * _capacity);
}

template <typename T>
Vector<T>::Vector(const Vector<T>& v) : _size(v.size()), _capacity(v.capacity()), data(nullptr) {
    data = new T[_capacity];
    memcpy(data, v.data, sizeof(T) * _capacity);
}

template <typename T>
Vector<T>::~Vector() {
    if (data) {
        delete []data;
        data = nullptr;
    }
};

template <typename T>
size_t Vector<T>::size() const {
    return _size;
}

template <typename T>
size_t Vector<T>::capacity() const {
    return _capacity;
}

template <typename T>
T& Vector<T>::operator[](size_t idx) {
    return data[idx];
}

template <typename T>
const T& Vector<T>::operator[](size_t idx) const {
    return data[idx];
}

template <typename T>
void Vector<T>::push_back(const T& x) {
    if (_size >= _capacity) {
        _capacity += INCREMENT;
        data = (T*)realloc(data, _capacity * sizeof(T));
    }
    data[_size++] = x;
}

template <typename T>
const T& Vector<T>::pop_back() {
    if (_size > 0)
        return data[--_size];
    else
        throw "vector pop out of range";
}

template <typename T>
size_t Vector<T>::find(const T& x) const {
    for (size_t i = 0; i < _size; i++)
        if (x == data[i])
            return i;
    return npos;
}

template <typename T>
void Vector<T>::reserve(size_t n) {
    if (_capacity < n) {
        _capacity = n;
        data = (T*)realloc(data, sizeof(T) * _capacity);
    }
}


template class Vector<int>;
template class Vector<size_t>;
