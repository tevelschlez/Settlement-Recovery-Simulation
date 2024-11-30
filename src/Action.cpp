#include "Action.h"
#include <string>
#include <vector>
#include "Simulation.h"

class BaseAction{
    public:
        BaseAction(){}
        ActionStatus getStatus() const{}
        virtual void act(Simulation& simulation){}
        virtual const string toString() const{}
        virtual BaseAction* clone() const {}
        virtual ~BaseAction() { }

    protected:
        void complete();
        void error(string errorMsg);
        const string &getErrorMsg() const;

    private:
        string errorMsg;
        ActionStatus status;
};

class SimulateStep : public BaseAction {

    public:
        SimulateStep::SimulateStep(const int numOfSteps):numOfSteps(numOfSteps){

        }
        void act(Simulation &simulation) override{

        }
        const string toString() const override{

        }
        SimulateStep *clone() const override{

        }
    private:
        const int numOfSteps;
};

class AddPlan : public BaseAction {
    public:
        AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):settlementName(settlementName),selectionPolicy(selectionPolicy){

        }
        void act(Simulation &simulation) override{

        }
        const string toString() const override{

        }
        AddPlan *clone() const override{

        }
    private:
        const string settlementName;
        const string selectionPolicy;
};


class AddSettlement : public BaseAction {
    public:
        AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType):settlementName(settlementName),settlementType(settlementType){

        }
        void act(Simulation &simulation) override{

        }
        AddSettlement *clone() const override{

        }
        const string toString() const override{

        }
        
    private:
        const string settlementName;
        const SettlementType settlementType;
};



class AddFacility : public BaseAction {
    public:
        AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):facilityName(facilityName),facilityCategory(facilityCategory),price(price),lifeQualityScore(lifeQualityScore),economyScore(economyScore),environmentScore(environmentScore){

        }
        void act(Simulation &simulation) override{

        }
        AddFacility *clone() const override{

        }
        const string toString() const override{

        }
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
        PrintPlanStatus::PrintPlanStatus(int planId):planId(planId){

        }
        void act(Simulation &simulation) override{

        }
        PrintPlanStatus *clone() const override{

        }
        const string toString() const override{

        }
    private:
        const int planId;
};


class ChangePlanPolicy : public BaseAction {
    public:
        ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy):planId(planId),newPolicy(newPolicy){

        }
        void act(Simulation &simulation) override{

        }
        ChangePlanPolicy *clone() const override{

        }
        const string toString() const override{

        }
    private:
        const int planId;
        const string newPolicy;
};


class PrintActionsLog : public BaseAction {
    public:
        PrintActionsLog::PrintActionsLog(){}
        void act(Simulation &simulation) override{

        }
        PrintActionsLog *clone() const override{

        }
        const string toString() const override{

        }
    private:
};

class Close : public BaseAction {
    public:
        Close::Close(){

        }
        void act(Simulation &simulation) override{

        }
        Close *clone() const override{

        }
        const string toString() const override{

        }
    private:
};

class BackupSimulation : public BaseAction {
    public:
        BackupSimulation::BackupSimulation(){}
        void act(Simulation &simulation) override{

        }
        BackupSimulation *clone() const override{

        }
        const string toString() const override{

        }
    private:
};


class RestoreSimulation : public BaseAction {
    public:
        RestoreSimulation::RestoreSimulation(){}
        void act(Simulation &simulation) override{

        }
        RestoreSimulation *clone() const override{

        }
        const string toString() const override{
            
        }
    private:
};