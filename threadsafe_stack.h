//
// Created by ajayasingh on 6/27/2018.
//

#include <exception>
#include <memory>
#include <mutex>
#include <stack>

#ifndef STACK_LOCK_H
#define STACK_LOCK_H

struct empty_stack : public std::exception {
    const char* what () const throw();
};

template<typename T>
class threadsafe_stack {
private:
    std::stack<T> data;
    mutable std::mutex mut;
public:
    threadsafe_stack() {

    }
    threadsafe_stack(const threadsafe_stack& in) {
            std::lock_guard<std::mutex> _(in.mut);
            data = in.data;
    }

    void push(T da) {
        std::lock_guard<std::mutex> _(mut);
        data.push(std::move(da));
    }

    std::shared_ptr<T> pop() {
        std::lock_guard<std::mutex> _(mut);
        if(data.empty()) throw empty_stack();
        std::shared_ptr<T> const res (std::make_shared<T>(std::move(data.top())));
        data.pop();
    }

    void pop(T& da) {
        std::lock_guard<std::mutex> _(mut);
        if(data.empty()) throw empty_stack();
        da = data.top();
        data.pop();
    }

};

#endif //STACK_LOCK_H

