#pragma once

#include "types.h"
#include "base.h"

struct Node {
    size_t value;
    Node* next;
    Node();
    Node(size_t x);
    virtual ~Node();
};

class LinkedList {
    Node* head;
    Node* cur_pos;
    size_t _size;
  public:
    LinkedList();
    LinkedList(const LinkedList& list);
    virtual ~LinkedList();
    size_t size() const;
    size_t front() const;
    void push_front(size_t x);
    void remove_first(size_t x);
    bool exist(size_t x);
    void cur_to_begin();
    size_t get_cur_val();
    bool is_cur_end() const;
    friend ostream& operator<<(ostream& out, const LinkedList& list);
//    friend class CnfContainer;
};
