#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

int point[] = {1304, 2312, 3639, 1315, 4177, 2244, 3712, 1399, 3488, 1535, 3326, 1556, 3238, 1229, 4196, 1004, 4312, 790, 4386, 570, 3007, 1970, 2562, 1756, 2788, 1491, 2381, 1676, 1332, 695, 3715, 1678, 3918, 2179, 4061, 2370, 3780, 2212, 3676, 2578, 4029, 2838, 4263, 2931, 3429, 1908, 3507, 2367, 3394, 2643, 3439, 3201, 2935, 3240, 3140, 3550, 2545, 2357, 2778, 2826, 2370, 2975}; // 城市的坐标

const int numOfCities = sizeof(point) / sizeof(int) / 2;

class TSP
{
    /*This Class defines the Basic functions for the TSP Problem, like taking the Input and calculating the distance matrix for the cities*/
public:
    const int number_of_population = 10000;

    vector<pair<int, int>> points;
    double Graph[numOfCities][numOfCities];

    vector<vector<int>> cities;
    vector<double> cities_distance;

    vector<vector<int>> temp_cities;

    vector<int> final_cities;
    double final_distance;

    unsigned seed;
    default_random_engine generator;

    int number_of_population_index;

    TSP()
    {
        cities.resize(number_of_population);
        cities_distance.resize(number_of_population);
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
        seed = chrono::system_clock::now().time_since_epoch().count();
        generator.seed(seed);
    }

    double CalculateDistance(int p1, int p2)
    {
        return sqrt(pow(points[p1].first - points[p2].first, 2) + pow(points[p1].second - points[p2].second, 2));
    }

    double Distance(int city1, int city2)
    {
        return Graph[city1][city2];
    }

    void InitialPoint() //puts the nearestNeighbourTour in the vector cities
    {
        vector<int> temp;
        for (int i = 1; i < numOfCities; i++)
        {
            temp.push_back(i);
        }
        cities[0].push_back(0);

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
            cities[0].push_back(pos);
            temp.erase(find(temp.begin(), temp.end(), pos));
        }

        for (int i = 1; i < number_of_population; i++)
        {
            cities[i] = cities[i - 1];
            random_shuffle(cities[i].begin(), cities[i].end());
        }

        return;
    }

    double GetTourLength(vector<int> city) //This function returns the tour length of the current order of cities
    {
        vector<int>::iterator pcity = --city.end();
        double tourLength = 0;
        for (auto ncity = city.begin(); ncity != city.end(); ncity++)
        {
            tourLength += Distance(*pcity, *ncity);
            pcity = ncity;
        }
        return tourLength;
    }

    double getRandomNumber(double i, double j) //This function generates a random number between i and j
    {
        uniform_real_distribution<double> distribution(i, j);
        return double(distribution(generator));
    }

    void Print()
    {
        for (auto &item : cities)
        {
            for (auto it = item.begin(); it != item.end(); it++)
            {
                cout << *it << ", ";
            }
            cout << endl;
        }
    }

    void Select()
    {
        seed = chrono::system_clock::now().time_since_epoch().count();
        generator.seed(seed);
        vector<double> percentage;
        double total = 0;
        for (auto &item : cities_distance)
        {
            total += 100000.0 / item;
            percentage.push_back(total);
        }

        vector<double> fixed_point(number_of_population / 2);
        int max_value = percentage.back();
        for (auto &item : fixed_point)
        {
            item = getRandomNumber(0, max_value);
        }

        sort(fixed_point.begin(), fixed_point.end());

        temp_cities = cities;
        int index = 0;
        number_of_population_index = 0;
        for (auto &item : fixed_point)
        {
            while (item > percentage[index])
                index++;
            cities[number_of_population_index++] = temp_cities[index];
        }

        random_shuffle(cities.begin(), cities.end());
    }

    void Crossover(vector<int> city1, vector<int> city2, vector<int> &cross_city1, vector<int> &cross_city2)
    {
        vector<int> inv1(city1.size(), 0);
        vector<int> inv2(city1.size(), 0);
        //encode
        for (int i = 0; i < city1.size(); i++)
        {
            int m = 0;
            while (city1[m] != i)
            {
                if (city1[m] > i)
                {
                    inv1[i]++;
                }
                m++;
            }
        }
        //encode
        for (int i = 0; i < city2.size(); i++)
        {
            int m = 0;
            while (city2[m] != i)
            {
                if (city2[m] > i)
                {
                    inv2[i]++;
                }
                m++;
            }
        }

        int position1 = getRandomNumber(0, numOfCities - 1);
        int position2 = getRandomNumber(0, numOfCities - 1);
        if (position1 > position2)
            swap(position1, position2);

        for (int i = position1; i <= position2; i++)
        {
            swap(inv1[i], inv2[i]);
        }

        vector<int> pos1(city1.size(), 0);
        vector<int> pos2(city1.size(), 0);

        //decode
        for (int i = inv1.size() - 1; i >= 0; i--)
        {
            for (int m = i + 1; m < pos1.size(); m++)
            {
                if (pos1[m] >= inv1[i])
                    pos1[m]++;
            }
            pos1[i] = inv1[i];
        }
        for (int i = 0; i < city1.size(); i++)
            cross_city1[pos1[i]] = i;

        //decode
        for (int i = inv2.size() - 1; i >= 0; i--)
        {
            for (int m = i + 1; m < pos2.size(); m++)
            {
                if (pos2[m] >= inv2[i])
                    pos2[m]++;
            }
            pos2[i] = inv2[i];
        }
        for (int i = 0; i < city2.size(); i++)
            cross_city2[pos2[i]] = i;
    }

    void Cross_all()
    {
        vector<int> temp1(cities.back().size());
        vector<int> temp2(cities.back().size());
        int cities_size = number_of_population / 2;
        for (int i = 0; i < cities_size; i += 2)
        {
            Crossover(cities[i], cities[i + 1], temp1, temp2);
            cities[number_of_population_index++] = temp1;
            cities[number_of_population_index++] = temp2;
        }
    }

    void Variation(double probility)
    {
        for (auto &item : cities)
        {
            if (getRandomNumber(0, 1) < probility)
            {
                int position1 = getRandomNumber(0, numOfCities - 1);
                int position2 = getRandomNumber(0, numOfCities - 1);
                if (position1 > position2)
                    swap(position1, position2);
                swap(item[position1], item[position2]);
                random_shuffle(&item[position1], &item[position2]);
            }
        }
    }

    void UpdateTourLength()
    {
        for (int i = 0; i < cities.size(); i++)
        {
            cities_distance[i] = GetTourLength(cities[i]);
        }
    }

    int similarity()
    {
        map<int, int> word_count;
        for (auto &item : cities_distance)
            word_count[item]++;
        int max = 0;
        for (auto &item : word_count)
        {
            if (max < item.second)
                max = item.second;
        }
        return max;
    }
};

