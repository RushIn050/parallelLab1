
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include "station.h"
#include "station.cpp"

void station_start(int &ways, std::vector<int> &arrival_schedule, int &max_l_of_train) {
    std::sort(arrival_schedule.begin(), arrival_schedule.end());
    std::shared_ptr<station> st(new station(ways, arrival_schedule, max_l_of_train));
}

int main()
{
    int ways = 5, max_l_of_train = 100, number_of_trains = 100;
    std::vector<int> arrival_schedule(number_of_trains+1, 0);
    for (int i = 1; i < number_of_trains; ++i)
        arrival_schedule[i] = rand() % 10 + 2; //время до прибытия следующего поезда на станцию
    
    station_start(ways, arrival_schedule, max_l_of_train);
}
