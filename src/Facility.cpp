#include "Facility.h"
#pragma once
#include <string>
#include <vector>
#include <sstream>
using std::string;
using std::vector;

class FacilityType {
    public:
        FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):name(name),category(category),price(price),lifeQuality_score(lifeQuality_score),economy_score(economy_score),environment_score(environment_score){}

        const string &FacilityType::getName() const{
            return name;
        }

        int FacilityType::getCost() const{
            return price;
        }
        int FacilityType::getLifeQualityScore() const{
            return lifeQuality_score;
        }
        int FacilityType::getEnvironmentScore() const{
            return environment_score;
        }
        int FacilityType::getEconomyScore() const{
            return economy_score;
        }

        FacilityCategory FacilityType::getCategory() const{
            return category;
        }

    protected:
        const string name;
        const FacilityCategory category;
        const int price;
        const int lifeQuality_score;
        const int economy_score;
        const int environment_score;
};



class Facility: public FacilityType {

    public:
        Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):FacilityType(name,category,price,lifeQuality_score,economy_score,environment_score),settlementName(settlementName),status(FacilityStatus::UNDER_CONSTRUCTIONS),timeLeft(price){
        }

        Facility::Facility(const FacilityType &type, const string &settlementName):FacilityType(type),settlementName(name),status(FacilityStatus::UNDER_CONSTRUCTIONS),timeLeft(price){
        }

        const string &Facility::getSettlementName() const{
            return settlementName;
        }

        const int Facility::getTimeLeft() const{
            return timeLeft;
        }

        FacilityStatus Facility::step(){
            timeLeft--;

            if(timeLeft==0)
                status=FacilityStatus::OPERATIONAL;
        }

        void Facility::setStatus(FacilityStatus status){
            this->status=status;
        }

        const FacilityStatus& Facility::getStatus() const{
            return status;
        }

        const string toString() const{
            string str=name+" "+settlementName+" ";

            if(category==FacilityCategory::ECONOMY)
                str+="economy ";
            else if (category == FacilityCategory::ENVIRONMENT)
                str+="enviroment ";
            else 
                str+="life quality ";

            str+=std::to_string(price)+" life_quality_score:"+std::to_string(lifeQuality_score)+" economy_score:"+std::to_string(economy_score)+" enviroment_score:"+std::to_string(environment_score);

            if(status==FacilityStatus::OPERATIONAL)
                str+="operational";
            else    
                str+="under_construction";

           str+=std::to_string(timeLeft);

           return str;
            
        }

    private:
        const string settlementName;
        FacilityStatus status;
        int timeLeft;
};