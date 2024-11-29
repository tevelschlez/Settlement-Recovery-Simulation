#include "SelectionPolicy.h"
#include <vector>
#include "Facility.h"
using std::vector;

class SelectionPolicy {
    public:

        virtual SelectionPolicy::~SelectionPolicy() {

        }
};

class NaiveSelection: public SelectionPolicy {
    public:
        NaiveSelection::NaiveSelection(){lastSelectedIndex=-1;}

        const FacilityType& NaiveSelection:: NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
            if(lastSelectedIndex==facilitiesOptions.size()-1)
                lastSelectedIndex=-1;

            return facilitiesOptions[lastSelectedIndex+1];
        }

        const string NaiveSelection:: toString() const {
            return "Naive Selection";
        }
        NaiveSelection * NaiveSelection:: clone() const {

        }
        NaiveSelection::~NaiveSelection()  {

        }
    private:
        int lastSelectedIndex;
};

class BalancedSelection: public SelectionPolicy {
    public:
        BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):LifeQualityScore(LifeQualityScore),EconomyScore(EconomyScore),EnvironmentScore(EnvironmentScore){}

        const FacilityType& BalancedSelection:: selectFacility(const vector<FacilityType>& facilitiesOptions) {
            int index=0;

            int sum_lifeQuality=LifeQualityScore+facilitiesOptions[0].getLifeQualityScore();
            int sum_ecomony=EconomyScore+facilitiesOptions[0].getEconomyScore();
            int sum_enviroment=EnvironmentScore+facilitiesOptions[0].getEnvironmentScore();

            int min_among_scores=minAmongScores(sum_lifeQuality,sum_ecomony,sum_enviroment);
            int max_among_scores=maxAmongScores(sum_lifeQuality,sum_ecomony,sum_enviroment);

            int distance=max_among_scores-min_among_scores;


            for(int i=1;i<facilitiesOptions.size();i++){
                sum_lifeQuality=LifeQualityScore-facilitiesOptions[i].getLifeQualityScore();
                sum_ecomony=EconomyScore-facilitiesOptions[i].getEconomyScore();
                sum_enviroment=EnvironmentScore-facilitiesOptions[i].getEnvironmentScore();

                min_among_scores=minAmongScores(sum_lifeQuality,sum_ecomony,sum_enviroment);
                max_among_scores=maxAmongScores(sum_lifeQuality,sum_ecomony,sum_enviroment);

                if(distance>(max_among_scores-min_among_scores)){
                    distance=max_among_scores-min_among_scores;
                    index=i;
                }
            }

            return facilitiesOptions[index];

        }

        int BalancedSelection::minAmongScores(int sum_lifeQuality,int sum_economy, int sum_enviroment){//returns the minimum of all sums of scores
            int min_among_scores;

            if(sum_lifeQuality<sum_economy&&sum_lifeQuality<sum_enviroment)
                min_among_scores=sum_lifeQuality;
            else if(sum_economy<sum_lifeQuality&&sum_economy<sum_enviroment)
                min_among_scores=sum_economy;
            else 
                min_among_scores=sum_enviroment;

            return min_among_scores;
        }

        int BalancedSelection::maxAmongScores(int sum_lifeQuality,int sum_economy, int sum_enviroment){//return the maximun of all sums of scores
            int max_among_scores;

            if(sum_lifeQuality>sum_economy&&sum_lifeQuality>sum_enviroment)
                max_among_scores=sum_lifeQuality;
            else if(sum_economy>sum_lifeQuality&&sum_economy>sum_enviroment)
                max_among_scores=sum_economy;
            else 
                max_among_scores=sum_enviroment;

            return max_among_scores;
        }


        const string BalancedSelection:: toString() const {
            return "Balanced Selection";
        }
        BalancedSelection * BalancedSelection:: clone() const {

        }
        BalancedSelection::~BalancedSelection()  {

        }
    private:
        int LifeQualityScore;
        int EconomyScore;
        int EnvironmentScore;
};

class EconomySelection: public SelectionPolicy {
    public:
        EconomySelection::EconomySelection(){lastSelectedIndex=-1;}

        const FacilityType& EconomySelection:: selectFacility(const vector<FacilityType>& facilitiesOptions) {
            if(lastSelectedIndex==facilitiesOptions.size()-1)
                lastSelectedIndex=-1;

            bool isFound=false;

            for(int i=lastSelectedIndex+1; !isFound ;i++){
                i=(i%facilitiesOptions.size());

                if(facilitiesOptions[i].getCategory()==FacilityCategory::ECONOMY){
                    lastSelectedIndex=i;
                    return facilitiesOptions[i];
                }
            }
        }

        const string EconomySelection:: toString() const {
            return "Economy Selection";
        }
        EconomySelection *EconomySelection::clone() const {

        }
        EconomySelection::~EconomySelection()  {

        }
    private:
        int lastSelectedIndex;

};

class SustainabilitySelection: public SelectionPolicy {
    public:
        SustainabilitySelection::SustainabilitySelection(){lastSelectedIndex=-1;}


        const FacilityType& SustainabilitySelection:: selectFacility(const vector<FacilityType>& facilitiesOptions) {
            if(lastSelectedIndex==facilitiesOptions.size()-1)
                lastSelectedIndex=-1;

            bool isFound=false;

            for(int i=lastSelectedIndex+1; !isFound ;i++){
                i=(i%facilitiesOptions.size());

                if(facilitiesOptions[i].getCategory()==FacilityCategory::ENVIRONMENT){
                    lastSelectedIndex=i;
                    return facilitiesOptions[i];
                }
            }
        }
        const string SustainabilitySelection:: toString() const {
            return "Sustainability Selection";
        }

        SustainabilitySelection *SustainabilitySelection::clone() const {

        }
        SustainabilitySelection::~SustainabilitySelection()  {

        }
    private:
        int lastSelectedIndex;
};