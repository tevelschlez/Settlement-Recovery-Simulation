#include "Settlement.h"
#include <string>
#include <vector>
using std::string;
using std::vector;


        Settlement::Settlement(const string &name, SettlementType type):name(name),type(type){}

        const string &Settlement::getName() const{
                return name;
        }

        SettlementType getType() const{
                return type;
        }

        const string Settlement::toString() const{
                string str=name + " ";

                if(type== SettlementType::VILLAGE)
                        str+="Village";
                else if(type ==  SettlementType::CITY)
                        str+="City";
                else if(type == SettlementType::METROPOLIS)
                        str+="Metropolis";
                
                return str;
        }
