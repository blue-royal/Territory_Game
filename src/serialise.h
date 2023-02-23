#ifndef SERIALISE__
#define SERIALISE__

#include <iostream>
#include <vector>
#include <string.h>

class Byte
{
    private:
        char byte;
    public:
        Byte() = default;
        Byte(char byte);
        char get_byte();
        void print();
        
};

class Serialise
{
    public:
        Serialise() = default;
        Serialise operator << (int integer);
        Serialise operator << (bool booleans);
        Serialise operator << (float reals);

        std::vector<Byte> get_bytes();
        
    private:
        std::vector<Byte> bytes;
        unsigned int length = 0;


};

enum class Data_Types : int { integer, reals, bools, finished };

class Deserialise
{
    public:
        Deserialise(std::vector<Byte> bytes);
        Data_Types get_next_type();
        int get_int();
        float get_real();
        bool get_bool();
    
    private:
        std::vector<Byte> bytes;
        Data_Types next_type;
        void find_next_type();
        unsigned int current = 0;
};

#endif