/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_algorithm.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eharuni < eharuni@student.42istanbul.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 23:01:54 by eharuni           #+#    #+#             */
/*   Updated: 2022/06/25 17:18:41 by eharuni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "tree_types.hpp"
#include <cstddef>

namespace ft
{
template <typename NodePtr>
inline bool tree_is_left_child(NodePtr ptr)
{
    return ptr == ptr->parent->left;
}

// Agacta surekli en saga gidersek NULL a kadar, en buyuk sayiyi bulmus oluruz
template <typename NodePtr>
NodePtr tree_max(NodePtr ptr)
{
    while (ptr->right != NULL) {
        ptr = ptr->right;
    }
    return ptr;
}

// Agacta surekli en sola gidersek NULL a kadar, en kucuk sayiyi bulmus oluruz
template <typename NodePtr>
NodePtr tree_min(NodePtr ptr)
{
    while (ptr->left != NULL) {
        ptr = ptr->left;
    }
    return ptr;
}

/*
- https://stackoverflow.com/questions/2942517/how-do-i-iterate-over-binary-tree

        13
      /    \
     8      17
    / \     / \
   1   11  15  25
   \           / \ 
   \          22 27   
    6

- Nodumuzun 17 de oldugunu varsayarsak siradaki iter demek 17 den buyuk en kucuk sayiyi bulmak demek bu yuzden eger sag node null degilse 17 den buyuk en kucuk sayi orda bi yerde olmali bu yuzden sagdaki nodedan itibaren en kucuk sayiyi tree_min() ile bul diyorum

- Eger sag node null ise bu sefer parentima bakmaliyim ve soyle bir durum var eger benim nodeum right child ise su anki nodeum parenttan buyuk olucagi icin benim sol cocuk bulana kadar parentlara bakmam gerek. Ornegin parentimda right child o zaman parentimin parentina bakmam gerek. Eger parentimin parentida right child ise onunda parentina bakicam ve o left child ise o zaman 7 den buyuk en kucuk sayi o olmus oluyo ve o nodeu donduruyorum
*/

template <typename IterPtr, typename NodePtr>
IterPtr tree_iter_next(NodePtr ptr)
{
    if (ptr->right != NULL) {
        return tree_min(ptr->right);
    }
    while (!tree_is_left_child(ptr)) {
        ptr = ptr->get_parent();
    }
    return ptr->parent; // eger node left child ise onun parenti benim siradaki nodeum olmus oluyo bu yuzden parent i dondurdum
}

/*
   - Bu sefer kendisinden kucuk en buyuk sayiya bakmam gerek aslinda oncekinin tam tersini yapiyorum
   - Eger sol node NULL degilse o subtreede su anki nodedan kucuk en buyuk sayi orda olucak bu yuzden sol tarafin maxini bul diyorum
   - Eger NULL ise bu sefer parentlardan hangisi right child ise onun parenti bana nodeumdan kucuk en buyuk sayiyi veriyor
*/

template <typename NodePtr, typename IterPtr>
IterPtr tree_iter_prev(IterPtr ptr)
{
    if (ptr->left != NULL) {
        return tree_max(ptr->left);
    }
    NodePtr nptr = static_cast<NodePtr>(ptr);
    while (tree_is_left_child(nptr)) {
        nptr = nptr->get_parent();
    }
    return nptr->parent;
}

// left rotate yapiyoruz 
/*
    node => 8
             \
              10
               \
                11 
    deki 8 oluyor
*/
template <typename NodePtr>
void tree_rotate_left(NodePtr node)
{
    NodePtr ptr = node->right;

    node->right = ptr->left;
    if (ptr->left != NULL) {
        ptr->left->set_parent(node);
    }
    ptr->left = node;
    ptr->parent = node->parent;
    if (tree_is_left_child(node)) {
        node->parent->left = ptr;
    } else {
        node->get_parent()->right = ptr;
    }
    node->set_parent(ptr);
}

/*
    node = 9
          /
         8
        /
       7 deki 9 oluyo

*/
template <typename NodePtr>
void tree_rotate_right(NodePtr node)
{
    NodePtr ptr = node->left;

    node->left = ptr->right;
    if (ptr->right != NULL) {
        ptr->right->set_parent(node);
    }
    ptr->right = node;
    ptr->parent = node->parent;
    if (tree_is_left_child(node)) {
        node->parent->left = ptr;
    } else {
        node->get_parent()->right = ptr;
    }
    node->set_parent(ptr);
}

