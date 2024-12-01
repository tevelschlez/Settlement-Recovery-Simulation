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
        
        const int getID();
        //

        //rule of 5:
        ~Plan();//destructors
        Plan(Plan &other);//copy constructor
        Plan(Plan &&other) noexcept;//move copy constructor

    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        PlanStatus status;
        int life_quality_score, economy_score, environment_score;
        const int constructionLimit;
        const int getConstructionLimit();

};