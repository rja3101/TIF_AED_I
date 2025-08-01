#ifndef QUEUE_H
#define QUEUE_H

template <typename T>
struct NodoQueue {
    T valor;
    NodoQueue* siguiente;
    NodoQueue(T val) : valor(val), siguiente(nullptr) {}
};

template <typename T>
class Queue {
private:
    NodoQueue<T>* frente;
    NodoQueue<T>* final;

public:
    Queue() : frente(nullptr), final(nullptr) {}

    ~Queue() {
        while (!empty()) pop();
    }

    void push(const T& val) {
        NodoQueue<T>* nuevo = new NodoQueue<T>(val);
        if (final) final->siguiente = nuevo;
        final = nuevo;
        if (!frente) frente = nuevo;
    }

    void pop() {
        if (!frente) return;
        NodoQueue<T>* temp = frente;
        frente = frente->siguiente;
        if (!frente) final = nullptr;
        delete temp;
    }

    T& front() {
        return frente->valor;
    }

    bool empty() const {
        return frente == nullptr;
    }
};

#endif
