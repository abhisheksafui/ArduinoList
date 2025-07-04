/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ArduinoList.h
 * Author: Abhishek
 *
 * Created on 29 April, 2018, 11:29 AM
 * Updated on 19 May, 2025, 00:10 AM
 */

#ifndef __ARDUINO_LISH_H__
#define __ARDUINO_LISH_H__
#include <functional>

template <typename T> class ArduinoListNode {
public:
  T data;

  ArduinoListNode *next;
  ArduinoListNode *prev;

  ArduinoListNode(T d) : data(d) {
  }
};

template <typename T>
class ArduinoList {
  ArduinoListNode<T> *first;
  ArduinoListNode<T> *last;

  int itemCount;
protected:

  /** Delete the current node and return next */
  ArduinoListNode<T>* remove(ArduinoListNode<T> *node) {
    auto next = node->next;
    auto prev = node->prev;

    if (first == node) {
      first = next;
    }
    
    if(last == node) {
      last = prev;
    }
    
    if(next != nullptr){
      next->prev = prev;
    }
    
    if(prev != nullptr){
      prev->next = next;
    }
    
    
    delete node;
    itemCount --;
    return next;
  }

public:

  class iterator {
    
    friend class ArduinoList;
    ArduinoListNode<T> *node;

  protected:
    ArduinoListNode<T>* getNode(){return node;}
  public:

    iterator(ArduinoListNode<T> *n) : node(n) {
    }

    iterator operator++() {
      if (node != nullptr)
        node = node->next;
      
      return *this;
    }
    
    iterator operator++(int) {
      if (node != nullptr)
        node = node->next;
      
      return *this;
    }
    
    iterator operator+(int i){
      for(int count=0; count<i && node!=nullptr; count++){
        node = node->next;
      }
      return *this;
    }

    bool operator==(const iterator& it) {
      return node == it.node;
    }

    bool operator!=(const iterator& it) {
      return node != it.node;
    }
    
    T operator*(){ return node->data; }


  };

  ArduinoList() {
    first = nullptr;
    last = nullptr;
    itemCount = 0;
  }

  ~ArduinoList() {
    ArduinoListNode<T> *current;

    while (first != nullptr) {
      current = first;
      first = first->next;
      delete current;
    }

    last = nullptr;
    itemCount = 0;
  }

  void push_front(T data) {

    ArduinoListNode<T> *newNode = new ArduinoListNode<T>(data);
    newNode->next = nullptr;
    newNode->prev = nullptr;

    if (first == nullptr) {
      /* first = last for first node*/
      first = newNode;
      last = newNode;
    } else {
      /* only change first*/
      newNode->next = first;
      first->prev = newNode;
      first = newNode;
    }
    itemCount++;
  }

  void push_back(T data) {

    ArduinoListNode<T> *newNode = new ArduinoListNode<T>(data);
    newNode->next = nullptr;
    newNode->prev = nullptr;

    if (last == nullptr) {
      /* first = last for first node*/
      first = newNode;
      last = newNode;
    } else {
      last->next = newNode;
      newNode->prev = last;
      last = newNode;
    }
    itemCount++;
  }
  
  iterator IRAM_ATTR begin(){
    return first;
  }
  
  iterator IRAM_ATTR end(){
    return last->next;
  }

  int size() {
    return itemCount;
  }

  bool isEmpty(){
    return size() == 0;
  }
  
  T pop_front(){
    iterator it = begin();
    if(it.node == nullptr){
      return T();
    }
    T tmp = *it;
    remove(it);
    return tmp;
  }
  /* If size() is zero then front return garbage */
  T front() {
    if (first) {
      return first->data;
    }
    return T();
  }

  /* If size() is zero then front return garbage */
  T back() {
    if (last) {
      return last->data;
    }
    return T();
  }
  
  iterator remove(iterator it){
    if(it == nullptr){
      return nullptr;
    }
    
    return remove(it.node);
   
  }

  void remove(T data) {
    ArduinoListNode<T> *current = first;

    while (current != nullptr) {
      if (current->data == data) {
        current = remove(current);
      } else {
        current = current->next;
      }
    }
  }

  void remove_if(std::function<bool(T) > func) {
    ArduinoListNode<T> *current = first;

    while (current != nullptr) {
      if (func(current->data)) {
        current = remove(current);
      } else {
        current = current->next;
      }
    }
  }

  void for_each(std::function<void(T) > func) {
    auto current = first;
    while (current != nullptr) {
      func(current->data);

      current = current->next;
    }
  }
};

#endif /* LIST_H */

