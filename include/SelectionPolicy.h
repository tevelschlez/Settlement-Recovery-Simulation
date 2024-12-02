#pragma once
#include <vector>
#include <stdexcept>
#include "Facility.h"
using std::vector;
using std::string;

class SelectionPolicy {
    public:
        virtual const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) = 0;
        virtual const string toString() const = 0;
        virtual SelectionPolicy* clone() const = 0;
        virtual ~SelectionPolicy() = default;
};

class NaiveSelection: public SelectionPolicy {
    public:
        NaiveSelection();
        const FacilityType &selectFacility(const vector<FacilityType> &facilitiesOptions) override;
        const string toString() const override;
        NaiveSelection *clone() const override;
        ~NaiveSelection() override=default;
    private:
        int lastSelectedIndex;
};

class BalancedSelection: public SelectionPolicy {
    public:
        BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore);
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions)override ;
        const string toString() const override;
        BalancedSelection *clone() const override;
        ~BalancedSelection() override = default;

        //
        int minAmongScores(int sum_lifeQuality,int sum_economy, int sum_enviroment);
        int maxAmongScores(int sum_lifeQuality,int sum_economy, int sum_enviroment);
        //

    private:
        int LifeQualityScore;
        int EconomyScore;
        int EnvironmentScore;
        int minAmongScores(int sum_lifeQuality,int sum_economy, int sum_enviroment);
        int maxAmongScores(int sum_lifeQuality,int sum_economy, int sum_enviroment);
};

class EconomySelection: public SelectionPolicy {
    public:
        EconomySelection();
        const FacilityType &selectFacility(const vector<FacilityType> &facilitiesOptions) override;
        const string toString() const override;
        EconomySelection *clone() const override;
        ~EconomySelection() override = default;
    private:
        int lastSelectedIndex;

};

class SustainabilitySelection: public SelectionPolicy {
    public:
        SustainabilitySelection();
        const FacilityType &selectFacility(const vector<FacilityType> &facilitiesOptions) override;
        const string toString() const override;
        SustainabilitySelection *clone() const override;
        ~SustainabilitySelection() override=default;
    private:
        int lastSelectedIndex;
};