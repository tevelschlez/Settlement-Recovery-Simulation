#include "Simulation.h"
#include "Auxiliary.h"
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "Action.h"
using std::string;
using std::vector;

#include <fstream>
#include <string>
#include <iostream>

class Auxiliary;


class Simulation{
    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;

    public:

        Simulation::Simulation(const string &configFilePath):isRunning(false),planCounter(0){

        }

        void Simulation::start(){// should print: "The simulation has started". should also start the loop of inputs from the user.
            open();//isRunning =true
            string command;
            std::cout << "The simulation has started" << std::endl;


            while (isRunning)
            {
                //analyziation of the user input

                string command;

                getline(std::cin, command);
                BaseAction *action;

                vector<string> arguments = Auxiliary::parseArguments(command);

                const string &actionType = arguments[0];

                if(actionType=="Plan"){
                 const string &settlementName = arguments[1];
                    SelectionPolicy *selection;

                    if(arguments[2]=="nve")
                        selection = new NaiveSelection();
                    else if (arguments[2]=="bal")
                        selection = new BalancedSelection(0,0,0);
                  else if (arguments[2]=="eco")
                        selection = new EconomySelection();
                    else if (arguments[2]=="env")
                        selection=new SustainabilitySelection();

                    if(!isSettlementExists(settlementName))
                        throw std::invalid_argument("the settlement does not exist");
                 else{
                     action = new AddPlan(settlementName, arguments[2]);
                     //whats the add plan method should do? it soes not get any parameters
                 }
              }

                if (actionType=="settlement"){
                    const string &settlementName = arguments[1];
                    SettlementType settlementType;
                    switch(stoi(arguments[2])){
                        case 0:
                            settlementType = SettlementType::VILLAGE;
                            break;
                        case 1:
                            settlementType = SettlementType::CITY;
                            break;
                        case 2:
                            settlementType = SettlementType::METROPOLIS;
                            break;
                        default:
                            throw std::invalid_argument("Unkown settlement type");
                        }

                        if(isSettlementExists(settlementName))
                            throw std::invalid_argument("the settlement already exist - choose a uniqe name");
                        else{
                            action = new AddSettlement(settlementName,settlementType);
                            action->act(*this);
                        }
                                }

                if (actionType=="Facility"){
                    const string &facilityName = arguments[1];
                    FacilityCategory category;
                    const int &price = stoi(arguments[3]);
                    const int &lifeQuality_score = stoi(arguments[4]);
                    const int &economy_score = stoi(arguments[5]);
                    const int &enviroment_Score = stoi(arguments[6]);

                    switch (stoi(arguments[2]))
                    {
                    case 0:
                        category = FacilityCategory::LIFE_QUALITY;
                        break;
                    case 1:
                        category = FacilityCategory::ECONOMY;
                        break;
                    case 2:
                        category = FacilityCategory::ENVIRONMENT;
                        break;
                    default:
                        throw std::invalid_argument("Unkown facility category");
                    }

                    if(isFacilityExists(facilityName))
                        throw std::invalid_argument("facility already exist - choose a uniqe name");
                    else{
                        action = new AddFacility(facilityName, category, price, lifeQuality_score, economy_score, enviroment_Score);
                        action->act(*this);
                    }
                }
            }
        }

        void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
        {
        }
        void Simulation::addAction(BaseAction *action)
        {
        }
        bool Simulation::addSettlement(Settlement *settlement){

        }
        bool Simulation::addFacility(FacilityType facility){

        }
        bool Simulation::isSettlementExists(const string &settlementName){
            for(auto &settlement:settlements){
                if(settlement->getName()==settlementName)
                    return true;
            }
            return false;
        }

        bool Simulation::isFacilityExists(const string &facilityName){
            for (auto &facility:facilitiesOptions){
                if(facility.getName()==facilityName)
                    return true;
            }
            return false;
        }

        Settlement &Simulation::getSettlement(const string &settlementName){
            for(auto &settlement:settlements){
                if (settlement->getName()==settlementName)
                    return *settlement;
            }
        }

        Plan &Simulation::getPlan(const int planID){
            for(auto &plan:plans)
                if(plan.getID()==planID)
                    return plan;
        }

        void Simulation::step(){//should iterate over all plans and perform step for each of them
        //in the action class, this method will be performed as many times as the user decides
            for (auto plan :plans){
                plan.step();
            }
        }

        void Simulation::close(){
            isRunning = false;
        }
        void Simulation::open(){
            isRunning = true;
        }
};

