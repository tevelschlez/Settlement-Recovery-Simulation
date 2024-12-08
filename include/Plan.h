#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;

        //
        bool comparePolicy(const string &policyToCompare);
        //

        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;

        //
        void activateSelectionPolicy();
        
        int getID() const;

        const Settlement &getSettlement() const;

        const vector<Facility*> &getUnderConstruction() const;

        SelectionPolicy* getSelectionPolicy() const;
        //
        Plan(const Plan& other, const Settlement& other_settlement);
        ~Plan();
        Plan(const Plan& other);
        Plan(Plan&& other);
        Plan& operator=(const Plan& other) = delete;
        Plan& operator=(const Plan&& other) = delete;

    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
        //
        const unsigned int constructionLimit;
        const int getConstructionLimit();
        //

};