template <typename NodePtr>
void tree_rotate_left(NodePtr& root, NodePtr node)
{
    // yeni rootum rootun sagindaki oluyo
    if (node == root) {
        root = node->right;
    }

    tree_rotate_left(node);
}

template <typename NodePtr>
void tree_rotate_right(NodePtr& root, NodePtr node)
{
    // yeni rootum rootun solundaki oluyo
    if (node == root) {
        root = node->left;
    }

    tree_rotate_right(node);
}

template <typename NodePtr>
inline bool tree_node_is_black(NodePtr node)
{
    if (node == NULL || node->is_black) {
        return true;
    }
    return false;
}




template <typename NodePtr>
void tree_insert_fix(NodePtr root, NodePtr z)
{
    z->is_black = z == root; // z roota esitse nodeum black olucak
    while (z != root && !z->get_parent()->is_black) {
        // z nin parenti sol cocuk mu sol cocuk ise amca sag cocuk olucak

        if (tree_is_left_child(z->get_parent())) { // parent sol cocuk ise
            NodePtr uncle = z->get_parent()->get_parent()->right;
            // eger unclue kirmizi ise sadece color flip yapilacak
            /*
                    RED
                   /    \
                BLACK    BLACK
                  /
                 Z          
                          
            */
            if (!tree_node_is_black(uncle)) { // case 1
                uncle->is_black = true;
                z = z->get_parent();
                z->is_black = true;
                z = z->get_parent();
                z->is_black = z == root; // uncle ve parent black  grandparentta red olucak ama root degilse tabi eger root ise root black kalicak
            } 
            /*
                - uncle siyah ve parent left child ise iki ihtimal var ya left-left ya da left-right
                - bunun icin once z nin right child olup olmadigina bakiyorum eger right child ise once left-rotate sonra right rotate yapmam gerekicek degilse left-left olucagi icin right rotate yapmam yetiyo

                -   grandparent
                    /          \        
                  parent       uncle
                       \
                        z
                => left-right rotate => parent etrafinda left rotate +  grandparnt etrafinda right rotate

                -   grandparent
                    /          \        => left-left => grandparent etrafinda right rotate yeterli
                  parent       uncle
                  /
                z

                - ROTATE SONRASI
                    BLACK (parent)
                   /     \
                Z(red)   RED (grandparent)
            */
            else 
            {
                if (!tree_is_left_child(z)) {
                    z = z->get_parent();
                    tree_rotate_left(z);
                }

                z = z->get_parent();
                z->is_black = true;
                z = z->get_parent();
                z->is_black = false;
                tree_rotate_right(z);
                return;
            }
        }
        else 
        { // parent sag cocuk ise usttekinin tam tersi yapiliyor
            NodePtr uncle = z->get_parent()->parent->left; // amca sol oluyo

            if (!tree_node_is_black(uncle)) {
                uncle->is_black = true;
                z = z->get_parent();
                z->is_black = true;
                z = z->get_parent();
                z->is_black = z == root; // Color red except if z's grandparent is root
            } 
            else 
            {
                if (tree_is_left_child(z)) {
                    z = z->get_parent();
                    tree_rotate_right(z);
                }

                // case 3
                z = z->get_parent();
                z->is_black = true;
                z = z->get_parent();
                z->is_black = false;
                tree_rotate_left(z);
                return;
            }
        }
    }
}


// P: Parent
// X: Double black node
// W: X's sibling
//
// Case 0: Root is double black node (terminal case)
//     - Nothing to do
//
// Case 1: Black parent (P), red sibling (W) with two black child
//
//     - Left rotation on parent
//     - Recolor parent and W
//
//        P-> B                        W-> B
//           / \                          / \
//     X->  DB  R <-W     ==>        P-> R   B <-Z
//             / \                      / \
//        Y-> B   B <-Z            X-> DB  B <-Y
//
// Case 2: Black parent (P), black sibling (W) with two black child
//
//     - Recolor W
//     - Parent becomes X
//
//        P-> B                    New X-> B
//           / \                          / \
//     X->  DB  B <-W     ==>    Old X-> B   R <-W
//             / \                          / \
//        Y-> B   B <-Z               Y->  B   B <-Z
//
// Case 3: Red parent (P), black sibling (W) with two black child (terminal case)
//
//     - Recolor parent and W
//
//        P-> R                        P-> B
//           / \                          / \
//     X->  DB  B <-W     ==>        X-> B   R <-W
//             / \                          / \
//        Y-> B   B <-Z               Y->  B   B <-Z
//
// Case 4: Black parent (P), black sibling (W) with red left child (Y) and black right child (B)
//
//     - Right rotation on W
//     - Recolor Y and W
//
//        P-> B                        P-> B
//           / \                          / \
//     X->  DB  B <-W     ==>        X-> DB  B <-Y
//             / \                            \
//        Y-> R   B <-Z                        R <-W
//                                              \
//                                               B <-Z
//
// Case 5: Red or black parent (P), black sibling (W) with red or black left child (Y)
//         and red right child (B) (terminal case)
//
//     - Left rotation on parent
//     - Color W with parent's color
//     - Color parent and Z black
//
//        P-> RB                       W-> RB
//           / \                          / \
//     X->  DB  B <-W     ==>        P-> B   B <-Z
//             / \                      / \
//        Y-> RB  R <-Z            X-> B  RB <-Y
//
// All cases apply to the mirrored cases


