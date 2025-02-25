/*
  ==============================================================================

    termite_data_types.h
    Created: 23 Oct 2024 2:02:48pm
    Author:  Jake Morgan

  ==============================================================================
*/

#pragma once
#include "math.h"
#include <JuceHeader.h>

//creating standard datatypes that will be used throughout the software.

namespace termite {

typedef struct{
    float current_sample_value;
    int current_sample_in_buffer;
} tOscillator_Index;

// save for later. might need.
class termite_UTF16Char{
public:
    using termite_chartype = char;
};


// this is a UTF-8 string
// maybe later one I will add UTF-16/32
// no use for it now.
class string{
public:
    // default constructor.
    string() : data(nullptr), length(0) {}
    //c style constructor.
    string(const char* str){
        length = 0;
        while (str[length] != '\0'){
            length++;
        }
        
        data = new char[length +1];
        for(size_t i = 0; i < length; i++){
            data[i] = str[i];
        }
        data[length] = '\0';
    }
    // deletes the data
    ~string(){
        delete[] data;
    }
    
    // returns size
    size_t size() const {
        return length;
    }
    
    // overloads the operator= for printing.
    friend std::ostream& operator<<(std::ostream& os, const string& str){
        if(str.data){
            os << str.data;
        }
        return os;
    }
    
    // overloads the operator= for assignment
    // theres probably a way to overload the 
    string& operator=(const string& other){
        if(this == &other) return *this; //self assignment check
        
        delete[] data;
        
        length= other.length;
        data = new char[length + 1];
        for(size_t i = 0; i < length; i++){
            data[i] = other.data[i];
        }
        data[length] = '\0';
        return *this;
    }
    
    
    char* returnCharacters(){
        return data;
    }
private:
    
    char* data;
    size_t length;
    
};

// =============================================
// this is exclusively for the termite string.
// yep.
// =============================================

class juce2termite{
public:
    juce2termite() : data(nullptr){}
    
    // i don't know why this isn't working.
    juce2termite(juce::String inJString, std::unique_ptr<termite::string> &inTString){
        data = inJString.toRawUTF8();
        inTString.reset(new termite::string(data));
    };
    
    ~juce2termite();
    
private:
    const char* data;
};

class var {
public:
    var() : size(0), variableInt(0), variableFloat(0.f), variableDouble(0.0){}
    
    var(int i){};
    
    var(float f){};
    
    var(double d){};
    
    var(const int& i){};
    
    var(const float& f){};
    
    var(const double& d){};
    
    
    ~var();
    
    // make work.
    auto getValue(){
    }
    
private:
    size_t size;
    
    int variableInt;
    float variableFloat;
    double variableDouble;
};
// id system to track variables and func
//std::unique_ptr<termite_String>* varID;

class complex{
public:
    complex();
    complex(float real, float imaginary){
        float_real = real;
        float_imaginary = imaginary;
    }
    complex(int real, int imaginary){
        int_real = real;
        int_imaginary = imaginary;
    }
    //move construct
    complex(const complex&&){}
    //copy construct;
    complex(const complex&){}
    
    float getReal(){
        return float_real;
    }
    float getImaginary(){
        return float_imaginary;
    }
//    int getReal(){
//        return int_real;
//    }
//    int getImaginary(){
//        return int_imaginary;
//    }
    ~complex();
private:
    float float_real;
    float float_imaginary;
    int int_real;
    int int_imaginary;
};

class hashmap{
    // i don't even know.
public:
    
private:
    
};

class chordMemory{
    
public:
    chordMemory();
    
    ~chordMemory();
    
    template<typename cm> cm chordMemoryThree(cm x, cm y, cm z){
        return (x, y, z);
    }
    template<typename cm> cm chordMemoryFour(cm w, cm x, cm y, cm z){
        return (w,x,y,z);
    }
    
private:
    
};

}
