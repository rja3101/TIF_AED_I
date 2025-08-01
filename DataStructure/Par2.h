#ifndef PAR2_H
#define PAR2_H

template <typename A, typename B>
struct Par2 {
    A primero;
    B segundo;

    Par2() : primero(), segundo() {}
    Par2(A a, B b) : primero(a), segundo(b) {}

    // Para comparar en Set: primero por 'primero', luego por 'segundo'
    bool operator<(const Par2& other) const {
        if (primero != other.primero)
            return primero < other.primero;
        return segundo < other.segundo;
    }

    bool operator==(const Par2& other) const {
        return primero == other.primero && segundo == other.segundo;
    }
};

#endif
