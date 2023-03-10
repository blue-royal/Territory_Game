#include "serialise.h"

// a byte is just a renamed char for readabiltity
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
    // add the type header
    bytes.push_back(Byte(0b00000010));
    length ++;

    bytes.resize(length+4);
    // move the memory of the integer to the Byte vector
    memcpy(&bytes[length], &integer, 4);
    length += 4;

    return *this;
}

Serialise Serialise::operator << (bool booleans){
    // booleans are self headered
    length ++;
    bytes.push_back(Byte(booleans));

    return *this;
}

Serialise Serialise::operator << (float reals){
    // add header for floats
    bytes.push_back(Byte(0b00000011));
    length ++;

    bytes.resize(length+4);
    // copy memory for the float into the vector
    memcpy(&bytes[length], &reals, 4);
    length += 4;

    return *this;
}


std::vector<Byte> Serialise::get_bytes(){
    std::vector<Byte> new_bytes = bytes;
    // add the end message header
    new_bytes.push_back(0b00000100);
    // return a vector of bytes
    return new_bytes;
}

Deserialise::Deserialise(std::vector<Byte> bytes){
    this->bytes = bytes;
    find_next_type();
}

Data_Types Deserialise::get_next_type(){
    // return the type of the next object in the serialised data
    return next_type;
}

void Deserialise::find_next_type(){
    // check all headers for the next data type
    if (current >= bytes.size()){
        next_type = Data_Types::finished;
    }
    else{
        switch (bytes[current].get_byte())
        {
        case (0b00000000):
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
        case 0b00000100:
            next_type = Data_Types::finished;
            break;
        default:
            next_type = Data_Types::finished;
            break;
        }
    }
}

int Deserialise::get_int(){
    // return the integer
    int result;
    current ++;
    memcpy(&result, &bytes[current], 4);
    current += 4;

    find_next_type();
    
    return result;
}

float Deserialise::get_real(){
    // return the float
    float result;
    current ++;
    memcpy(&result, &bytes[current], 4);
    current += 4;

    find_next_type();

    return result;
}

bool Deserialise::get_bool(){
    // return the bool
    current++;
    find_next_type();
    return static_cast<bool>(bytes[current-1].get_byte());
}
