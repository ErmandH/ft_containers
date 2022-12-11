/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eharuni < eharuni@student.42istanbul.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 00:07:48 by eharuni           #+#    #+#             */
/*   Updated: 2022/06/18 00:07:48 by eharuni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "type_traits.hpp"
#include "iterator.hpp"
#include "random_access_iterator.hpp"
#include "utility.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <iterator>

namespace ft
{
template <typename T, typename Alloc = std::allocator<T> >
class vector
{
/***************************** MEMBER TYPES ******************************************************/
	public:
		typedef T													value_type;
		typedef Alloc												allocator_type;
		typedef typename allocator_type::reference					reference;
		typedef typename allocator_type::const_reference			const_reference;
		typedef typename allocator_type::pointer					pointer;
		typedef typename allocator_type::const_pointer				const_pointer;
		typedef random_access_iterator<pointer, vector>				iterator;
		typedef random_access_iterator<const_pointer, vector>		const_iterator;	
		typedef typename allocator_type::size_type					size_type;
		typedef typename allocator_type::difference_type			difference_type;
		typedef ft::reverse_iterator<iterator>						reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>				const_reverse_iterator;

/***************************** MEMBER VALUES ************************************************/
	private:
		allocator_type _alloc;
		pointer			_start; // dizinin baslangici
		pointer			_end; // dizinin son elemani + 1
		pointer			_end_cap; // dizinin kapasitesinin son pointeri

/*************************** CONSTRUCTORS AND DESTRUCTOR ***********************************/
	public:
	
		vector() 
		: _alloc(allocator_type()), _start(NULL), _end(NULL), _end_cap(NULL)
		{}

		vector(const vector& other)
        : _alloc(other._alloc),
          _start(NULL),
          _end(NULL),
          _end_cap(NULL)
   		{
			const size_type cap = other.capacity();
			if (cap == 0) {
				return;
			}
			_start = _alloc.allocate(cap);
			_end_cap = _start + cap;
			_end = construct_range(_start, other._start, other._end);
    	}

		explicit vector( const Alloc& alloc ) 
		: _alloc(alloc), _start(NULL),_end(NULL), _end_cap(NULL)
		{}

		explicit vector(size_type count, 
					const value_type& value = value_type(),
                    const allocator_type& alloc = allocator_type())
		: _alloc(alloc), _start(NULL), _end(NULL), _end_cap(NULL)
		{
			if (count == 0)
				return ;
			check_size(count);

			_start = _alloc.allocate(count);
			_end = _start + count;
			_end_cap = _end;
			construct_range(_start, _end, value);
		}

		// https://en.cppreference.com/w/cpp/container/vector/vector
		// integral degiskenler olmamasi gerek inputun ve InputIteratoru saglamali
		// eger integral  true ise ancak enable_if aktiflesip calisacak
		template< class InputIt >
		vector( InputIt first, typename enable_if<!is_integral<InputIt>::value, InputIt>::type last,
        const Alloc& alloc = Alloc() )
		: _alloc(alloc), _start(NULL), _end(NULL), _end_cap(NULL)
		{
			// categorysi neyse onun kategorisini yolluyorum
			typedef typename iterator_traits<InputIt>::iterator_category category;
			range_init(first, last, category());	
		}

		~vector()
		{
			deallocate_vector();		
		}
		vector & operator=(const vector& rhs)
		{
			if (this != &rhs)
				assign(rhs.begin(), rhs.end());

			return *this;
		}

