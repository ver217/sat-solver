#include "../include/linkedlist.h"

Node::Node() : next(nullptr) {}

Node::Node(size_t x) : value(x), next(nullptr) {}

Node::~Node() {
    if (next) {
        delete next;
        next = nullptr;
    }
}

LinkedList::LinkedList() : head(nullptr), cur_pos(nullptr), _size(0) {}

LinkedList::LinkedList(const LinkedList &list) : cur_pos(nullptr), _size(list._size) {
    if (list.head) {
        head = new Node(list.head->value);
        Node* node = head;
        Node* that = list.head->next;
        while (that) {
            node->next = new Node(that->value);
            node = node->next;
            that = that->next;
        }
    }
}

LinkedList::~LinkedList() {
    if (head) {
        delete head;
        head = nullptr;
    }
}

size_t LinkedList::size() const {
    return _size;
}

size_t LinkedList::front() const {
    if (head)
        return head->value;
    else
        throw "list from out of range";
}

void LinkedList::push_front(size_t x) {
    Node* node = new Node(x);
    node->next = head;
    head = node;
    ++_size;
}

void LinkedList::remove_first(size_t x) {
    Node* prev = head;
    if (!prev)
        return;
    if (head && head->value == x) {
        head = head->next;
        prev->next = nullptr;
        delete prev;
        --_size;
        return;
    }
    Node* node = head->next;
    while (node) {
        if (node->value == x) {
            prev->next = node->next;
            node->next = nullptr;
            delete node;
            --_size;
            return;
        }
        prev = node;
        node = node->next;
    }
}

bool LinkedList::exist(size_t x) {
    Node* node = head;
    while (node) {
        if (node->value == x)
            return true;
        node = node ->next;
    }
    return false;
}

ostream& operator<<(ostream& out, const LinkedList& list) {
    Node* node = list.head;
    while (node) {
        out << node->value << ' ';
        node = node->next;
    }
    out << endl;
    return out;
}

void LinkedList::cur_to_begin() {
    cur_pos = head;
}

size_t LinkedList::get_cur_val() {
    if (cur_pos) {
        size_t val = cur_pos->value;
        cur_pos = cur_pos->next;
        return val;
    }
    throw "list get cur val ERROR";
}

bool LinkedList::is_cur_end() const {
    return cur_pos == nullptr;
}
