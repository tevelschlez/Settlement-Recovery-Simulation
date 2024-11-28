#include "Plan.h"
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;

class Plan {
    public:

        Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions) : plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), facilityOptions(facilityOptions),status(PlanStatus::AVALIABLE){}


        void Plan::activateSelectionPolicy(){
            int construcion_limit;
            if(settlement.getType()==SettlementType::VILLAGE)
                construcion_limit=1;
            if(settlement.getType()==SettlementType::CITY)
                construcion_limit=2;
            if(settlement.getType()==SettlementType::METROPOLIS)
                construcion_limit=3;
            for(int i=1;i<=construcion_limit;i++){
                Facility* f_to_push=new Facility(selectionPolicy->selectFacility(facilityOptions),settlement.getName());
                underConstruction.push_back(f_to_push);
            }

            status=PlanStatus::BUSY;
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

        void step(){

            for( int i=0; i<underConstruction.size();i++){
                underConstruction[i]->step();

                if(underConstruction[i]->getStatus()==FacilityStatus::OPERATIONAL){
                    facilities.push_back(underConstruction[i]);
                    underConstruction.erase(underConstruction.begin() + i);
                    i=i-1;
                }
            }
        }
        void printStatus(){

        }
        const vector<Facility*> &Plan:: getFacilities() const{
            return facilities;
        }
        
        void addFacility(Facility* facility){
            if(facility->getStatus()==FacilityStatus::UNDER_CONSTRUCTIONS)
                underConstruction.push_back(facility);
            else
                facilities.push_back(facility);
        }

        const string toString() const{

        }

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