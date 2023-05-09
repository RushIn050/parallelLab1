#pragma once
#include <vector>
#include <ctime>
#include <thread>
#include <string>
#include <memory>
#include <chrono>
#include <mutex>
#include <map>
#include "train.h"

class  station {

	int count_of_ways;
	int max_length_of_train;
	int lenght_of_way = 1000;
	int lenght_of_transit = 110;
	std::vector<int> arrival_of_trains; 
	std::vector<bool> ways_is_free;
	std::map<std::string, bool> transits_is_free;
	std::mutex Mtx;

public:

	station(int& ws, std::vector<int>& arrival, int& max_l);
	~station();
	void station_work();
	void train_into_station(std::shared_ptr<train> train);
};