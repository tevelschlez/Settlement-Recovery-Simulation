#include "Simulation.h"
#include "Auxiliary.h"
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
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

        Simulation(const string &configFilePath){
    
        }

        void start(){// should print: "The simulation has started". should also start the loop of inputs from the user.

        }
        void addPlan(){

        }
        void addAction(){

        }
        bool addSettlement(Settlement *settlement){

        }
        bool addFacility(FacilityType facility){

        }
        bool isSettlementExists(const string &settlementName){

        }
        Settlement &getSettlement(const string &settlementName){

        }
        Plan &getPlan(const int planID){

        }
        void step(){

        }
        void close(){

        }
        void open(){

        }

};

