#include "Action.h"
#include <string>
#include <vector>
#include "Simulation.h"
#include <iostream>

//BaseAction Class
        BaseAction::BaseAction():errorMsg(""){}
        ActionStatus BaseAction::getStatus() const { return status; }

        void BaseAction::complete() { status = ActionStatus::COMPLETED; }

        void BaseAction::error(string errorMsg) { this->errorMsg = errorMsg;
            status = ActionStatus::ERROR;
            std::cout << "Error:" + errorMsg << std::endl;
        }

        const string &BaseAction::getErrorMsg() const { return errorMsg; }


//SimulateStep Class

        SimulateStep::SimulateStep(const int numOfSteps):numOfSteps(numOfSteps){}

        void    SimulateStep::act(Simulation &simulation) {//does not return error
            for (int i = 0; i < numOfSteps;i++)
                simulation.step();
            simulation.addAction(this);
            complete();
        }

        const string SimulateStep::toString() const {}
        SimulateStep *SimulateStep::clone() const { return new SimulateStep(*this); }


//AddPlan Class
        AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):settlementName(settlementName),selectionPolicy(selectionPolicy){}

        void AddPlan::act(Simulation &simulation) {
            SelectionPolicy* policy=simulation.getSelectionPolicy(selectionPolicy);
            try{
                simulation.addPlan(simulation.getSettlement(settlementName), policy);
                simulation.addAction(this);
                complete();
            }
            catch (const std::exception &e){
                error(e.what());
            }
        }

        const string AddPlan::toString() const {}
        AddPlan *AddPlan::clone() const {}



//AddSettlement
        AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType):settlementName(settlementName),settlementType(settlementType){}
        
        void AddSettlement::act(Simulation &simulation) {
            try{
                simulation.addSettlement(new Settlement(settlementName,settlementType));
                simulation.addAction(this);
                complete();
            }
            catch(const std::exception &e){
                error(e.what());
            }
        }

        AddSettlement *AddSettlement::clone() const {}
        const string AddSettlement:: toString() const {}
        




//AddFacility Class
        AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):facilityName(facilityName),facilityCategory(facilityCategory),price(price),lifeQualityScore(lifeQualityScore),economyScore(economyScore),environmentScore(environmentScore){}
        
        void AddFacility::act(Simulation &simulation) {
            try{
                simulation.addFacility(FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore));
                simulation.addAction(this);
                complete();
            }
            catch(std::exception &e){
                error(e.what());
            }
        }
      
        AddFacility *AddFacility::clone() const {}
        const string AddFacility:: toString() const {}


//PrintPlanStatus Class
        PrintPlanStatus::PrintPlanStatus(int planId):planId(planId){}

        void PrintPlanStatus::act(Simulation &simulation) {
            if(!simulation.isPlanExists(planId))
                error("Plan doesn't exist");
            else{
                Plan plan = simulation.getPlan(planId);
                plan.printStatus();
                simulation.addAction(this);
                complete();
            }
        }
        PrintPlanStatus *PrintPlanStatus::clone() const {}
        const string PrintPlanStatus:: toString() const {}




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
                    SelectionPolicy *policy = simulation.getSelectionPolicy(newPolicy);
                    if(policy==nullptr)
                        error(e);
                    else{
                        plan.setSelectionPolicy(policy);
                        simulation.addAction(this);
                        complete();
                    }
                }
            }
        }

        ChangePlanPolicy *ChangePlanPolicy::clone() const {}
        const string ChangePlanPolicy::toString() const {}




//PrintActionsLog Class
        PrintActionsLog::PrintActionsLog(){}
        void PrintActionsLog::act(Simulation &simulation) {

        }

        PrintActionsLog *PrintActionsLog::clone() const {}
        const string PrintActionsLog:: toString() const {}



//Close Class
        Close::Close(){}
        void Close::act(Simulation &simulation) {
            simulation.close();
            complete();
        }
        Close *Close::clone() const {}
        const string Close::toString() const {}



//BackupSimulation Class
        BackupSimulation::BackupSimulation(){}
        void BackupSimulation::act(Simulation &simulation) {}
        BackupSimulation *BackupSimulation::clone() const {}
        const string BackupSimulation::toString() const {}



//RestoreSimualtion Class
        RestoreSimulation::RestoreSimulation(){}
        void RestoreSimulation::act(Simulation &simulation) {}
        RestoreSimulation *RestoreSimulation::clone() const {}
        const string RestoreSimulation::toString() const {}
        
