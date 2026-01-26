#ifndef STACK_H
#define STACK_H

using namespace std;

template <typename T>
class Stack {
   private:
    T* arr;
    int capacity;
    int topIndex;

    void resize() {
        capacity *= 2;
        T* temp = new T[capacity];
        for (int i = 0; i <= topIndex; i++) temp[i] = arr[i];
        delete[] arr;
        arr = temp;
    }

   public:
    Stack() : capacity(10), topIndex(-1) { arr = new T[capacity]; }

    ~Stack() { delete[] arr; }

    void push(T val) {
        if (topIndex == capacity - 1) resize();
        arr[++topIndex] = val;
    }

    void pop() {
        if (topIndex >= 0) topIndex--;
    }

    T top() { return arr[topIndex]; }

    bool empty() { return topIndex == -1; }
};

#endif