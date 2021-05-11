#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

int point[] = {1304, 2312, 3639, 1315, 4177, 2244, 3712, 1399, 3488, 1535, 3326, 1556, 3238, 1229, 4196, 1004, 4312, 790, 4386, 570, 3007, 1970, 2562, 1756, 2788, 1491, 2381, 1676, 1332, 695, 3715, 1678, 3918, 2179, 4061, 2370, 3780, 2212, 3676, 2578, 4029, 2838, 4263, 2931, 3429, 1908, 3507, 2367, 3394, 2643, 3439, 3201, 2935, 3240, 3140, 3550, 2545, 2357, 2778, 2826, 2370, 2975}; // 城市的坐标

const int numOfCities = sizeof(point) / sizeof(int) / 2;

class TSP
{
    /*This Class defines the Basic functions for the TSP Problem, like taking the Input and calculating the distance matrix for the cities*/
public:
    vector<pair<int, int>> points;
    double Graph[numOfCities][numOfCities];
    vector<int> finalOrder;

    vector<int> cities;

    TSP()
    {
        for (int i = 0; i < numOfCities; i++)
        {
            points.push_back(pair<int, int>(point[2 * i], point[2 * i + 1]));
        }

        for (int i = 0; i < numOfCities; i++)
        {
            Graph[i][i] = 0;
            for (int j = i + 1; j < numOfCities; j++)
            {
                Graph[i][j] = CalculateDistance(i, j);
                Graph[j][i] = Graph[i][j];
                // This is a Symmetric TSP
            }
        }
    }

    double CalculateDistance(int p1, int p2)
    {
        return sqrt(pow(points[p1].first - points[p2].first, 2) + pow(points[p1].second - points[p2].second, 2));
    }

    double Distance(int city1, int city2)
    {
        return Graph[city1][city2];
    }

    double InitialPoint() //puts the nearestNeighbourTour in the vector cities
    {
        vector<int> temp;
        for (int i = 1; i < numOfCities; i++)
        {
            temp.push_back(i);
        }
        cities.push_back(0);

        int pos = 0, i = 0;

        while (temp.size())
        {
            int min = INT_MAX;
            for (auto n : temp)
            {
                if (min > Distance(pos, n))
                {
                    min = Distance(pos, n);
                    i = n;
                }
            }
            pos = i;
            cities.push_back(pos);
            temp.erase(find(temp.begin(), temp.end(), pos));
        }

        finalOrder = cities;

        return GetTourLength();
    }

    double GetTourLength() //This function returns the tour length of the current order of cities
    {
        vector<int>::iterator pcity = --cities.end();
        double tourLength = 0;
        for (auto ncity = cities.begin(); ncity != cities.end(); ncity++)
        {
            tourLength += Distance(*pcity, *ncity);
            pcity = ncity;
        }

        return tourLength;
    }
};

double getRandomNumber(double i, double j) //This function generates a random number between
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_real_distribution<double> distribution(i, j);
    return double(distribution(generator));
}

double getProbability(int difference, double temperature) //This function finds the probability of how bad the new solution is
{
    return exp(-1 * difference / temperature);
}

int main(int argc, char *argv[])
{
    vector<int> min_cities;
    double min;
#pragma omp parallel
    {
        vector<int>::iterator it;
        TSP tsp;
        //Generate the initial city tour and get the  nearestNeighbourTourLength
        double bestTourLength = tsp.InitialPoint(); //start with a point
#pragma omp single
        cout << "startLength = " << bestTourLength << endl;

        min = bestTourLength;

        min_cities = tsp.cities;
        double temperature, coolingRate = 0.99, absoluteTemperature = 0.00001, probability;
        int position1 = 0, position2 = 0;
        double newTourLength, difference;
        for (int rs = 0; rs < 10; rs++) // 算法执行的次数
        {
            temperature = 1e+30; //Initial Temperature

            while (temperature > absoluteTemperature)
            {
                int continuous_time = 1000;
                while (continuous_time != 0)
                {
                    continuous_time--;
                    position1 = int(getRandomNumber(0, numOfCities - 1));
                    position2 = int(getRandomNumber(0, numOfCities - 1));
                    while (position1 == position2)
                    {
                        position1 = int(getRandomNumber(0, numOfCities - 1));
                        position2 = int(getRandomNumber(0, numOfCities - 1));
                    }
                    it = tsp.cities.begin();
                    swap(*(it + position1), *(it + position2));
                    // if (position2 > position1)
                    //     random_shuffle(it + position1, it + position2);
                    newTourLength = tsp.GetTourLength();
                    if (min > newTourLength)
                    {
                        min = newTourLength;
                        min_cities = tsp.cities;
                    }

                    difference = newTourLength - bestTourLength;

                    if (difference < 0 or (difference > 0 and getProbability(difference, temperature) > getRandomNumber(0, 1)))
                    {
                        tsp.finalOrder = tsp.cities;
                        bestTourLength = newTourLength;
                    }
                    else
                    {
                        tsp.cities = tsp.finalOrder;
                    }
                }
                tsp.cities = min_cities;
                temperature = temperature * coolingRate;
            }
            random_shuffle(tsp.cities.begin(), tsp.cities.end());
        }
    }
    cout << " the minimum path length is  " << min << endl;
    cout << " the minimum path is  ";
    for (auto it = min_cities.begin(); it != min_cities.end(); it++)
    {
        cout << *it << ", ";
    }
    cout << endl;
    return 0;
}
