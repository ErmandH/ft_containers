#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

namespace ft
{

/*
    Eger parametre true ise typedef type ozelligi olucak ancak false ise type olmayacagi icin program hata vermicek sadece discard edicek fonksiyonu ve bu durumda aslinda sadece enable_if in ilk parametresi true ise program calisacak (SFINAE)
*/
template <bool B, typename T = void>
struct enable_if {
};

template <typename T>
struct enable_if<true, T> {
    typedef T type;
};

struct true_type {
    static const bool value = true;
    typedef true_type type;
};

struct false_type {
    static const bool value = false;
    typedef false_type type;
};

/* integral tip olanlar disindaki her sey false_type alicak */
template <typename>
struct is_integral : public false_type {
};

template <>
struct is_integral<bool> : public true_type {
};

template <>
struct is_integral<char> : public true_type {
};

template <>
struct is_integral<signed char> : public true_type {
};

template <>
struct is_integral<short int> : public true_type {
};

template <>
struct is_integral<int> : public true_type {
};

template <>
struct is_integral<long int> : public true_type {
};

template <>
struct is_integral<unsigned char> : public true_type {
};

template <>
struct is_integral<unsigned short int> : public true_type {
};

template <>
struct is_integral<unsigned int> : public true_type {
};

template <>
struct is_integral<unsigned long int> : public true_type {
};

template <typename T, typename U>
struct is_same : public false_type {
};

template <typename T>
struct is_same<T, T> : public true_type {
};
} // namespace ending

#endif