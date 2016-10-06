#ifndef NUM_H
#define NUM_H

#include <assert.h>

class Num
{
public:
    typedef int  num_t;
    typedef char dig_t;

    num_t base;
    num_t num;
    dig_t dig1;
    dig_t dig2;
    dig_t dig3;
    bool  isBackwards;

protected:
    Num() {};

public:
	Num(int root) : base(root),
    num(root*root), isBackwards(false)
    { 
        assert(num < 1000);
        dig3 = num % 10;
        dig2 = (num / 10) % 10;
        dig1 = num / 100;
    };
    
public:
    Num& flip() {
        char tmp = dig3;
        dig3 = dig1;
        dig1 = tmp;
        isBackwards = !isBackwards;
        return *this;
    };

    Num* clone() {
        Num* copy = new Num();
        *copy = *this;
        return copy;
   };
   
   Num* flipped() {
        return &(clone()->flip());
   };
};

#endif /* NUM_H */
