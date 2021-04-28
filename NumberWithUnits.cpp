#include "NumberWithUnits.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;


namespace ariel {

    map<string, map<string, double>> NumberWithUnits::units;

    /**
     * @brief Construct a new Number With Units:: Number With Units object
     * 
     * @param amount - represent the unit amount
     * @param unit 
     */
    NumberWithUnits::NumberWithUnits(double a, const std::string& u) : amount(a), unit(u){ 
        if(units.count(u)==0){
            throw invalid_argument{"Error: unit [" + u + "] doesn't exist in the units map"};
        }
    }

    /**
     * @brief - read a text file & add to the map
     * 
     * @param units_file - ifstream with the path of the units file
     */
    void NumberWithUnits::read_units(ifstream& units_file){
        if(units_file){ 
            string u1, u2, eq;
            double one=0, n=0;

            while(units_file >> one >> u1 >> eq >> n >> u2){
                if(one!=1 && eq!="="){
                    throw invalid_argument{"Error: invalid units file format"}; 
                }

                units[u1][u2]=n;
                units[u2][u1]=1/n;

                for(auto const &i : units.at(u2)) {
                    double tmpAmount=i.second*units[u1][u2];
                    string tmpUnit=i.first;
                    units[u1][tmpUnit]=tmpAmount;
                    units[tmpUnit][u1]=1/tmpAmount;
                }

                for(auto const &i : units.at(u1)) {
                    double tmpAmount=i.second*units[u2][u1];
                    string tmpUnit=i.first;
                    units[u2][tmpUnit]=tmpAmount;
                    units[tmpUnit][u2]=1/tmpAmount;
                }
            }
        }
        else{
            throw string("Error: file doesn't exist");
        }
    }

/**
 * @brief - A function that converts from one unit to another
 * 
 * @param from - the source unit
 * @param to - the dest unit
 * @param n - the amount that should be converted to the other unit
 * @return double - num that represent the converted amount
 */
    double NumberWithUnits::convertFromTo(const string& from, const string& to, double n){
        if(units.at(from).count(to)==0){
            throw invalid_argument("Error: can't convert from [" + from + "] to [" + to + "]");
        }
        if(from==to){
            return n;
        }
        return n*units.at(from).at(to);
    }

    //Overloading the input & output operators
    std::ostream& operator<<(std::ostream& out, const NumberWithUnits& u){
       return out << u.amount << "[" << u.unit << "]";
    }

    std::istream& operator>>(std::istream& in, NumberWithUnits& u){
        double tmpAmount=0;
        string tmpUnit=" ";
        char c=']', c1=' ', c2=' ';
        in >> skipws >> tmpAmount >> c1 >> tmpUnit;
        if(tmpUnit[tmpUnit.length()-1]!=c)
        {
            in >> c2;
        }
        else
        {
            tmpUnit=tmpUnit.substr(0, tmpUnit.length()-1);
        }
        if (NumberWithUnits::units.count(tmpUnit)==0)
        {
            throw invalid_argument("Error: unit doesn't exist in the units map");
        }
        u=NumberWithUnits(tmpAmount,tmpUnit);
        return in;
    }
        
    //Overloading the arithmetic operators
    //Plus & Minus operators
    NumberWithUnits operator+(const NumberWithUnits& u1, const NumberWithUnits& u2){ 
        double conv=NumberWithUnits::convertFromTo(u2.unit, u1.unit, u2.amount);
        return NumberWithUnits(u1.amount+conv, u1.unit);
    }

    NumberWithUnits operator+(const NumberWithUnits& u1, double n){ 
        return NumberWithUnits(u1.amount+n, u1.unit);
    }        

    NumberWithUnits operator-(const NumberWithUnits& u1, const NumberWithUnits& u2){ 
        double conv=NumberWithUnits::convertFromTo(u2.unit, u1.unit, u2.amount);
        return NumberWithUnits(u1.amount-conv, u1.unit);
    }

