#include "SelectionPolicy.h"
#include <vector>
#include "Facility.h"
using std::vector;
#include <algorithm>

//NaiveSelection Class

        NaiveSelection::NaiveSelection():lastSelectedIndex(-1){}

        const FacilityType& NaiveSelection:: NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
            lastSelectedIndex = static_cast<unsigned int>(lastSelectedIndex + 1) % facilitiesOptions.size();
            return facilitiesOptions[lastSelectedIndex];
        }

        const string NaiveSelection:: toString() const {return "nve";}

        NaiveSelection *NaiveSelection::clone() const { return new NaiveSelection(*this); }


//BalancedSelection Class

            BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore) : LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore) {}

            const FacilityType &BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
            {
                int index = 0;
                int counter = 0;

                int sum_lifeQuality = LifeQualityScore + facilitiesOptions[0].getLifeQualityScore();
                int sum_ecomony = EconomyScore + facilitiesOptions[0].getEconomyScore();
                int sum_enviroment = EnvironmentScore + facilitiesOptions[0].getEnvironmentScore();

                int min_among_scores = minAmongScores(sum_lifeQuality, sum_ecomony, sum_enviroment);
                int max_among_scores = maxAmongScores(sum_lifeQuality, sum_ecomony, sum_enviroment);

                int distance = max_among_scores - min_among_scores;

                for (auto facility:facilitiesOptions)
                {

                    sum_lifeQuality = LifeQualityScore + facility.getLifeQualityScore();
                    sum_ecomony = EconomyScore + facility.getEconomyScore();
                    sum_enviroment = EnvironmentScore + facility.getEnvironmentScore();

                    min_among_scores = minAmongScores(sum_lifeQuality, sum_ecomony, sum_enviroment);
                    max_among_scores = maxAmongScores(sum_lifeQuality, sum_ecomony, sum_enviroment);

                    if (distance > (max_among_scores - min_among_scores))
                    {
                        distance = max_among_scores - min_among_scores;
                        index = counter;
                    }
                    counter++;
                }

                return facilitiesOptions[index];
            }

            int BalancedSelection::minAmongScores(int sum_lifeQuality, int sum_economy, int sum_enviroment)
            { // returns the minimum of all sums of scores
                int min_among_scores;

                min_among_scores = std::min(sum_lifeQuality, sum_enviroment);
                min_among_scores = std::min(min_among_scores, sum_economy);

                return min_among_scores;
            }

            int BalancedSelection::maxAmongScores(int sum_lifeQuality, int sum_economy, int sum_enviroment)
            { // return the maximun of all sums of scores
                int max_among_scores;

                max_among_scores = std::max(sum_lifeQuality, sum_economy);
                max_among_scores = std::max(max_among_scores, sum_enviroment);

                return max_among_scores;
            }

            const string BalancedSelection::toString() const{return "bal";}
            BalancedSelection *BalancedSelection::clone() const { return new BalancedSelection(*this); }


//EconomySelection Class


                EconomySelection::EconomySelection():lastSelectedIndex(-1){ }

                const FacilityType &EconomySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
                {
                    lastSelectedIndex = static_cast<unsigned int>(lastSelectedIndex + 1) % facilitiesOptions.size();

                    while(facilitiesOptions[lastSelectedIndex].getCategory()!=FacilityCategory::ECONOMY)
                        lastSelectedIndex = static_cast<unsigned int>(lastSelectedIndex + 1) % facilitiesOptions.size();

                    return facilitiesOptions[lastSelectedIndex];
                }

                const string EconomySelection::toString() const{return "eco";}
                EconomySelection *EconomySelection::clone() const { return new EconomySelection(*this); }


//SustainabilitySelection Class


                    SustainabilitySelection::SustainabilitySelection():lastSelectedIndex(-1) {}

                    const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
                    {
                        lastSelectedIndex = static_cast<unsigned int>(lastSelectedIndex + 1) % facilitiesOptions.size();

                        while (facilitiesOptions[lastSelectedIndex].getCategory() != FacilityCategory::ENVIRONMENT)
                            lastSelectedIndex = static_cast<unsigned int>(lastSelectedIndex + 1) % facilitiesOptions.size();

                        return facilitiesOptions[lastSelectedIndex];
                    }
                    const string SustainabilitySelection::toString() const{ return "env";}

                    SustainabilitySelection *SustainabilitySelection::clone() const { return new SustainabilitySelection(*this); }

