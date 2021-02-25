#ifndef BUFFER_H // include guard
#define BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <queue>

#include "../types/types.h"

namespace app {
namespace buffer {
    struct BufferOverflowException : public std::exception
    {
    	const char * what () const throw ()
        {
        	return "Buffer Overflow";
        }
    };
    
    template< typename T >
    class Buffer {
    private:
        std::queue<T> _buffer;
        size_t _max_capacity;
    
        void _throw_if_full() {
            if (_buffer.size() >= _max_capacity) {
                throw BufferOverflowException();
            }
        }
    
        void _push(T val) {
            _throw_if_full();
    
            _buffer.push(val);
        }
    
    public:
    
        Buffer(size_t max_capacity): _max_capacity(capacity), _buffer(capacity, 0) { }
    
        void push(T val) { _push(val); }
    
        T pop_front() {
            T front_element = _buffer.front();
            _buffer.pop();

            return front_element;
        }
    };
    
}; };

#endif