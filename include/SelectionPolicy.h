#pragma once
#include <vector>
#include "Facility.h"
using std::vector;

class SelectionPolicy {
    public:
        virtual const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) = 0;
        virtual const string toString() const = 0;
        virtual SelectionPolicy* clone() const = 0;
        virtual ~SelectionPolicy() = default;
};

class NaiveSelection: public SelectionPolicy {
    public:
        NaiveSelection(){}
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) ;
        const string toString() const ;
        NaiveSelection *clone() const ;
        ~NaiveSelection();
    private:
        int lastSelectedIndex;
};

class BalancedSelection: public SelectionPolicy {
    public:
        BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore);
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) ;
        const string toString() const ;
        BalancedSelection *clone() const ;
        ~BalancedSelection() ;
        int minAmongScores(int sum_lifeQuality,int sum_economy, int sum_enviroment);
        int maxAmongScores(int sum_lifeQuality,int sum_economy, int sum_enviroment);
    private:
        int LifeQualityScore;
        int EconomyScore;
        int EnvironmentScore;
};

class EconomySelection: public SelectionPolicy {
    public:
        EconomySelection(){}
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) ;
        const string toString() const ;
        EconomySelection *clone() const ;
        ~EconomySelection();
    private:
        int lastSelectedIndex;

};

class SustainabilitySelection: public SelectionPolicy {
    public:
        SustainabilitySelection(){}
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) ;
        const string toString() const ;
        SustainabilitySelection *clone() const ;
        ~SustainabilitySelection() ;
    private:
        int lastSelectedIndex;
};