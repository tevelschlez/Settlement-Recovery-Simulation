#include "Plan.h"
#include <iostream>
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;

class Plan {
    public:

        //in this class there is a need to implement the 'Rule Of 5": destructor, copy constructor, copy assignment operator, move constructor, move assignment operator

        //need to initialize the scores to be 0
        Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions) : plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), facilityOptions(facilityOptions),status(PlanStatus::AVALIABLE),life_quality_score(0),economy_score(0),environment_score(0){}



        const int Plan::getConstructionLimit(){
            int construcion_limit;
            if (settlement.getType() == SettlementType::VILLAGE)
                construcion_limit = 1;
            if (settlement.getType() == SettlementType::CITY)
                construcion_limit = 2;
            if (settlement.getType() == SettlementType::METROPOLIS)
                construcion_limit = 3;
            return construcion_limit;
        }

        void Plan::activateSelectionPolicy(){//updating the plan scores according to the facilities that are chosen

            Facility* f_to_push=new Facility(selectionPolicy->selectFacility(facilityOptions),settlement.getName());
            underConstruction.push_back(f_to_push);
            life_quality_score += f_to_push->getLifeQualityScore();
            economy_score += f_to_push->getEconomyScore();
            environment_score += f_to_push->getEnvironmentScore();

            if(underConstruction.size()==getConstructionLimit())
                status = PlanStatus::BUSY;
            if(underConstruction.size()<getConstructionLimit())
                status = PlanStatus::AVALIABLE;
        }

        const int Plan::getlifeQualityScore() const{
            return life_quality_score;
        }
        const int Plan::getEconomyScore() const{
            return economy_score;
        }
        const int Plan::getEnvironmentScore() const{
            return environment_score;
        }

        void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy) { this->selectionPolicy = selectionPolicy; }


        //if a facility is getting off from the under constructuion to the operational it meand there is a free spot to add to the list
        void step(){

            for( int i=0; i<underConstruction.size();i++){
                underConstruction[i]->step();

                if(underConstruction[i]->getStatus()==FacilityStatus::OPERATIONAL){
                    facilities.push_back(underConstruction[i]);
                    underConstruction.erase(underConstruction.begin() + i);
                    while (status==PlanStatus::AVALIABLE)
                        activateSelectionPolicy();

                    i=i-1;
                }
            }
        }
        void printStatus(){
            if(status==PlanStatus::AVALIABLE)
                std::cout << "available" << std::endl;
            else
                std::cout << "busy" << std::endl;
        }
        
        const vector<Facility*> &Plan:: getFacilities() const{
            return facilities;
        }

        //if the length of underConstruction is equal to constuction_limit we can not add a plan

        void addFacility(Facility* facility){//update the plan scores according to the facility
            if(facility->getStatus()==FacilityStatus::UNDER_CONSTRUCTIONS && underConstruction.size()<getConstructionLimit())
                underConstruction.push_back(facility);
            else if (facility->getStatus()==FacilityStatus::UNDER_CONSTRUCTIONS&& underConstruction.size()==getConstructionLimit())
                std::cout << "cannot add this facility to the under construction list" << std::endl;
                
            else
                facilities.push_back(facility);

            life_quality_score += facility->getLifeQualityScore();
            economy_score += facility->getEconomyScore();
            environment_score += facility->getEnvironmentScore();
        }

        const string toString() const{
            string str = "plan id:" + std::to_string(plan_id) + " selection policy:" + selectionPolicy->toString();
            if(status==PlanStatus::AVALIABLE)
                str += " available";
            else
                str += " busy";

            str += " life_quality_score:" + std::to_string(life_quality_score) += " economy_score:" + std::to_string(economy_score) + " enviroment_score:" + std::to_string(environment_score);

            str += " operational_facilities:\n";

            for (int i = 0; i < facilities.size(); i++)
            {
                str += facilities[i]->toString()+"\n";
            }

            str += "underConstruction_facilities:\n";

            for (int i = 0; i < underConstruction.size();i++){
                str += underConstruction[i]->toString() + "\n";
            }

            return str;
        }

        const int getID(){
            return plan_id;
        }

        //rule of 5

        Plan::~Plan(){
            for(auto facility:facilities)
                delete facility;
            for(auto facility:underConstruction)
                delete facility;

            delete selectionPolicy;
        }

        //shallow copy of a constant reference - facilitiyOptions. since it can not be deleted or changed from a diff reference
        Plan::Plan(Plan &other):plan_id(other.plan_id), settlement(other.settlement),selectionPolicy(other.selectionPolicy),status(other.status),facilityOptions(other.facilityOptions),life_quality_score(other.life_quality_score),economy_score(other.economy_score),environment_score(other.environment_score){
            for(auto facility:other.facilities)
                facilities.push_back(facility);
            for(auto facility:other.underConstruction)
                underConstruction.push_back(facility);
        }

        Plan::Plan(Plan &&other) noexcept : plan_id(other.plan_id), settlement(other.settlement), selectionPolicy(other.selectionPolicy),facilityOptions(other.facilityOptions), status(other.status), life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score) ,facilities(std::move(other.facilities)),underConstruction(std::move(other.underConstruction)){}

    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};