// This function should only be called to fix a double black node case
template <typename NodePtr>
void tree_delete_fix(NodePtr root, NodePtr x_parent)
{
    // Double black nodes always start as a null pointer
    NodePtr x = NULL;

    while (root != x && tree_node_is_black(x)) {
        if (x == x_parent->left) {
            NodePtr w = x_parent->right;

            if (!w->is_black) { // case 1
                x_parent->is_black = false;
                w->is_black = true;
                tree_rotate_left(root, x_parent);
                w = x_parent->right;
            }

            if (tree_node_is_black(w->left) && tree_node_is_black(w->right)) { // case 2 && case 3
                w->is_black = false;
                x = x_parent;
                x_parent = x->get_parent();
            } else {
                if (tree_node_is_black(w->right)) { // case 4
                    w->is_black = false;
                    tree_rotate_right(root, w);
                    w = x_parent->right;
                    w->is_black = true;
                }

                // case 5
                w->is_black = x_parent->is_black;
                x_parent->is_black = true;
                w->right->is_black = true;
                tree_rotate_left(root, x_parent);
                x = root;
                break;
            }
        } else {
            NodePtr w = x_parent->left;

            if (!w->is_black) { // case 1
                x_parent->is_black = false;
                w->is_black = true;
                tree_rotate_right(root, x_parent);
                w = x_parent->left;
            }

            if (tree_node_is_black(w->right) && tree_node_is_black(w->left)) { // case 2 && case 3
                w->is_black = false;
                x = x_parent;
                x_parent = x->get_parent();
            } else {
                if (tree_node_is_black(w->left)) { // case 4
                    w->is_black = false;
                    tree_rotate_left(root, w);
                    w = x_parent->left;
                    w->is_black = true;
                }

                // case 5
                w->is_black = x_parent->is_black;
                x_parent->is_black = true;
                w->left->is_black = true;
                tree_rotate_right(root, x_parent);
                x = root;
                break;
            }
        }
    }
    if (x) { // case 0 + 2 when parent redti
        x->is_black = true;
    }
}

template <typename NodePtr>
void tree_transplant_node(NodePtr pos, NodePtr& node)
{
    node->is_black = pos->is_black;
    node->parent = pos->parent;
    if (tree_is_left_child(pos)) {
        node->parent->left = node;
    } else {
        node->get_parent()->right = node;
    }
    node->left = pos->left;
    node->left->set_parent(node);
    node->right = pos->right;
    if (node->right) {
        node->right->set_parent(node);
    }
}

template <typename NodePtr>
void tree_remove_node(NodePtr root, NodePtr target)
{
    NodePtr y = target;

    if (y->left != NULL && y->right != NULL) {
        y = tree_min(target->right);
    }

    // x is NULL or y's only child
    NodePtr x;
    if (y->left != NULL) {
        x = y->left;
    } else {
        x = y->right;
    }

 
    NodePtr x_parent = y->get_parent();


    if (x != NULL) {
        x->parent = y->parent;
    }
    if (tree_is_left_child(y)) {
        y->parent->left = x;
        if (root == y) {
            root = x;
        }
    } else {
        if (target->right == y) {
            x_parent = y;
        }
        y->get_parent()->right = x;
    }

    bool removed_black = y->is_black;

    if (y != target) {
        tree_transplant_node(target, y);
        if (target == root) {
            root = y;
        }
    }

    // eger kaldırılan node siyah ise balance edilecek ağaç
    if (removed_black) {
        if (root == NULL) {
            return;
        }

        if (x != NULL) {
            x->is_black = true;
            return;
        }

        tree_delete_fix(root, x_parent);
    }
}
} // namespace ft
