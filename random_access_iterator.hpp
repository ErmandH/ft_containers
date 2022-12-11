# ifndef RANDOM_ACCESS_ITERATOR_HPP
# define RANDOM_ACCESS_ITERATOR_HPP
# include "iterator.hpp"

namespace ft
{
	template <typename It, typename Container>
	class random_access_iterator
	{
		public:
			// pointerla ayni aslinda It
			typedef It												iterator_type;
			typedef typename iterator_traits<It>::iterator_category	iterator_category;
			typedef typename iterator_traits<It>::value_type		value_type;
			typedef typename iterator_traits<It>::pointer			pointer;
			typedef typename iterator_traits<It>::reference			reference;
			typedef typename iterator_traits<It>::difference_type	difference_type;
		
		// member value pointer ile esit aslinda
		protected:
			It _ptr;
		// constructor destructor copy assignment
		public:
			random_access_iterator() : _ptr(iterator_type())
			{}

			// adam it = some_it seklinde yapamasin sadece it(some_iter) seklinde yapabilsin
			explicit random_access_iterator(iterator_type const &it) : _ptr(it)
			{}

			/*
				Eger gelen Iter bizim Container ımızın pointerı ile eşitse is_same::value true olacak ve enable_if te aktifleşeceği için enable_if::type çalışacak ancak false gelirse type olmayacağı için fonksiyon discard edilecek (SFIANE) bunu zaten enable_if te de açıkladım. Type ımızda zaten Container olucağı için ikinci parametreye Container almis oluyo haliyle	
			*/
			template <typename Iter>
    		random_access_iterator(const random_access_iterator<
                    Iter, typename enable_if<is_same<Iter, typename Container::pointer>::value,
                                             Container>::type>& it)
        	: _ptr(it.base())
			{}

			~random_access_iterator(){}

			random_access_iterator & operator=(random_access_iterator const &rhs)
			{
				if (this != &rhs)
					_ptr = rhs._ptr;
				return *this;
			}
		// (*, ->, ++, --, +=,  -=, +, -, [])  operator overload
		public:
			// pointera erismemizi saglayan fonksiyon
			const iterator_type&	base() const
			{
				return _ptr;
			}
			reference	operator*()
			{
				return *_ptr;
			}
			// bu int icin bi anlam ifade etmiyo ama mesela bi class oldugunu dusundugunde it->x dedigimde hangi class pointeri ise onun elemanlarina erisebilecek
			pointer		operator->()
			{
				return _ptr;
			}
			// prefix
			random_access_iterator	&operator++()
			{
				++_ptr;
				return *this;
			}
			// postfix
			random_access_iterator	operator++(int)
			{
				
				return random_access_iterator(_ptr++);
			}
			// prefix
			random_access_iterator	&operator--()
			{
				--_ptr;
				return *this;
			}
			// postfix
			random_access_iterator	operator--(int)
			{
				return random_access_iterator(_ptr--);
			}
			random_access_iterator operator+(difference_type n)
			{
				return random_access_iterator(_ptr + n);
			}
			random_access_iterator operator-(difference_type n)
			{
				return random_access_iterator(_ptr - n);
			}
			// bunlarda assignment sonucta o yuzden referans donuyo
			random_access_iterator & operator+=(difference_type n)
			{
				_ptr += n;
				return *this;
			}
			random_access_iterator & operator-=(difference_type n)
			{
				_ptr -= n;
				return *this;
			}
			reference	operator[](difference_type n)
			{
				return *(_ptr + n);
			}
	}; // end of random_access_iterator

	/********** KARSILASTIRMA OPERATORLERI ***********/

	// iterator left, iterator right, container
	template <typename ItL, typename ItR, typename Container>
	inline bool operator==(const random_access_iterator<ItL, Container>&lhs,
						   const random_access_iterator<ItR, Container>&rhs)
	{
		return lhs.base() == rhs.base();
	}

	// ayni tur pointerlar icin
	template <typename It, typename Container>
	inline bool operator==(const random_access_iterator<It, Container>&lhs,
						   const random_access_iterator<It, Container>&rhs)
	{
		return lhs.base() == rhs.base();
	}

	template <typename ItL, typename ItR, typename Container>
	inline bool operator!=(const random_access_iterator<ItL, Container>&lhs,
						   const random_access_iterator<ItR, Container>&rhs)
	{
		return lhs.base() != rhs.base();
	}

	template <typename It, typename Container>
	inline bool operator!=(const random_access_iterator<It, Container>&lhs,
						   const random_access_iterator<It, Container>&rhs)
	{
		return lhs.base() != rhs.base();
	}

	template <typename ItL, typename ItR, typename Container>
	inline bool operator<(const random_access_iterator<ItL, Container>&lhs,
						   const random_access_iterator<ItR, Container>&rhs)
	{
		return lhs.base() < rhs.base();
	}

	template <typename It, typename Container>
	inline bool operator<(const random_access_iterator<It, Container>&lhs,
						   const random_access_iterator<It, Container>&rhs)
	{
		return lhs.base() < rhs.base();
	}

	template <typename ItL, typename ItR, typename Container>
	inline bool operator<=(const random_access_iterator<ItL, Container>&lhs,
						   const random_access_iterator<ItR, Container>&rhs)
	{
		return lhs.base() <= rhs.base();
	}

	template <typename It, typename Container>
	inline bool operator<=(const random_access_iterator<It, Container>&lhs,
						   const random_access_iterator<It, Container>&rhs)
	{
		return lhs.base() <= rhs.base();
	}

	template <typename ItL, typename ItR, typename Container>
	inline bool operator>(const random_access_iterator<ItL, Container>&lhs,
						   const random_access_iterator<ItR, Container>&rhs)
	{
		return lhs.base() > rhs.base();
	}

	template <typename It, typename Container>
	inline bool operator>(const random_access_iterator<It, Container>&lhs,
						   const random_access_iterator<It, Container>&rhs)
	{
		return lhs.base() > rhs.base();
	}

	template <typename ItL, typename ItR, typename Container>
	inline bool operator>=(const random_access_iterator<ItL, Container>&lhs,
						   const random_access_iterator<ItR, Container>&rhs)
	{
		return lhs.base() >= rhs.base();
	}

	template <typename It, typename Container>
	inline bool operator>=(const random_access_iterator<It, Container>&lhs,
						   const random_access_iterator<It, Container>&rhs)
	{
		return lhs.base() >= rhs.base();
	}

	/*********** TOPLAMA CIKARMA OPERATORLERI iteratorler arasinda **********/
	template <typename ItL, typename ItR, typename Container>
	inline typename random_access_iterator<ItL, Container>::difference_type 
	operator-(const random_access_iterator<ItL, Container>&lhs,const random_access_iterator<ItR, Container>&rhs)
	{
		return lhs.base() - rhs.base();
	}

	template <typename It, typename Container>
	inline typename random_access_iterator<It, Container>::difference_type
	operator-(const random_access_iterator<It, Container>&lhs, const random_access_iterator<It, Container>&rhs)
	{
		return lhs.base() - rhs.base();
	}


	// class in icinde iter + n var ama n + iter yok onu yapiyorum
	template <typename It, typename Container>
	inline random_access_iterator<It, Container>
	operator+(typename random_access_iterator<It, Container>::difference_type n,
          const random_access_iterator<It, Container>& it)
	{
    	return random_access_iterator<It, Container>(it.base() + n);
	}

} // end of namespace ft

#endif