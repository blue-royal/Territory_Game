#include "serialise.h"

Byte::Byte(char byte){
    this->byte = byte;
}

char Byte::get_byte(){
    return byte;
}

void Byte::print(){
    std::cout << (int)byte;
}


Serialise Serialise::operator << (int integer){
    bytes.push_back(Byte(0b00000010));
    length ++;

    bytes.resize(length+4);
    memcpy(&bytes[length], &integer, 4);
    length += 4;

    return *this;
    
}

Serialise Serialise::operator << (bool booleans){
    length ++;
    bytes.push_back(Byte(booleans));

    return *this;
}

Serialise Serialise::operator << (float reals){
    bytes.push_back(Byte(0b00000011));
    length ++;

    bytes.resize(length+4);
    memcpy(&bytes[length], &reals, 4);
    length += 4;

    return *this;
}


std::vector<Byte> Serialise::get_bytes(){
    return bytes;
}

Deserialise::Deserialise(std::vector<Byte> bytes){
    this->bytes = bytes;
    find_next_type();
}

Data_Types Deserialise::get_next_type(){
    return next_type;
}

void Deserialise::find_next_type(){
    if (current >= bytes.size()){
        next_type = Data_Types::finished;
    }
    else{
        switch (bytes[current].get_byte())
        {
        case 0b00000000:
            next_type = Data_Types::bools;
            break;
        case 0b00000001:
            next_type = Data_Types::bools;
            break;
        case 0b00000010:
            next_type = Data_Types::integer;
            break;
        case 0b00000011:
            next_type = Data_Types::reals;
            break;
        default:
            next_type = Data_Types::finished;
            break;
        }
    }
}

int Deserialise::get_int(){
    int result;
    current ++;
    memcpy(&result, &bytes[current], 4);
    current += 4;

    find_next_type();
    
    return result;
}

float Deserialise::get_real(){
    float result;
    current ++;
    memcpy(&result, &bytes[current], 4);
    current += 4;

    find_next_type();

    return result;
}

bool Deserialise::get_bool(){
    current++;
    find_next_type();
    return static_cast<bool>(bytes[current-1].get_byte());
}
