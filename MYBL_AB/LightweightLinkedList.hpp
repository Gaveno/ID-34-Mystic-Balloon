#pragma once
#ifndef LIGHTWEIGHT_LINKED_LIST_H
#define LIGHTWEIGHT_LINKED_LIST_H

// Does not do creation, only deletion

template<typename T>
class LightweightLinkedList {
public:
    T* head;
    T* tail;

    LightweightLinkedList() : head(nullptr), tail(nullptr) {}

    ~LightweightLinkedList() {
        clear();
    }

    void add(T* node) {
        if (!head) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }

    bool remove(T* location) {
        T* current = head;
        T* previous = nullptr;

        while (current) {
            if (current == location) {
                if (previous) {
                    previous->next = current->next;
                } else {
                    head = current->next;
                }

                if (current == tail) {
                    tail = previous;
                }

                delete current;
                return true;
            }
            previous = current;
            current = current->next;
        }

        return false;
    }

    void clear() {
        T* current = head;
        while (current) {
            T* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
    }

    bool isEmpty() const {
        return head == nullptr;
    }
};

#endif // LIGHTWEIGHT_LINKED_LIST_H