/******************************* PRIVATE MEMBER FUNCTIONS *******************************************/
	private:
		// start ve end iteratorleri arasindaki degerleri dst ye construct ediyorum
		template<typename It>
		pointer	construct_range(pointer dst, It start, It end)
		{
			for (; start != end; ++dst, (void)++start)
				_alloc.construct(dst, *start);

			return dst;
		}

		// dst ile end arasinda value yi construct ediyorum
		pointer construct_range(pointer dst, const_pointer end, const_reference value)
		{
			for (; dst != end; ++dst)
				_alloc.construct(dst, value);
			
			return dst;
		}

		template <typename InputIt>
		void range_init(InputIt first, InputIt last, std::input_iterator_tag)
		{
			for (; first != last; ++first) {
				push_back(*first);
			}
		}

		// forward iterator => hem OutputIterator hem de InputIteratoru sagliyo
		template <typename ForwardIt>
		void range_init(ForwardIt first, ForwardIt last, std::forward_iterator_tag)
		{
			const size_type count = std::distance(first, last);
			if (count == 0) {
				return;
			}
			check_size(count);

			_start = _alloc.allocate(count);
			_end_cap = _start + count;
			_end = construct_range(_start, first, last);
		}
		/*
			- Eger size + n > capacity() * 2 ise yeni kapasite size() + n olucak
			- capacity() * 2 den kucuk ise yeni kapasite capacity() * 2 olucak (satir 410-413)
		*/
		template <typename InputIt>
		void range_insert(iterator pos, InputIt first, InputIt last, std::input_iterator_tag)
		{
			if (pos == end()) {
				for (; first != last; ++first) {
					push_back(*first);
				}
			} else if (first != last) {
				vector tmp(first, last);
				insert(pos, tmp.begin(), tmp.end());
			}
		}

		template <typename ForwardIt>
		void range_insert(iterator pos, ForwardIt first, ForwardIt last, std::forward_iterator_tag)
		{
			if (first != last) {
				const size_type count = std::distance(first, last);
				check_size(count);
				const size_type extra_space = _end_cap - _end;

				if (extra_space >= count) {
					const size_type elems_after = end() - pos;
					pointer old_end = _end;

					if (elems_after > count) {
						_end = construct_range(_end, _end - count, _end);
						std::copy_backward(pos.base(), old_end - count, old_end);
						std::copy(first, last, pos);
					} else {
						ForwardIt mid = first;
						std::advance(mid, elems_after);
						_end = construct_range(_end, mid, last);
						_end = construct_range(_end, pos.base(), old_end);
						std::copy(first, mid, pos);
					}
				} else {
					const size_type new_size = calculate_growth(count);
					pointer new_start = _alloc.allocate(new_size);
					pointer new_end = new_start;

					new_end = construct_range(new_start, _start, pos.base());
					new_end = construct_range(new_end, first, last);
					new_end = construct_range(new_end, pos.base(), _end);

					deallocate_vector();
					_start = new_start;
					_end = new_end;
					_end_cap = new_start + new_size;
				}
			}
		}


	/*
		ForwardIterator le aradaki fark InputIteratorler sadece okundugu icin algoritma fonksiyonlarina yollanmiyo tam o yuzden farkli yapiyorum ikisini
	*/
		template <typename InputIt>
		void range_assign(InputIt first, InputIt last, std::input_iterator_tag)
		{
			clear();
			for (; first != last; ++first)
				push_back(*first);

		}

		template <typename ForwardIt>
		void range_assign(ForwardIt first, ForwardIt last, std::forward_iterator_tag)
		{
			const size_type count = std::distance(first, last);

			if (count < size()) 
			{
				iterator it = std::copy(first, last, begin());
				erase_at_end(it.base());
			} 
			else 
			{
				ForwardIt it = first;
				std::advance(it, size());
				std::copy(first, it, begin());
				insert(end(), it, last);
			}
		}

		/* Eger size() + eklenecek eleman sayisi kapasitenin 2 katindan fazlaysa yeni kapasitem size() + extra oluyo burda da bunu hesapliyorum */
		size_type calculate_growth(size_type extra) const
    	{
			const size_type max = max_size();
			const size_type cap = capacity();
			if (max - cap < extra) {
				length_exception();
			}

			// kapasite max / 2 den buyukse cap * 2 maxi gecicegi icin max i donduruyorum
			if (cap >= max / 2) {
				return max;
			}

			return std::max(size() + extra, cap * 2);
    	}

		void destroy_range(pointer pos, pointer end)
		{
			for (; pos != end; ++pos)
				_alloc.destroy(pos);
		}

		void erase_at_end(pointer pos)
    	{
			destroy_range(pos, _end);
			_end = pos; // pos tan end e kadar olan kismi siliyo yeni endim pos oluyo
    	}

		void	length_exception() const
		{
			throw std::length_error("cannot create ft::vector larger than max_size()");
		}

		// olusacak vector hafizada ayrilabilir max_size() dan buyuk mu kontrolu
		void	check_size(size_type count) const
		{
			if (count > _alloc.max_size())
				length_exception();
		}

		void	check_range(size_type pos) const
		{
			if (pos >= size())
				throw std::out_of_range("Index is out of vector range");
		}

		// end ile end_cap esitse kapasitem dolmus demek bu yuzden true donucek
		bool	should_grow()
		{
			return _end == _end_cap;
		}

		void	deallocate_vector()
		{
			if (_start != NULL)
			{
				pointer tmp = _start;
				for (; tmp != _end; ++tmp)
					_alloc.destroy(tmp);
				_alloc.deallocate(_start, capacity());
			}			
		}

/*********************** PUBLIC MEMBER FUNCTIONS *********************************************/
	public:
		/******** ITERATORS ***********/
		iterator begin(){ return iterator(_start);}
		const_iterator begin() const { return const_iterator(_start);}
		
		iterator end(){ return iterator(_end);}
		const_iterator end() const { return const_iterator(_end);}

		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }

		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
