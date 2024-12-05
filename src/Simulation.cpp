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


        Simulation::Simulation(const string &configFilePath) 
        : isRunning(false), planCounter(-1), actionsLog(), plans(), settlements(), facilitiesOptions() { 
            loadConfigFile(configFilePath); 
        }
        void Simulation::loadConfigFile(const string &configFilePath){
            std::ifstream configFile(configFilePath);

            if (!configFile.is_open())
            {
                std::cout << "ERROR - opening config file failed: " << configFilePath << std::endl;
                return;
            }

            string line;
            
            while (std::getline(configFile, line))
            {
                vector<string> arguments = Auxiliary::parseArguments(line);

                const string &initializerType = arguments[0];

                if (initializerType == "plan")
                {
                    const string &settlementName = arguments[1];
                    SelectionPolicy *selection = getSelectionPolicy(arguments[2]);

                    addPlan(getSettlement(settlementName), selection);
                }

                if (initializerType == "settlement")
                {
                    const string &settlementName = arguments[1];
                    SettlementType settlementType;
                    switch (stoi(arguments[2]))
                    {
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
                    addSettlement(new Settlement(settlementName, settlementType));
                }

                if (initializerType == "facility")
                {
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

                    addFacility(FacilityType(facilityName, category, price, lifeQuality_score, economy_score, enviroment_Score));
                }
            }

            configFile.close();
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

                if(actionType=="plan"){
                    const string &settlementName = arguments[1];
                    const string &policy = arguments[2];

                    action = new AddPlan(settlementName, policy);

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
                            action = new AddSettlement(settlementName,settlementType);
                        
                }

                if (actionType=="facility"){
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

                        action = new AddFacility(facilityName, category, price, lifeQuality_score, economy_score, enviroment_Score);
                
                }

                if(actionType =="step"){
                    const int &num_of_steps = stoi(arguments[1]);
                    action = new SimulateStep(num_of_steps);
                }

                if(actionType=="planStatus"){
                    const int &plan_id = stoi(arguments[1]);
                    action = new PrintPlanStatus(plan_id);
                }

                if(actionType=="changePolicy"){
                    const int &plan_id = stoi(arguments[1]);
                    const string &policy = arguments[2];

                    action = new ChangePlanPolicy(plan_id,policy);
                }

                if(actionType=="log"){
                    action = new PrintActionsLog();
                }

                if(actionType=="close"){
                    action = new Close();
                }

                if(actionType=="backup"){
                    action = new BackupSimulation();
                }

                if(actionType=="restore"){
                    action = new RestoreSimulation();
                    action->act(*this);
                    continue;
                }

                action->act(*this);
            }
        }

        void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
        {
            if(!isSettlementExists(settlement.getName())||selectionPolicy==nullptr)
                throw std::invalid_argument("Cannot create this plan");
            else
            {
                planCounter++;
                plans.push_back(Plan(planCounter, settlement, selectionPolicy, facilitiesOptions));
            }
        }

        void Simulation::addAction(BaseAction *action)
        {
            actionsLog.push_back(action);
        }

        bool Simulation::addSettlement(Settlement *settlement){
            if(isSettlementExists(settlement->getName())){
                throw std::invalid_argument("Settlement already exists");
            }
                settlements.push_back(settlement);
                return true;
        }

        bool Simulation::addFacility(FacilityType facility){
            if (isFacilityExists(facility.getName())){
                throw std::invalid_argument("Facility already exists");
            }
                facilitiesOptions.push_back(facility);
                return true;
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

        bool Simulation::isPlanExists(const int &ID){
            for (auto plan : plans)
            {
                if (plan.getID() == ID)
                    return true;
            }
            return false;
        }


         SelectionPolicy *Simulation::getSelectionPolicy(const string &policy){//returning null if the policy does not exist
             SelectionPolicy *selection;

            if (policy == "nve")
                selection = new NaiveSelection();
            else if (policy == "bal")
                selection = new BalancedSelection(0, 0, 0);
            else if (policy == "eco")
                selection = new EconomySelection();
            else if (policy == "env")
                selection = new SustainabilitySelection();
            else
            {
                selection = nullptr;
            }
            return selection;
        }


        Settlement &Simulation::getSettlement(const string &settlementName){
            for(auto &settlement:settlements){
                if (settlement->getName()==settlementName){
                    return *settlement;
                }
            }  
            throw std::invalid_argument("Settlement does not exist");
        }

        Plan &Simulation::getPlan(const int planID){
            for(auto &plan:plans){
                if(plan.getID()==planID)
                    return plan;
            }
            throw std::invalid_argument("Plan does not exist");
        }
        

        void Simulation::step(){//should iterate over all plans and perform step for each of them
        //in the action class, this method will be performed as many times as the user decides
            for (auto plan :plans){
                plan.step();
            }
        }

        void Simulation::close(){
            for(Plan plan :plans){
                std::cout << plan.toString() << std::endl;
            }
            isRunning = false;
        }
        void Simulation::open(){
            isRunning = true;
        }

        void Simulation::printActionsLog(){
            for(BaseAction *action:actionsLog){
                std::cout << action->toString() << std::endl;
            }
        }

// Rule Of Five:

        // Destructor
        Simulation::~Simulation(){
            for(auto &settlement:settlements){
                delete settlement;
            }
            for(auto &action:actionsLog){
                delete action;
            } 
        }
        // Copy constructor
        Simulation::Simulation(const Simulation &other) : 
        isRunning(other.isRunning), planCounter(other.planCounter), actionsLog(),
        plans(other.plans),settlements(), facilitiesOptions(other.facilitiesOptions) {

            for (const auto &settlement : other.settlements) {
                settlements.push_back(new Settlement(*settlement)); 
            }
            for (const auto &action : other.actionsLog) {
                actionsLog.push_back(action->clone());
            }
        }

        // Move constructor
        Simulation::Simulation(Simulation &&other) noexcept: 
            isRunning(other.isRunning), planCounter(other.planCounter), 
            actionsLog(std::move(other.actionsLog)), plans(other.plans), 
            settlements(std::move(other.settlements)), facilitiesOptions(other.facilitiesOptions) {}
        

        // Copy assignment
        Simulation& Simulation::operator=(const Simulation& other) {
            if (this != &other) {

                isRunning = other.isRunning;
                planCounter = other.planCounter;

                for (auto &action : actionsLog) {
                    delete action;
                }
                actionsLog.clear();

                for (const auto &action : other.actionsLog) {
                    actionsLog.push_back(action->clone());
                }
        
                for (auto &settlement : settlements) {
                    delete settlement;
                }
                settlements.clear();
                for (const auto &settlement : other.settlements) {
                    settlements.push_back(new Settlement(*settlement));
                }

                facilitiesOptions.clear();
                for (const auto &facility : other.facilitiesOptions) {
                    facilitiesOptions.push_back(facility);
                }
        
                for (const Plan &plan : other.plans) {
                    Settlement &settlement = getSettlement(plan.getSettlement().getName());
                    Plan newPlan = Plan(plan.getID(), settlement, plan.getSelectionPolicy(), facilitiesOptions);
                    for (Facility *facility : plan.getFacilities()) {
                        newPlan.addFacility(new Facility(*facility));
                    }
                    for (Facility *facility : plan.getUnderConstruction()) {
                        newPlan.addFacility(new Facility(*facility));
                    }
                    plans.push_back(newPlan);
                }
            }

            return *this;   
        }

        // Move assignment
        Simulation& Simulation::operator=(Simulation&& other) noexcept {
            if (this != &other) {
                for (auto& action : actionsLog) {
                    delete action;
                }
                actionsLog.clear();

                for (auto& settlement : settlements) {
                    delete settlement;
                }
                settlements.clear();

                isRunning = other.isRunning;
                planCounter = other.planCounter;

                actionsLog = std::move(other.actionsLog);
                plans = std::move(other.plans);
                settlements = std::move(other.settlements);
                facilitiesOptions = std::move(other.facilitiesOptions);
            }
            return *this;
        }
    



