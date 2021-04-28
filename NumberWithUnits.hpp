#pragma once

#include <string>
#include <map>
#include <iostream>

using namespace std;

namespace ariel {

    class NumberWithUnits {

        private:
        double amount;
        std::string unit;

        public:
        static std::map<std::string, std::map<std::string, double>> units;
        NumberWithUnits(double a, const std::string& u);

        static void read_units(std::ifstream& units_file);
        static double convertFromTo(const string& from, const string& to, double n);
        static int compare(const NumberWithUnits& u1, const NumberWithUnits& u2, double convert);

        //Input & Output operators
        friend std::ostream& operator<<(std::ostream& out, const NumberWithUnits& u);
        friend std::istream& operator>>(std::istream& in, NumberWithUnits& u);

        //Arithmetic operation:
        //Plus & Minus operators
        friend NumberWithUnits operator+(const NumberWithUnits& u1, const NumberWithUnits& u2); //(a+b)
        friend NumberWithUnits operator+(const NumberWithUnits& u1, double n); //(a+2)
        friend NumberWithUnits operator-(const NumberWithUnits& u1, const NumberWithUnits& u2); //(a-b)
        friend NumberWithUnits operator-(const NumberWithUnits& u1, double n); //(a-2)

        //Unary
        friend NumberWithUnits operator+(const NumberWithUnits& u1); //(+a)
        friend NumberWithUnits operator-(const NumberWithUnits& u1); //(-a)

        //Compound operators
        NumberWithUnits& operator+=(const NumberWithUnits& u1); 
        NumberWithUnits& operator-=(const NumberWithUnits& u1);
        friend NumberWithUnits operator*=(NumberWithUnits& u1, double n); //(a*=2)
        friend NumberWithUnits operator*=(double n, NumberWithUnits& u1); //(2*=a)
          
        //Mul operator
        friend NumberWithUnits operator*(const NumberWithUnits& u1, const double& n); //(a*2)
        friend NumberWithUnits operator*(const double& n, const NumberWithUnits& u1); //(2*a)

        //Increase & Decrease operators
        NumberWithUnits& operator++(); //prefix ++a
        NumberWithUnits operator++(int); //postfix a++ 
        NumberWithUnits& operator--(); //prefix --a
        NumberWithUnits operator--(int); //postfix a--
    
        //Comparison operators
        friend bool operator>(const NumberWithUnits& u1, const NumberWithUnits& u2); //a>b
        friend bool operator<(const NumberWithUnits& u1, const NumberWithUnits& u2); //a<b
        friend bool operator>=(const NumberWithUnits& u1, const NumberWithUnits& u2); //a>=b
        friend bool operator<=(const NumberWithUnits& u1, const NumberWithUnits& u2); //a<=b
        friend bool operator==(const NumberWithUnits& u1, const NumberWithUnits& u2); //a==b
        friend bool operator!=(const NumberWithUnits& u1, const NumberWithUnits& u2); //a!=b
    };
}