/******************************* ELEMENT ACCESS*******************************************/
		reference operator[]( size_type pos )
		{
			return *(_start + pos);
		}

		const_reference operator[]( size_type pos ) const
		{
			return *(_start + pos);
		}

		reference at(size_type pos)
		{
			check_range(pos);
			return (*this)[pos];
		}
		const_reference at(size_type pos) const
		{
			check_range(pos);
			return (*this)[pos];
		}

		// ilk elemana referans donduruyo
		reference front()
		{
			return *begin();
		}
		
		const_reference front() const
		{
			return *begin();
		}

		// son elemana referans
		reference back()
		{
			return *(end() - 1);
		}
		
		const_reference back() const
		{
			return *(end() - 1);
		}

		// pointera erisim
		pointer  data()
		{
			return _start;
		}

		const_pointer  data() const
		{
			return _start;
		}

/***************************************** CAPACITY *************************************************/
		bool empty() const
		{
			return _start == _end;
		}
		size_type size() const
		{
			return static_cast<size_type>(std::distance(begin(), end()));
		}
		// containerin alabilecegi max size i donduruyo
		size_type max_size() const
		{
			return std::min(_alloc.max_size(),
                        static_cast<size_type>(std::numeric_limits<difference_type>::max()));
		}

		// yeni kapasite kadar yer ayiriyo oncekini kopyalayip eskisini deallocate ediyo
		void reserve( size_type new_cap )
		{
			if (capacity() >= new_cap)
				return ;
			if (new_cap > max_size())
				length_exception();
			
			pointer new_start = _alloc.allocate(new_cap);
			pointer new_end = construct_range(new_start, _start, _end);
			deallocate_vector();
			_start = new_start;
			_end_cap = _start + new_cap;
			_end = new_end;
		}

		size_type capacity() const
		{
			return static_cast<size_type>(std::distance(begin(), const_iterator(_end_cap)));
		}

/*****************************ASSIGN AND GET_ALLOCATOR ************************************/
		allocator_type get_allocator() const
		{
			return _alloc;
		}
		// count = containerin yeni size i, value = ici value ile doldurulacak
		void assign( size_type count, const T& value )
		{
			// kapasiteden buyukse yeni vector olusturdum swap ile kendiminkiyle degistirdim
			if (count > capacity())
			{
				vector tmp(count, value);
				tmp.swap(*this);
			}
			// eger sizedan buyukse size a kadar olan kisimlari degistirdim geri kalan kismida construct ettim
			else if (count > size())
			{
				std::fill(begin(), end(), value);
           	 	const size_type extra = count - size();
            	_end = construct_range(_end, _end + extra, value);
			}
			else // eger sizedan da kucukse buraya gelicek
			{
				// mesela size = 5 count = 3, 3 elemani value yapiyo, it te 4. elemanin iteri oluyo
				pointer it = std::fill_n(_start, count, value);
				// 4. elemandan end e kadar siliyo
            	erase_at_end(it);
			}
			
		}
		template <typename InputIt>
		void assign(InputIt first, typename enable_if<!is_integral<InputIt>::value, InputIt>::type last)
		{
			typedef typename iterator_traits<InputIt>::iterator_category category;
			range_assign(first, last, category());
		}