int main(int argc, char *argv[])
{

    //ofstream out_distance;
    //out_distance.open("distance", ios::out | ios::trunc);
    //ofstream out_similarity;
    //out_similarity.open("similarity", ios::out | ios::trunc);
    vector<int> min_cities;
    double min = 100000;

#pragma omp parallel
    {
        TSP tsp;
        //Generate the initial city tour and get the  nearestNeighbourTourLength
        //start with a point
        tsp.InitialPoint();
        //tsp.Print();
        tsp.UpdateTourLength();
        int index = min_element(tsp.cities_distance.begin(), tsp.cities_distance.end()) - tsp.cities_distance.begin();
        tsp.final_cities = tsp.cities[index];
        tsp.final_distance = tsp.cities_distance[index];
        if (min > tsp.final_distance)
        {
            min_cities = tsp.final_cities;
            min = tsp.final_distance;
        }
        double similarity_precent = 0.8;
        // double cross_precent = 0.9;

        while (tsp.similarity() < tsp.number_of_population * similarity_precent)
        {
            tsp.Select();
            // if (tsp.getRandomNumber(0, 1) < 0.99) //交叉概率
            tsp.Cross_all();
            tsp.UpdateTourLength();
            double variation_probility = tsp.similarity() / tsp.number_of_population * 0.4;
            tsp.Variation(variation_probility);
            tsp.UpdateTourLength();
            index = min_element(tsp.cities_distance.begin(), tsp.cities_distance.end()) - tsp.cities_distance.begin();
            if (tsp.cities_distance[index] < tsp.final_distance)
            {
                tsp.final_cities = tsp.cities[index];
                tsp.final_distance = tsp.cities_distance[index];
                if (min > tsp.final_distance)
                {
                    min_cities = tsp.final_cities;
                    min = tsp.final_distance;
                }
            }

//if (omp_get_thread_num() == 0)
#pragma omp single nowait
            {
                cout << " the minimum path length is  " << min << endl;
                cout << " the minimum path is  ";
                for (auto it = min_cities.begin(); it != min_cities.end(); it++)
                {
                    cout << *it << ", ";
                }
                cout << endl;
                cout << " similarity " << tsp.similarity() << endl;
            }
        }
    }

    cout << " the minimum path length is  " << min << endl;
    //out_distance << min << endl;
    cout << " the minimum path is  ";
    for (auto it = min_cities.begin(); it != min_cities.end(); it++)
    {
        cout << *it << ", ";
    }
    cout << endl;
    //out_distance.close();
    //out_similarity.close();
    return 0;
}
