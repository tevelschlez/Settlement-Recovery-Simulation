#include "Plan.h"
#include <iostream>
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;


        //in this class there is a need to implement the 'Rule Of 5": destructor, copy constructor, copy assignment operator, move constructor, move assignment operator

        //need to initialize the scores to be 0
    Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions) : plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), status(PlanStatus::AVALIABLE),  facilities(), underConstruction(), facilityOptions(facilityOptions), life_quality_score(0),economy_score(0),environment_score(0),constructionLimit(getConstructionLimit()){}


    const int Plan::getlifeQualityScore() const{
        return life_quality_score;
    }
    const int Plan::getEconomyScore() const{
        return economy_score;
    }
    const int Plan::getEnvironmentScore() const{
        return environment_score;
    }

    void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy) {
        string str = "planID: " + std::to_string(plan_id) + "\n";
        str += "previousPolicy: " + this->selectionPolicy->toString() + "\n";
        str += "newPolicy: " + selectionPolicy->toString() ;
        this->selectionPolicy = selectionPolicy;
        std::cout << str << std::endl;
    }

    void Plan::activateSelectionPolicy(){//choosing the next facility according to the selection polity and adding it to the under construction lst

        Facility* f_to_add=new Facility(selectionPolicy->selectFacility(facilityOptions),settlement.getName());
        addFacility(f_to_add);

        if (underConstruction.size() == constructionLimit)
            status = PlanStatus::BUSY;
    }

    //if a facility is getting off from the under constructuion to the operational it meand there is a free spot to add to the list
    void Plan::step(){

        while (status == PlanStatus::AVALIABLE){
            activateSelectionPolicy();
        }


        for (unsigned int i = 1; i < underConstruction.size(); i++) {//not iterating over 0 since reducing i by 1 can cause a wrap around
        
            FacilityStatus currentStatus = underConstruction[i]->step();

            if(currentStatus == FacilityStatus::OPERATIONAL){
                life_quality_score += underConstruction[i]->getLifeQualityScore();
                economy_score += underConstruction[i]->getEconomyScore();
                environment_score += underConstruction[i]->getEnvironmentScore();
                facilities.push_back(underConstruction[i]);
                underConstruction.erase(underConstruction.begin() + i);
                --i;

                status = PlanStatus::AVALIABLE;
            }
        }

        FacilityStatus currentStatus = underConstruction[0]->step();//safely iterating over the first item
        if(currentStatus == FacilityStatus::OPERATIONAL){
            life_quality_score += underConstruction[0]->getLifeQualityScore();
            economy_score += underConstruction[0]->getEconomyScore();
            environment_score += underConstruction[0]->getEnvironmentScore();
            facilities.push_back(underConstruction[0]);
            underConstruction.erase(underConstruction.begin() + 0);//will remove the first object if needed

            status = PlanStatus::AVALIABLE;
        }
    }

    void Plan::printStatus(){
        string str = "PlanID:" + std::to_string(plan_id) + "\n";
        str += "SettlementName:" + settlement.getName() + "\n";
        str += "PlanStatus:";
        if (status == PlanStatus::AVALIABLE)
            str += "AVAILABLE";
        else
            str += "BUSY";
        str += "\nSelectionPolicy:" + selectionPolicy->toString() + "\n";
        str += "LifeQualityScore:" + std::to_string(life_quality_score) + "\n";
        str += "EconomyScore:" + std::to_string(economy_score) + "\n";
        str += "EnviromentScore:" + std::to_string(environment_score);
        std::cout << str << std::endl;

        for (auto *facility : getUnderConstruction())
           std::cout<<facility->toString()<<std::endl;

        for (auto *facility : getFacilities())
            std::cout << facility->toString() << std::endl;
    }

    const vector<Facility*> &Plan:: getFacilities() const{
        return facilities;
    }

    //if the length of underConstruction is equal to constuction_limit we can not add a plan

    void Plan::addFacility(Facility* facility){//update the plan scores according to the facility
        if(facility->getStatus()==FacilityStatus::UNDER_CONSTRUCTIONS && status==PlanStatus::AVALIABLE)
            underConstruction.push_back(facility);

        else if (facility->getStatus()==FacilityStatus::UNDER_CONSTRUCTIONS&& status==PlanStatus::BUSY)
            std::cout << "cannot add this facility to the under construction list" << std::endl;

        else
            facilities.push_back(facility);

    }

    const string Plan::toString() const{
        string str = "PlanID:" + std::to_string(plan_id) + "\nSettlementName:" + settlement.getName() + "\nLifeQuality_Score:" + std::to_string(life_quality_score) + "\nEconomy_Score:" + std::to_string(economy_score) + "\nEnviroment_Score:" + std::to_string(environment_score);
        return str;
    }

    int Plan::getID() const{
        return plan_id;
    }

    // new methods
    const Settlement &Plan::getSettlement() const{
        return settlement;
    }

    const vector<Facility*> &Plan::getUnderConstruction() const{
        return underConstruction;
    }
    SelectionPolicy* Plan::getSelectionPolicy() const{
        return selectionPolicy;
    }
    //



    bool Plan::comparePolicy(const string &policyToCompare){
        if(policyToCompare==selectionPolicy->toString())
            return true;
        return false;
    } 
    
    //Rule of 5

    Plan::Plan(const Plan &other, const Settlement& other_settlement) :
    plan_id(other.plan_id),
    settlement(other_settlement),
    selectionPolicy(other.selectionPolicy->clone()),
    status(other.status),
    facilities(),
    underConstruction(),
    facilityOptions(other.facilityOptions),
    life_quality_score(other.life_quality_score),
    economy_score(other.economy_score),
    environment_score(other.economy_score),
    constructionLimit(other.constructionLimit)
    {
    for (Facility* facility : other.facilities){
            facilities.push_back(new Facility(*facility));
        }
    for (auto facility : other.underConstruction){
            underConstruction.push_back(new Facility(*facility));
        }
}

   

    //destructor
    Plan::~Plan(){ 
        for (auto* facility : facilities) {
        delete facility;
        }
        for (auto* facility : underConstruction) {
        delete facility;
        }
        delete selectionPolicy;
        
        }
        
    //copy constructor
    Plan::Plan(const Plan& other) 
    : plan_id(other.plan_id), settlement(other.settlement), 
    selectionPolicy(other.selectionPolicy->clone()),
    status(other.status), facilities(), underConstruction(), 
    facilityOptions(other.facilityOptions),
    life_quality_score(other.life_quality_score),
    economy_score(other.economy_score),
    environment_score(other.environment_score),
    constructionLimit(other.constructionLimit) {
    

    for (const auto* facility : other.facilities) {
        facilities.push_back(new Facility(*facility));
    }
    for (const auto* facility : other.underConstruction) {
        underConstruction.push_back(new Facility(*facility));
    }
    }

    //move copy constructor
    Plan::Plan(Plan &&other) 
        : plan_id(other.plan_id),
        settlement(std::move(other.settlement)),
        selectionPolicy(other.selectionPolicy), 
        status(std::move(other.status)),
        facilities(std::move(other.facilities)),
        underConstruction(std::move(other.underConstruction)),
        facilityOptions(std::move(other.facilityOptions)),
        life_quality_score(other.life_quality_score),
        economy_score(other.economy_score),
        environment_score(other.environment_score),
        constructionLimit(other.constructionLimit)
    {

        other.selectionPolicy = nullptr;
        other.facilities.clear();
        other.underConstruction.clear();
    }

const int Plan::getConstructionLimit(){
    unsigned int construcion_limit;
    if (settlement.getType() == SettlementType::VILLAGE)
        construcion_limit = 1;
    if (settlement.getType() == SettlementType::CITY)
        construcion_limit = 2;
    if (settlement.getType() == SettlementType::METROPOLIS)
        construcion_limit = 3;
    return construcion_limit;
}