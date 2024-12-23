#include "Settlement.h"
#include <string>
#include <vector>
using std::string;
using std::vector;


        Settlement::Settlement(const string &name, SettlementType type):name(name),type(type){}

        const string &Settlement::getName() const{
                return name;
        }

        SettlementType Settlement::getType() const{
                return type;
        }

        const string Settlement::toString() const{
                string str=name+" ";

                if(type== SettlementType::VILLAGE)
                        str+="village";
                else if(type ==  SettlementType::CITY)
                        str+="city";
                else if(type == SettlementType::METROPOLIS)
                        str+="metropolis";
                
                return str;
        }