    NumberWithUnits operator-(const NumberWithUnits& u1, double n){ 
        return NumberWithUnits(u1.amount-n, u1.unit);
    }

    //Unary
    NumberWithUnits operator+(const NumberWithUnits& u1){ 
        return NumberWithUnits(u1.amount, u1.unit);
    }

    NumberWithUnits operator-(const NumberWithUnits& u1){ 
        return NumberWithUnits(-u1.amount, u1.unit);
    }

    //Compound operators
    NumberWithUnits& NumberWithUnits::operator+=(const NumberWithUnits& u1){
        this->amount+=convertFromTo(u1.unit, this->unit, u1.amount);
        return *this;
    }

    NumberWithUnits& NumberWithUnits::operator-=(const NumberWithUnits& u1){
        this->amount-=convertFromTo(u1.unit, this->unit, u1.amount);
        return *this;
    }

    NumberWithUnits operator*=(NumberWithUnits& u1, double n){ 
        return NumberWithUnits(u1.amount*=n, u1.unit);
    }

    NumberWithUnits operator*=(double n, NumberWithUnits& u1){ 
        return NumberWithUnits(u1.amount*=n, u1.unit);
    }

    //Mul operator
    NumberWithUnits operator*(const NumberWithUnits& u1, const double& n){ 
        return NumberWithUnits(u1.amount*n, u1.unit);
    }

    NumberWithUnits operator*(const double& n, const NumberWithUnits& u1){ 
        return u1*n;
    }

    //Increase & Decrease operators
    NumberWithUnits& NumberWithUnits::operator++(){ //prefix ++a
        this->amount++;
        return *this;
    } 

    NumberWithUnits& NumberWithUnits::operator--(){ //prefix --a
        this->amount--;
        return *this;
    }

    NumberWithUnits NumberWithUnits::operator++(int){ //postfix a++
        NumberWithUnits ans(amount, unit);
        this->amount++;
        return ans;
    }

    NumberWithUnits NumberWithUnits::operator--(int) { //postfix a--
        NumberWithUnits ans(amount, unit);
        this->amount--;
        return ans;
    }

    /**
     * @brief - A function that compare between two units
     * 
     * @param u1 - the first unit
     * @param u2 - the second unit
     * @param convert - the converted amount
     * @return int - (1) if the first unit is bigger than the second one, (-1) if it's smaller, otherwish (0)
     */
    int NumberWithUnits::compare(const NumberWithUnits& u1, const NumberWithUnits& u2, double convert){
        const double EPS = 0.001;
        double comp=u1.amount-convert;
        return (comp>EPS) ? 1 : (-comp>EPS) ? -1 : 0;
    }

    //Comparison operators
    bool operator>(const NumberWithUnits& u1, const NumberWithUnits& u2){
        double conv=NumberWithUnits::convertFromTo(u2.unit, u1.unit, u2.amount);
        return NumberWithUnits::compare(u1, u2, conv)==1;
    }

    bool operator<(const NumberWithUnits& u1, const NumberWithUnits& u2){
        double conv=NumberWithUnits::convertFromTo(u2.unit, u1.unit, u2.amount);
        return NumberWithUnits::compare(u1, u2, conv)==-1;
    }

    bool operator>=(const NumberWithUnits& u1, const NumberWithUnits& u2){
        return (!(u1<u2));
    }

    bool operator<=(const NumberWithUnits& u1, const NumberWithUnits& u2){
        return (!(u1>u2));
    }

    bool operator==(const NumberWithUnits& u1, const NumberWithUnits& u2){
        double conv=NumberWithUnits::convertFromTo(u2.unit, u1.unit, u2.amount);
        return NumberWithUnits::compare(u1, u2, conv)==0;
    }

    bool operator!=(const NumberWithUnits& u1, const NumberWithUnits& u2){
        return (!(u1==u2));
    } 
}