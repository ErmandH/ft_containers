#pragma once

#include "vector.hpp"

namespace ft
{
	template <typename T, typename Container = ft::vector<T> > class stack
	{
		public:
			typedef T			value_type;
			typedef Container	container_type;
			typedef size_t		size_type;
		
		protected:
			container_type c;
/***************************** CONSTRUCTORS **************************************************/
		public:
			explicit stack (const container_type& ctnr = container_type())
			: c(ctnr)
			{}

			stack(const stack &rhs) : c(rhs.c)
			{}

			stack& operator=(const stack &rhs)
			{
				if (this != &rhs)
					c = rhs.c;
				return *this;
			}

			~stack()
			{}
/***************************** PUBLIC FUNCTIONS *********************************************/
		public:
			bool empty() const{
				return c.empty();
			}

			size_type size() const{
				return c.size();
			}

			value_type& top()
			{
				return c.back();
			}
			const value_type& top() const{
				return c.back();
			}

			void push (const value_type& val){
				c.push_back(val);
			}

			void pop(){
				c.pop_back();
			}
			// < ve == containera erisebilsin diye
			template <typename T1, typename C1>
			friend bool operator== (const stack<T1,C1>& lhs, const stack<T1,C1>& rhs);

    		template <typename T1, typename C1>
			friend bool operator< (const stack<T1,C1>& lhs, const stack<T1,C1>& rhs);
	};// end of stack

template <typename T1, typename C1>
inline  bool operator== (const stack<T1,C1>& lhs, const stack<T1,C1>& rhs){
	return lhs.c == rhs.c;
}
	
template <typename T1, typename C1>
inline  bool operator!= (const stack<T1, C1>& lhs, const stack<T1, C1>& rhs){
	return !(lhs == rhs);
}
	
template <typename T1, typename C1>
inline  bool operator<  (const stack<T1, C1>& lhs, const stack<T1, C1>& rhs){
	return lhs.c < rhs.c;
}
	
template <typename T1, typename C1>
inline  bool operator<= (const stack<T1, C1>& lhs, const stack<T1, C1>& rhs){
	return !(rhs < lhs);
}
	
template <typename T1, typename C1>
inline  bool operator>  (const stack<T1, C1>& lhs, const stack<T1, C1>& rhs){
	return rhs < lhs;
}
	
template <typename T1, typename C1>
inline bool operator>= (const stack<T1, C1>& lhs, const stack<T1, C1>& rhs){
	return !(lhs < rhs);
}
} // end of namesapce

	

