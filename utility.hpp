#ifndef UTILITY_HPP
#define UTILITY_HPP
#include "type_traits.hpp"

namespace ft
{

template <typename T1, typename T2>
struct pair
{
	// member types
	typedef T1 first_type;
	typedef T2 second_type;

	// member objects
	first_type first;
	second_type second;

	pair() : first(first_type()), second(second_type())
	{}

	pair(const first_type &f, const second_type &s) : first(f), second(s)
	{}

	template< class U1, class U2 >
	pair( const pair<U1, U2>& p ) : first(p.first), second(p.second)
	{}

	pair(const pair& other): first(other.first), second(other.second)
    {}

	pair & operator=(const pair& other)
	{
		if (this != &other)
		{
			first = other.first;
			second = other.second;
		}	
		return *this;
	}
};

template< typename T1, typename T2 >
pair<T1,T2> make_pair( T1 x, T2 y )
{
	return (pair<T1,T2>(x, y));
}

template< typename T1, typename T2 >
inline bool operator==( const pair<T1,T2>& lhs, const pair<T1,T2>& rhs )
{
	return (lhs.first == rhs.first && lhs.second == rhs.second);
}

// == ten false donerse true donucek
template< typename T1, typename T2 >
inline bool operator!=( const pair<T1,T2>& lhs, const pair<T1,T2>& rhs )
{
	return !(lhs == rhs);
}

/*
	If lhs.first<rhs.first, returns true. Otherwise, if rhs.first<lhs.first, returns false. Otherwise, if lhs.second<rhs.second, returns true. Otherwise, returns false.
*/
template< typename T1, typename T2 >
inline bool operator<( const pair<T1,T2>& lhs, const pair<T1,T2>& rhs )
{
	return (lhs.first < rhs.first) || (!(rhs.first < lhs.first) && lhs.second < rhs.second);
}

template <typename T1, typename T2>
inline bool operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
    return !(rhs < lhs);
}

template <typename T1, typename T2>
inline bool operator>(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
{
    return rhs < lhs;
}


template< typename T1, typename T2 >
inline bool operator>=( const pair<T1,T2>& lhs, const pair<T1,T2>& rhs )
{
	return !(lhs < rhs);
}

// iteratorlerin icindeki degerler esit mi diye kontrol ediyo first1 den last1 e kadar
template<typename InputIt1, typename InputIt2>
bool equal(InputIt1 first1, InputIt1 last1, 
           InputIt2 first2)
{
    for (; first1 != last1; ++first1, (void)++first2) {
        if (!(*first1 == *first2)) {
            return false;
        }
    }
    return true;
}


// iteratorlerin icindeki degerler p fonksiyonuna gore esit olup olmadigini kontrol ediyo
template<typename InputIt1, typename InputIt2, typename BinaryPredicate>
bool equal(InputIt1 first1, InputIt1 last1, 
           InputIt2 first2, BinaryPredicate p)
{
    for (; first1 != last1; ++first1, (void)++first2) {
        if (!p(*first1, *first2)) {
            return false;
        }
    }
    return true;
}

// alfabetik siraya gore test ediyo eger ilk iteratorun elementi ikinci iteratorun elementinden kucuk ise o zaman true donecek esitse farkli gorene kadar devam edecek yani ilk dizi alfabetik siraya gore daha kucuk degilse false donuyor
// eger hepsi esitse ve donguden ciktiysa bu sefer iteratorun bitip bitmedigine bakiyorum eger ilk first1 iteratoru sona geldiyse ve first2 daha sona gelmediyse (last1 e gelemediyse) o zaman yine ilk iterator alfabetik olarak daha kucuk sayiliyor eger ikiside iteratorunun sonuna geldiyse ya da ilk first2 sona geldiyse o zaman ilk iterator alfabetik olarak daha buyuk ve false donuyor

template<typename InputIt1, typename InputIt2>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
                             InputIt2 first2, InputIt2 last2)
{
    for ( ; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2 ) {
        if (*first1 < *first2)
			return true;
        if (*first2 < *first1)
			return false;
    }
    return (first1 == last1) && (first2 != last2);
}

// burda da karsilastirma fonksiyonuna gore yapiyo std::less<> gibi verebilirim
template<typename InputIt1, typename InputIt2, typename Compare>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
                             InputIt2 first2, InputIt2 last2,
                             Compare comp)
{
    for ( ; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2 ) {
        if (comp(*first1, *first2))
			return true;
        if (comp(*first2, *first1))
			return false;
    }
    return (first1 == last1) && (first2 != last2);
}

} // namespace ft ending

#endif