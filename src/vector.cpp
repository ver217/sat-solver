#include "../include/vector.h"
#include <stdlib.h>
#include <string.h>

template <typename T>
Vector<T>::Vector() : _size(0), _capacity(INIT_CAPACITY), data(NULL) {
    data = new T[_capacity];
}

template <typename T>
Vector<T>::Vector(size_t n) : _size(n), _capacity(n + INCREMENT), data(NULL) {
    data = new T[_capacity];
}

template <typename T>
Vector<T>::Vector(const Vector<T>& v) : _size(v.size()), _capacity(v.capacity()), data(NULL) {
    data = new T[_capacity];
    memcpy(data, v.data, sizeof(T) * _capacity);
}

template <typename T>
Vector<T>::~Vector() {};

template <typename T>
void Vector<T>::release() {
    if (data) {
        delete []data;
        data = NULL;
    }
}

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
void Vector<T>::push_back(const T& x) {
    if (_size + 1 >= _capacity) {
        _capacity += INCREMENT;
        data = (T*)realloc(data, _capacity * sizeof(T));
    }
    data[_size - 1] = x;
    _size++;
}

template class Vector<int>;
