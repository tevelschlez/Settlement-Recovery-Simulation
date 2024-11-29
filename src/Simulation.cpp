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

static int Plan_id_counter = 0;

class Simulation{
    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;

    public:

        Simulation::Simulation(const string &configFilePath):isRunning(false),planCounter(0){}

        void Simulation::start(){// should print: "The simulation has started". should also start the loop of inputs from the user.

            open();//isRunning =true

            while (isRunning)
            {
                BaseAction *action;
                string command;

                getline(std::cin, command);

                vector<string> arguments = Auxiliary::parseArguments(command);

                const string &actionType = arguments[0];

                if(actionType=="Plan"){
                    const string &settlementName = arguments[1];
                    const string &SelectionPolicy = arguments[2];

                    if(!isSettlementExists(settlementName))
                        std::cout << "settlement does not exist" << std::endl;
                    else{

                    action = new AddPlan(settlementName, SelectionPolicy);

                    actionsLog.push_back(action);
                    //plans.push_back(new Plan(Plan_id_counter++,std::find(settlements.begin(),));
                    }
                }

                if (actionType=="settlement"){
                    const string &selttlementName = arguments[1];
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

                        action = new AddSettlement(selttlementName, settlementType);
                        actionsLog.push_back(action);
                        settlements.push_back(new Settlement(selttlementName, settlementType));
                }

                if (actionType=="Facility"){
                    const string &facilityName = arguments[1];
                    FacilityCategory category;
                    const string &price = arguments[3];
                    const string &lifeQuality_score = arguments[4];
                    const string &economy_score = arguments[5];
                    const string &enviroment_Score = arguments[6];

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

                    action = new AddFacility(facilityName, category, std::stoi(price), std::stoi(lifeQuality_score), std::stoi(economy_score), std::stoi(enviroment_Score));
                    actionsLog.push_back(action);
                    facilitiesOptions.push_back(FacilityType(facilityName,category,std::stoi(price),std::stoi(lifeQuality_score),std::stoi(economy_score),std::stoi(enviroment_Score)));
                    }

                    std::cout << "The simulation has started" << std::endl;
            }
        }
        void Simulation::addPlan(){

        }
        void Simulation::addAction(){

        }
        bool Simulation::addSettlement(Settlement *settlement){

        }
        bool Simulation::addFacility(FacilityType facility){

        }
        bool Simulation::isSettlementExists(const string &settlementName){

        }
        Settlement &Simulation::getSettlement(const string &settlementName){

        }
        Plan &Simulation::getPlan(const int planID){

        }
        void Simulation::step(){

        }
        void Simulation::close(){

        }
        void Simulation::open(){
            isRunning = true;
        }
};

