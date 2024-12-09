#include "Action.h"
#include <string>
#include <vector>
#include "Simulation.h"
#include <iostream>

extern Simulation *backup;

//BaseAction Class
        BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::COMPLETED) {}
        
        ActionStatus BaseAction::getStatus() const { return status; }

        void BaseAction::complete() {status = ActionStatus::COMPLETED; }

        void BaseAction::error(string errorMsg) { this->errorMsg = errorMsg;
            status = ActionStatus::ERROR;
            std::cout << "Error:" + errorMsg << std::endl;
        }

        const string &BaseAction::getErrorMsg() const { return errorMsg; }

        const string BaseAction::statusToString() const{
            string str="";
            if (getStatus() == ActionStatus::COMPLETED)
                str += "COMPLETED";
            else if (getStatus() == ActionStatus::ERROR)
                str + "ERROR";
            return str;
        }


//SimulateStep Class

        SimulateStep::SimulateStep(const int numOfSteps):numOfSteps(numOfSteps){}

        void SimulateStep::act(Simulation &simulation) {//does not return error
            for (int i = 0; i < numOfSteps;i++){
                simulation.step();
            }
            complete();
            simulation.addAction(this);
        }

        const string SimulateStep::toString() const {
            string str = "step " + std::to_string(numOfSteps) + " " + statusToString();
            return str;
        }

        SimulateStep *SimulateStep::clone() const { return new SimulateStep(*this); }


//AddPlan Class
        AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):settlementName(settlementName),selectionPolicy(selectionPolicy){}

        void AddPlan::act(Simulation &simulation) {
            SelectionPolicy* policy=simulation.getSelectionPolicy(0,0,0,selectionPolicy);
            try{
                simulation.addPlan(simulation.getSettlement(settlementName), policy);
                complete();
            }
            catch (const std::exception &e){
                error(e.what());
            }

            simulation.addAction(this);
        }

        const string AddPlan::toString() const {
            string str = "plan " + settlementName + " " + selectionPolicy+" "+statusToString();
            return str;
        }

        AddPlan *AddPlan::clone() const { return new AddPlan(*this); }




        // AddSettlement
        AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType):settlementName(settlementName),settlementType(settlementType){}
        
        void AddSettlement::act(Simulation &simulation) {
            try{
                simulation.addSettlement(new Settlement(settlementName,settlementType));
                complete();
            }
            catch(const std::exception &e){
                error(e.what());
            }

            simulation.addAction(this);
        }

        AddSettlement *AddSettlement::clone() const { return new AddSettlement(*this); }


        const string AddSettlement:: toString() const {
            string str = "settlement " + settlementName + " ";
            if(settlementType==SettlementType::CITY)
                str += "1 ";
            else if(settlementType==SettlementType::METROPOLIS)
                str += "2 ";
            else
                str += "0 ";

            str += statusToString();
            return str;
        }



//AddFacility Class
        AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):facilityName(facilityName),facilityCategory(facilityCategory),price(price),lifeQualityScore(lifeQualityScore),economyScore(economyScore),environmentScore(environmentScore){}
        
        void AddFacility::act(Simulation &simulation) {
            try{
                simulation.addFacility(FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore));
                complete();
            }
            catch(std::exception &e){
                error(e.what());
            }

            simulation.addAction(this);
        }

        AddFacility *AddFacility::clone() const { return new AddFacility(*this); }


        const string AddFacility:: toString() const {
            string str = "facility " + facilityName;
            if(facilityCategory==FacilityCategory::LIFE_QUALITY)
                str += " 0 ";
            else if (facilityCategory == FacilityCategory::ECONOMY)
                str += " 1 ";
            else 
                str += " 2 ";

            str += std::to_string(price) + " " + std::to_string(lifeQualityScore) + " " + std::to_string(economyScore) + " " + std::to_string(environmentScore)+" "+statusToString();
            return str; 
        }



//PrintPlanStatus Class
        PrintPlanStatus::PrintPlanStatus(int planId):planId(planId){}

        void PrintPlanStatus::act(Simulation &simulation) {
            if(!simulation.isPlanExists(planId))
                error("Plan doesn't exist");
            else{
                Plan &plan = simulation.getPlan(planId);
                plan.printStatus();
                complete();
            }

            simulation.addAction(this);
        }
        PrintPlanStatus *PrintPlanStatus::clone() const { return new PrintPlanStatus(*this); }


        const string PrintPlanStatus:: toString() const {
            string str = "planStatus " + std::to_string(planId) + " " + statusToString();
            return str;
        }




//ChangePlanPolicy Class
        ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy):planId(planId),newPolicy(newPolicy){}

        void ChangePlanPolicy::act(Simulation &simulation) {
            string e = "Cannot change selection policy";
            if (!simulation.isPlanExists(planId))
                error(e);
            else{
                Plan &plan = simulation.getPlan(planId);
                if(plan.comparePolicy(newPolicy))
                    error(e);
                else{
                    SelectionPolicy *policy = simulation.getSelectionPolicy(plan.getTotalLifeQualityScore(), plan.getTotalEconomyScore(), plan.getTotalEnviromentScore(), newPolicy);
                    if(policy==nullptr)
                        error(e);
                    else{
                        plan.setSelectionPolicy(policy);
                        complete();
                    }
                }
            }

            simulation.addAction(this);
        }

        ChangePlanPolicy *ChangePlanPolicy::clone() const { return new ChangePlanPolicy(*this); }


        const string ChangePlanPolicy::toString() const {
            string str = "changePolicy " + std::to_string(planId) + " " + newPolicy + " " + statusToString();
            return str;
        }

//PrintActionsLog Class
        PrintActionsLog::PrintActionsLog(){}
        void PrintActionsLog::act(Simulation &simulation) {
            simulation.printActionsLog();
            complete();
            simulation.addAction(this);
        }

        PrintActionsLog *PrintActionsLog::clone() const { return new PrintActionsLog(*this); }

        const string PrintActionsLog:: toString() const {
            string str = "log "+statusToString();
            return str;
        }

//Close Class
        Close::Close(){}
        void Close::act(Simulation &simulation) {
            simulation.close();
            complete();
            simulation.addAction(this);
        }
        Close *Close::clone() const { return new Close(*this); }

        const string Close::toString() const {
            string str = "close " + statusToString();
            return str;
        }

//BackupSimulation Class
        BackupSimulation::BackupSimulation(){}

        void BackupSimulation::act(Simulation &simulation) {
            if (backup != nullptr)
            {
                delete backup;
            }
            backup = new Simulation(simulation);
            complete();
            simulation.addAction(this);
        }

        BackupSimulation *BackupSimulation::clone() const { return new BackupSimulation(*this); }

        const string BackupSimulation::toString() const {
            string str = "backup " + statusToString();
            return str;
        }

//RestoreSimualtion Class
        RestoreSimulation::RestoreSimulation(){}

        void RestoreSimulation::act(Simulation &simulation) {
            if(backup==nullptr)
                error("No backup available");
            else{
                 simulation = *backup;
                complete();
            }
            simulation.addAction(this);
        }

        RestoreSimulation *RestoreSimulation::clone() const { return new RestoreSimulation(*this); }

        const string RestoreSimulation::toString() const {
            string str = "restore " + statusToString();
            return str;
        }