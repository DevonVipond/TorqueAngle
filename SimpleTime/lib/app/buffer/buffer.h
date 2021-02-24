#ifndef BUFFER_H // include guard
#define BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "..//types/types.h"

namespace Buffer {
    struct BufferOverflowException : public std::exception
    {
    	const char * what () const throw ()
        {
        	return "Buffer Overflow";
        }
    };
    
    types::timestamp mystamp = 0;
    
    template< typename T >
    class Buffer {
    private:
        std::vector<T> _buffer;
        size_t _capacity;
    
        void _throw_if_full() {
            if (_buffer.size() >= _capacity) {
                throw BufferOverflowException();
            }
        }
    
        void _add(T val) {
            _throw_if_full();
    
            _buffer.push_back(val);
        }
    
    public:
    
        Buffer(size_t capacity): _capacity(capacity), _buffer(capacity, 0) { }
    
        void add(T val) {
            _add(val);
        }
    
        T pop_front() {
            return _buffer.pop_front();
        }
    
    };
    
}

#endif