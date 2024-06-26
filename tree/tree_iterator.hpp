/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_iterator.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eharuni < eharuni@student.42istanbul.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 17:37:17 by eharuni           #+#    #+#             */
/*   Updated: 2022/06/25 17:21:19 by eharuni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iterator>

#include "tree_algorithm.hpp"
#include "tree_types.hpp"

namespace ft
{
template <typename T, typename DiffType>
class const_tree_iterator;

template <typename T, typename DiffType>
class tree_iterator
{
public:
    typedef std::bidirectional_iterator_tag  iterator_category;
    typedef T                                value_type;
    typedef T&                               reference;
    typedef T*                               pointer;
    typedef DiffType                         difference_type;
    typedef const_tree_iterator<T, DiffType> const_iterator;

private:
    typedef typename tree_node_types<T>::end_node_pointer  end_node_pointer;
    typedef typename tree_node_types<T>::node_pointer      node_pointer;

public:
    tree_iterator()
        : ptr(NULL)
    {
    }

    tree_iterator(end_node_pointer p)
        : ptr(p)
    {
    }

    tree_iterator(node_pointer p)
        : ptr(static_cast<end_node_pointer>(p))
    {
    }

public:
    end_node_pointer& base()
    {
        return ptr;
    }

    const end_node_pointer& base() const
    {
        return ptr;
    }

    node_pointer node_ptr() const
    {
        return static_cast<node_pointer>(ptr);
    }

    // pairi donduruyo
    reference operator*() const
    {
        return static_cast<node_pointer>(ptr)->value;
    }

    // pair pointer donduruyo
    pointer operator->() const
    {
        return &(operator*());
    }

    // prefix increment
    tree_iterator& operator++()
    {
        ptr = tree_iter_next<end_node_pointer>(static_cast<node_pointer>(ptr));
        return *this;
    }

    // postfix increment
    tree_iterator operator++(int)
    {
        tree_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    // prefix decrement
    tree_iterator& operator--()
    {
        ptr = tree_iter_prev<node_pointer>(ptr);
        return *this;
    }

    // postfix decrement
    tree_iterator operator--(int)
    {
        tree_iterator t = *this;
        --(*this);
        return t;
    }

    bool operator==(const tree_iterator& other) const
    {
        return ptr == other.ptr;
    }

    bool operator==(const const_iterator& other) const
    {
        return ptr == other.base();
    }

    bool operator!=(const tree_iterator& other) const
    {
        return !(*this == other);
    }

    bool operator!=(const const_iterator& other) const
    {
        return !(*this == other);
    }

private:
    end_node_pointer ptr;
};

template <typename T, typename DiffType>
class const_tree_iterator
{

public:
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T                               value_type;
    typedef const T&                        reference;
    typedef const T*                        pointer;
    typedef DiffType                        difference_type;
    typedef tree_iterator<T, DiffType>      non_const_iterator;

private:
    typedef typename tree_node_types<T>::end_node_pointer  end_node_pointer;
    typedef typename tree_node_types<T>::node_pointer      node_pointer;


public:
    const_tree_iterator()
        : ptr(NULL)
    {
    }

    const_tree_iterator(end_node_pointer p)
        : ptr(p)
    {
    }

    const_tree_iterator(node_pointer p)
        : ptr(static_cast<end_node_pointer>(p))
    {
    }

    const_tree_iterator(non_const_iterator it)
        : ptr(it.base())
    {
    }

public:
    end_node_pointer& base()
    {
        return ptr;
    }

    const end_node_pointer& base() const
    {
        return ptr;
    }

    node_pointer node_ptr() const
    {
        return static_cast<node_pointer>(ptr);
    }

    reference operator*() const
    {
        return static_cast<node_pointer>(ptr)->value;
    }

    pointer operator->() const
    {
        return &(operator*());
    }

    const_tree_iterator& operator++()
    {
        ptr = tree_iter_next<end_node_pointer>(static_cast<node_pointer>(ptr));
        return *this;
    }

    const_tree_iterator operator++(int)
    {
        const_tree_iterator t = *this;
        ++(*this);
        return t;
    }

    const_tree_iterator& operator--()
    {
        ptr = tree_iter_prev<node_pointer>(ptr);
        return *this;
    }

    const_tree_iterator operator--(int)
    {
        const_tree_iterator t = *this;
        --(*this);
        return t;
    }

    bool operator==(const const_tree_iterator& other) const
    {
        return ptr == other.ptr;
    }

    bool operator==(const non_const_iterator& other) const
    {
        return ptr == other.base();
    }

    bool operator!=(const const_tree_iterator& other) const
    {
        return !(*this == other);
    }

    bool operator!=(const non_const_iterator& other) const
    {
        return !(*this == other);
    }

private:
    end_node_pointer ptr;
};

} // namespace ft
