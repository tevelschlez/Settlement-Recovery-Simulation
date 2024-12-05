#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include <fstream>
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        void loadConfigFile(const string &configFilePath);
        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        bool isFacilityExists(const string &facilityName);
        bool isPlanExists(const int &ID);
        SelectionPolicy *getSelectionPolicy(const string &policy);
        Settlement &getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);
        void step();
        void close();
        void open();
        void printActionsLog();

        // Rule of Five:
        ~Simulation();                         // Destructor
        Simulation(const Simulation &other);   // Copy constructor
        Simulation(Simulation &&other) noexcept; // Move constructor
        Simulation& operator=(const Simulation& other); // Copy assignment 
        Simulation& operator=(Simulation&& other) noexcept; // Move assignment 

    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;
        
};