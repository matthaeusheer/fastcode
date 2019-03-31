#ifndef FASTCODE_UTILS_H
#define FASTCODE_UTILS_H

/**
 * Python inspired [range(start, stop, step)] function.
 *
 * @tparam T type of the iterator
 * @tparam iBegin start value
 * @tparam iEnd end value
 * @tparam iStep step value
 */
template <typename T,T iBegin,T iEnd,T iStep=1>
class range {
public:
    struct iterator {
        T value;
        iterator    (T v) : value(v) {}
        operator T  () const         { return value; }
        operator T& ()               { return value; }
        T operator* () const         { return value; }
        iterator& operator++ ()      { value += iStep; return *this; }
    };
    iterator begin() { return iBegin; }
    iterator end() { return iEnd; }
};

#endif //FASTCODE_UTILS_H
