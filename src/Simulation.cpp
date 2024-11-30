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
                        std::cout << "settlement does not exist" << std::endl;
                    else{

                    action = new AddPlan(settlementName, arguments[2]);

                    actionsLog.push_back(action);
                    planCounter++;
                    plans.push_back(Plan(planCounter, getSettlement(settlementName), selection, facilitiesOptions));
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
        // change status to AVALIABLE - not completed

        void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
            if (!isSettlementExists(settlement.getName()) || !isValidSelectionPolicy(selectionPolicy->toString())){
                std::cout << "Cannot create this plan" << std::endl;
                return;
            }
            planCounter++;
            plans.push_back(Plan(planCounter, settlement, selectionPolicy, facilitiesOptions));
    
        }
        

        void Simulation::addAction(BaseAction *action){
            actionsLog.push_back(action);

        }
        bool Simulation::addSettlement(Settlement *settlement){
            if (isSettlementExists(settlement->getName())){
                    std::cout << "settlement already exists" << std::endl;
                    return false;
                }
            settlements.push_back(settlement);
            return true;
        }

        bool Simulation::addFacility(FacilityType facility){
            for(auto &facility:facilitiesOptions)
                if(facility.getName()==facility.getName()){
                    std::cout << "facility already exists" << std::endl;
                    return false;
                }
            facilitiesOptions.push_back(facility);
            return true;
        }

        bool Simulation::isSettlementExists(const string &settlementName){
            for(auto &settlement:settlements)
                if(settlement->getName()==settlementName)
                    return true;
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

        void Simulation::step(){
            for(auto &plan:plans)
                plan.step();

        }
        void Simulation::close(){
            for (const auto& plan : plans) {
                std::cout << "PlanID: " << plan.getID() << std::endl;
                std::cout << "SettlementName: " << plan.getSettlement().getName() << std::endl;
                std::cout << "LifeQuality_Score: " << plan.getlifeQualityScore() << std::endl;
                std::cout << "Economy_Score: " << plan.getEconomyScore() << std::endl;
                std::cout << "Environment_Score: " << plan.getEnvironmentScore() << std::endl;
            }
            for (auto &settlement : settlements) {
                delete settlement;
            }
            settlements.clear();
            isRunning = false;
        }
        void Simulation::open(){
            isRunning = true;
        }

        bool Simulation::isValidSelectionPolicy(const string &selectionPolicy){
            return selectionPolicy=="nve" || selectionPolicy=="bal" || selectionPolicy=="eco" || selectionPolicy=="env";
        }

    // Rule of Five

    // Destructor
        ~Simulation() {
        for (auto& settlement : settlements) {
            delete settlement;
        }
        settlements.clear();

        for (auto& action : actionsLog) {
            delete action;
        }
        actionsLog.clear();
    }
    

    // Copy Constructor
    Simulation(const Simulation& other)
        : isRunning(other.isRunning),
        planCounter(other.planCounter),
        facilitiesOptions(other.facilitiesOptions) {
        for (const auto& settlement : other.settlements) {
            settlements.push_back(new Settlement(*settlement));
        }
        for (const auto& action : other.actionsLog) {
            actionsLog.push_back(action->clone()); // Assuming BaseAction has a clone method
        }
        plans = other.plans; // `Plan` must handle its own Rule of Five
    }

    // Copy Assignment Operator
    Simulation& operator=(const Simulation& other) {
        if (this != &other) {
            // Free existing resources
            for (auto& settlement : settlements) {
                delete settlement;
            }
            settlements.clear();

            for (auto& action : actionsLog) {
                delete action;
            }
            actionsLog.clear();

            // Copy data
            isRunning = other.isRunning;
            planCounter = other.planCounter;
            facilitiesOptions = other.facilitiesOptions;

            // Deep copy settlements
            for (const auto& settlement : other.settlements) {
                settlements.push_back(new Settlement(*settlement));
            }

            // Deep copy actions
            for (const auto& action : other.actionsLog) {
                actionsLog.push_back(action->clone());
            }

            plans = other.plans; 
        }
        return *this;
    }

    // Move Constructor
    Simulation(Simulation&& other) noexcept
        : isRunning(other.isRunning),
        planCounter(other.planCounter),
        settlements(std::move(other.settlements)),
        actionsLog(std::move(other.actionsLog)),
        plans(std::move(other.plans)),
        facilitiesOptions(std::move(other.facilitiesOptions)) {
        other.isRunning = false;
    }

    // Move Assignment Operator
    Simulation& operator=(Simulation&& other) noexcept {
        if (this != &other) {
            // Free existing resources
            for (auto& settlement : settlements) {
                delete settlement;
            }
            settlements.clear();

            for (auto& action : actionsLog) {
                delete action;
            }
            actionsLog.clear();

            // Move data
            isRunning = other.isRunning;
            planCounter = other.planCounter;
            settlements = std::move(other.settlements);
            actionsLog = std::move(other.actionsLog);
            plans = std::move(other.plans);
            facilitiesOptions = std::move(other.facilitiesOptions);

            other.isRunning = false;
        }
        return *this;
    }
};

