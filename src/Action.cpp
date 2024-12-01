#include "Action.h"
#include <string>
#include <vector>
#include "Simulation.h"
#include <iostream>

class BaseAction{
    public:
        BaseAction(){}
        ActionStatus getStatus() const{}
        virtual void act(Simulation& simulation){}
        virtual const string toString() const{}
        virtual BaseAction* clone() const {}
        virtual ~BaseAction() { }

    protected:
        void BaseAction::complete() { status == ActionStatus::COMPLETED; }

        void BaseAction::error(string errorMsg) { this->errorMsg = errorMsg;
            status == ActionStatus::ERROR;
            std::cout << "Error:" + errorMsg << std::endl;
        }

        const string &getErrorMsg() const { return errorMsg; }

    private:
        string errorMsg;
        ActionStatus status;
};

class SimulateStep : public BaseAction {

    public:
        SimulateStep::SimulateStep(const int numOfSteps):numOfSteps(numOfSteps){}

        void    SimulateStep::act(Simulation &simulation) override{//does not return error
            for (int i = 0; i < numOfSteps;i++)
                simulation.step();
            simulation.addAction(this);
            complete();
        }

        const string toString() const override{}
        SimulateStep *clone() const override{}

    private:
        const int numOfSteps;
};

class AddPlan : public BaseAction {
    public:
        AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):settlementName(settlementName),selectionPolicy(selectionPolicy){}

        void AddPlan::act(Simulation &simulation) override{
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

        const string toString() const override{}
        AddPlan *clone() const override{}

    private:
        const string settlementName;
        const string selectionPolicy;
};


class AddSettlement : public BaseAction {
    public:
        AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType):settlementName(settlementName),settlementType(settlementType){}
        
        void AddSettlement::act(Simulation &simulation) override{
            try{
                simulation.addSettlement(new Settlement(settlementName,settlementType));
                simulation.addAction(this);
                complete();
            }
            catch(const std::exception &e){
                error(e.what());
            }
        }

        AddSettlement *clone() const override{}
        const string toString() const override{}
        
    private:
        const string settlementName;
        const SettlementType settlementType;
};



class AddFacility : public BaseAction {
    public:
        AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):facilityName(facilityName),facilityCategory(facilityCategory),price(price),lifeQualityScore(lifeQualityScore),economyScore(economyScore),environmentScore(environmentScore){}
        
        void AddFacility::act(Simulation &simulation) override{
            try{
                simulation.addFacility(FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore));
                simulation.addAction(this);
                complete();
            }
            catch(std::exception &e){
                error(e.what());
            }
        }
      
        AddFacility *clone() const override{}
        const string toString() const override{}

    private:
        const string facilityName;
        const FacilityCategory facilityCategory;
        const int price;
        const int lifeQualityScore;
        const int economyScore;
        const int environmentScore;

};

class PrintPlanStatus: public BaseAction {
    public:
        PrintPlanStatus::PrintPlanStatus(int planId):planId(planId){}

        void PrintPlanStatus::act(Simulation &simulation) override{
            if(!simulation.isPlanExists(planId))
                error("Plan doesn't exist");
            else{
                Plan plan = simulation.getPlan(planId);
                std::cout << plan.toString() << std::endl;
                simulation.addAction(this);
                complete();
            }
        }
        PrintPlanStatus *clone() const override{}
        const string toString() const override{}

    private:
        const int planId;
};


class ChangePlanPolicy : public BaseAction {
    public:
        ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy):planId(planId),newPolicy(newPolicy){}

        void ChangePlanPolicy::act(Simulation &simulation) override{
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

        ChangePlanPolicy *clone() const override{}
        const string toString() const override{}

    private:
        const int planId;
        const string newPolicy;
};


class PrintActionsLog : public BaseAction {
    public:
        PrintActionsLog::PrintActionsLog(){}
        void PrintActionsLog::act(Simulation &simulation) override{}

        PrintActionsLog *clone() const override{}
        const string toString() const override{}

    private:
};

class Close : public BaseAction {
    public:
        Close::Close(){}
        void Close::act(Simulation &simulation) override{

        }
        Close *clone() const override{}
        const string toString() const override{}

    private:
};

class BackupSimulation : public BaseAction {
    public:
        BackupSimulation::BackupSimulation(){}
        void BackupSimulation::act(Simulation &simulation) override{}
        BackupSimulation *clone() const override{}
        const string toString() const override{}

    private:
};


class RestoreSimulation : public BaseAction {
    public:
        RestoreSimulation::RestoreSimulation(){}
        void RestoreSimulation::act(Simulation &simulation) override{}
        RestoreSimulation *clone() const override{}
        const string toString() const override{}
        
    private:
};