#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <algorithm>

template <typename T>
class MyVector {
private:
    T* data;
    size_t sz;
    size_t cap;

    void reallocate(size_t new_capacity) {
        if (new_capacity <= cap && new_capacity >= sz)
            return;

        if (new_capacity == 0)
            new_capacity = 1;

        T* new_data = new T[new_capacity];

        for (size_t i = 0; i < sz; ++i) {
            new_data[i] = std::move(data[i]);
        }

        delete[] data;
        data = new_data;
        cap = new_capacity;
    }

public:
    MyVector() : data(nullptr), sz(0), cap(0) {}

    MyVector(int n, const T& value) : data(nullptr), sz(0), cap(0) {
        if (n > 0) {
            data = new T[n];
            for (int i = 0; i < n; ++i) {
                data[i] = value;
            }
            sz = cap = n;
        }
    }

    ~MyVector() {
        delete[] data;
    }

    MyVector(const MyVector& other) : data(nullptr), sz(0), cap(0) {
        reallocate(other.sz);
        sz = other.sz;
        for (size_t i = 0; i < sz; ++i) {
            data[i] = other.data[i];
        }
    }

    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            delete[] data;
            data = nullptr;
            sz = 0;
            cap = 0;
            reallocate(other.sz);
            sz = other.sz;
            for (size_t i = 0; i < sz; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    MyVector(MyVector&& other) noexcept : data(other.data), sz(other.sz), cap(other.cap) {
        other.data = nullptr;
        other.sz = 0;
        other.cap = 0;
    }

    MyVector& operator=(MyVector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            sz = other.sz;
            cap = other.cap;
            other.data = nullptr;
            other.sz = 0;
            other.cap = 0;
        }
        return *this;
    }

    void push_back(const T& value) {
        if (sz == cap) reallocate(cap == 0 ? 1 : cap * 2);
        data[sz++] = value;
    }

    void push_back(T&& value) {
        if (sz == cap) reallocate(cap == 0 ? 1 : cap * 2);
        data[sz++] = std::move(value);
    }

    template <typename... Args>
    void emplace_back(Args&&... args) {
        if (sz == cap) reallocate(cap == 0 ? 1 : cap * 2);
        data[sz++] = T(std::forward<Args>(args)...);
    }

    void pop_back() {
        if (sz == 0) throw std::out_of_range("pop_back on empty vector");
        --sz;
    }

    void resize(size_t n, const T& value) {
        if (n > cap) reallocate(n);
        for (size_t i = sz; i < n; ++i) {
            data[i] = value;
        }
        sz = n;
    }

    void resize(size_t new_size) {
        if (new_size > cap) reallocate(new_size);
        for (size_t i = sz; i < new_size; ++i) {
            data[i] = T();
        }
        sz = new_size;
    }

    void reserve(size_t new_cap) {
        if (new_cap > cap) reallocate(new_cap);
    }

    void shrink_to_fit() {
        if (cap > sz) reallocate(sz);
    }

    void clear() {
        sz = 0;
    }

    void erase(size_t index) {
        if (index >= sz) throw std::out_of_range("erase() index out of range");
        for (size_t i = index; i < sz - 1; ++i) {
            data[i] = std::move(data[i + 1]);
        }
        --sz;
    }

    void erase(T* position) {
        if (position < data || position >= data + sz)
            throw std::out_of_range("erase(pointer) out of range");
        erase(static_cast<size_t>(position - data));
    }

    void insert(size_t index, const T& value) {
        if (index > sz) throw std::out_of_range("insert index out of range");
        if (sz == cap) reallocate(cap == 0 ? 1 : cap * 2);
        for (size_t i = sz; i > index; --i) {
            data[i] = std::move(data[i - 1]);
        }
        data[index] = value;
        ++sz;
    }

    void swap(MyVector& other) noexcept {
        std::swap(data, other.data);
        std::swap(sz, other.sz);
        std::swap(cap, other.cap);
    }

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    T& at(size_t index) {
        if (index >= sz) throw std::out_of_range("at() index out of range");
        return data[index];
    }

    const T& at(size_t index) const {
        if (index >= sz) throw std::out_of_range("at() index out of range");
        return data[index];
    }

    T& front() {
        if (sz == 0) throw std::out_of_range("front() on empty vector");
        return data[0];
    }

    const T& front() const {
        if (sz == 0) throw std::out_of_range("front() on empty vector");
        return data[0];
    }

    T& back() {
        if (sz == 0) throw std::out_of_range("back() on empty vector");
        return data[sz - 1];
    }

    const T& back() const {
        if (sz == 0) throw std::out_of_range("back() on empty vector");
        return data[sz - 1];
    }

    size_t size() const { return sz; }
    size_t capacity() const { return cap; }
    bool empty() const { return sz == 0; }

    T* begin() { return data; }
    const T* begin() const { return data; }
    T* end() { return data + sz; }
    const T* end() const { return data + sz; }

    // NUEVO: función for_each como en Set
    template <typename Func>
    void for_each(Func func) const {
        for (size_t i = 0; i < sz; ++i) {
            func(data[i]);
        }
    }
};

#endif // MY_VECTOR_H