/*********************************MODIFIERS***********************************************/
		void clear()
		{
			destroy_range(_start, _end);
			_end = _start;
		}

		/*
			
		*/
		iterator insert(iterator pos, const value_type& value)
		{
			const size_type index = pos - begin();

			insert(pos, 1, value);

			return iterator(_start + index);
		}

		/*
			- count = eklenecek eleman sayisi, pos = eklenmeye baslanacak pozisyon
			- Eger count bos alandan buyukse yani kapasiteye sigmiyosa else e gidiyo degilse if e giriyo
			- elems_after = yeri degistirilecek eleman sayisi
		*/
		void insert(iterator pos, size_type count, const value_type& value)
		{
			if (count != 0) {
				const size_type extra_space = _end_cap - _end;
				// 1 2 3 4 0 0 0 0 0 pos = begin() + 2  value = 12 extra = 3 count = 2
				if (extra_space >= count) {
					const size_type elems_after = end() - pos; // 4
					pointer old_end = _end;

					if (elems_after > count) {
						// endden sonrasini _end - counttan itibaren doldur
						_end = construct_range(_end, _end - count, _end);
						// geri kalan tasinacak elemanlarida copy backward ile tasidim
						std::copy_backward(pos.base(), old_end - count, old_end);
						// sonra postan itibaren count kadar elemanlarimi degistirdim
						std::fill_n(pos, count, value);
					} 
					else {
						// burda ta tasima islemi yapiyorum
						_end = construct_range(_end, _end + (count - elems_after), value);
						_end = construct_range(_end, pos.base(), old_end);
						std::fill(pos.base(), old_end, value);
					}
				} // end if
				else {
					// eger eleman sayisi kapasiteyi asiyosa yeni dizi olusturuyorum
					const size_type new_size = calculate_growth(count);
					pointer new_start = _alloc.allocate(new_size);
					pointer new_end;

					new_end = construct_range(new_start, _start, pos.base());
					new_end = construct_range(new_end, new_end + count, value);
					new_end = construct_range(new_end, pos.base(), _end);

					deallocate_vector();
					_start = new_start;
					_end = new_end;
					_end_cap = new_start + new_size;
				} // end else
			}
		}

		template <class InputIt>
		void insert(iterator pos, InputIt first,
					typename enable_if<!is_integral<InputIt>::value, InputIt>::type last)
		{
			typedef typename iterator_traits<InputIt>::iterator_category category;
			range_insert(pos, first, last, category());
		}

		void push_back (const value_type& val)
		{
			if (!should_grow()) {
            _alloc.construct(_end, val);
            ++_end;
			} else {
				insert(end(), val);
			}
		}

		void	pop_back()
		{
			erase_at_end(_end - 1);
		}


		iterator erase(iterator pos)
		{
			// son eleman degil ise pos + 1 den end() e kadar posa kopyala memcpy gibi
			if (pos + 1 != end()) {
				std::copy(pos + 1, end(), pos);
			}
			--_end;
			// eger son elemansa zaten sadece bir azaltip son elemani destryo edicem
			_alloc.destroy(_end);
			return pos;
		}

		/*
			- first ve firstten sonraki buutn pointerlar ve referanslar invalidated oluyo
			firstten oncesi ise sabit kalmak zorunda

			- dist => silinecek eleman sayisi
			- 510 da yaptigim kontrol => eger last in end e olan uzakligi dist ten kucukse end e kadar gidicem yoksa patlar kod onun kontrolunu yaptim
		*/
		iterator erase(iterator first, iterator last)
		{
			const difference_type dist = std::distance(first, last); // silinecek eleman sayisi
			const difference_type index = std::distance(begin(), first); // silinecek ilk elemanin indexini tutuyorum en son return etmek icin
			destroy_range(first.base(), last.base()); // first ile last arasindakileri destroy ettim

			/*
				- first ile last arasinda kac eleman sildiysem yerini last ve last + dist arasindakilerle doldurdum sonra last ile last + dist arasinida sildim
				- geriye last + dist ile end() arasinda olan elemanlar kaldi onlarida lasttan sonrasina kopyaladim ve last + dist ile end() arasindakileride destroy ettim
			*/
			if (std::distance(last, end()) > dist)
			{
				iterator it = last + dist;
				std::copy(last, it, first);
				destroy_range(last.base(), it.base());
				std::copy(it, end(), last);
				destroy_range(it.base(), end().base());			
			}
			else
			{
				// eger dist daha buyukse zaten sadece last ile end arasini kopyalamam yeterli
				std::copy(last, end(), first);
				destroy_range(last.base(), end().base());				
			}
			_end = _start + size() - static_cast<size_type>(dist);
			return iterator(begin() + index);
		}

		/*
			- n < size() ise yeni size n olucak ve end e kadar olanlar destroy edilecek
			- size() < n <= capacity() ise size dan n e kadar olan kisim value ile doldurulacak var olan degerler degistirilmicek, yeni size = n olacak
			- n > capacity() ise yeni alan acilacak eski degerler degerini koruyacak
		*/
		void resize (size_type count, value_type val = value_type())
		{
			const size_type len = size();
			if (count > len) {
				insert(end(), count - len, val);
			} else if (count < len) {
				erase_at_end(_start + count);
			}
		}
		
		void swap(vector& other)
    	{
			std::swap(_start, other._start);
			std::swap(_end, other._end);
			std::swap(_end_cap, other._end_cap);
   		}

};// end of vector class


template <typename T, typename Alloc>
inline bool operator== (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
{
	if (lhs.size() != rhs.size())
		return false;
	return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, typename Alloc>
inline  bool operator!= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
{
	return !(lhs == rhs);
}

template <typename T, typename Alloc>
inline  bool operator<  (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
{
	return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename Alloc>
inline  bool operator>  (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
{
	return rhs < lhs;
}

template <typename T, typename Alloc>
inline  bool operator<= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
{
	return !(rhs < lhs);
}


template <typename T, typename Alloc>
inline  bool operator>= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
{
	return !(lhs < rhs);
}

} // end of namespace ft

#endif