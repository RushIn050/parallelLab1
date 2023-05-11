#pragma once
#include "station.h"
#include "train.h"
#include "train.cpp"


station::station(int& ws, std::vector<int>& arrival, int& max_l) {
	std::cout << "station is created!\n";
	count_of_ways = ws;
	max_length_of_train = max_l;
	arrival_of_trains = arrival;
	for (int i = 0; i <= count_of_ways; ++i) {
		ways_is_free.push_back(true);
		ways_priority.push_back(true);
	}
	for (int i = 1; i < count_of_ways; ++i) {
		std::string key = "left " + std::to_string(i) + "-" + std::to_string(i + 1); // 
		transits_is_free[key] = true;
		std::string key_of_map = "left " + std::to_string(i + 1) + "-" + std::to_string(i);
		transits_is_free[key_of_map] = true;
		key = "right " + std::to_string(i) + "-" + std::to_string(i + 1); // 
		transits_is_free[key] = true;
		key_of_map = "right " + std::to_string(i + 1) + "-" + std::to_string(i);
		transits_is_free[key_of_map] = true;
	}
	station::station_work();
}

station::~station() {}


void station::station_work() {
	std::string train_name = "train-";
	std::vector<std::thread> vecThread(arrival_of_trains.size());
	std::cout << "station is work!\n";
	for (int i = 1; i < arrival_of_trains.size(); ++i) {
		std::this_thread::sleep_for(std::chrono::seconds(arrival_of_trains[i]));// ожидание, когда прибудет следующий поезд 
		train_name = "train " + std::to_string(i);
		std::cout << "ready for " << train_name << std::endl;
		std::shared_ptr<train> tr(new train(max_length_of_train, count_of_ways, train_name));
		vecThread.at(i) = std::thread(&station::train_into_station, this, tr);
	}
	for (int i = 1; i < vecThread.size(); ++i)
		vecThread.at(i).join();
}

void station::train_into_station(std::shared_ptr<train> train) {
	int way = train->get_start_way();
	std::cout << "is ready " << ways_is_free[way] << " on " << way << " " << train->get_side() << " to " << train->get_end_way() << std::endl;
	while(!ways_is_free[way] && !ways_priority[way])
		std::this_thread::sleep_for(std::chrono::milliseconds(300));

	{
		std::lock_guard<std::mutex> lock(Mtx);
		ways_is_free[way] = false; 
		ways_is_free[train->get_end_way()] = false;
		std::cout << train->get_name() << " in station on way" << way << std::endl;
	}

	if (way < train->get_end_way()) { 
		if (train->get_side() == "to right") {
			std::this_thread::sleep_for(std::chrono::milliseconds(way * 50 * 1000 / train->get_speed()));
			for (int i = 0; i < abs(train->get_start_way() - train->get_end_way()); ++i) {
				while (!transits_is_free["left " + std::to_string(way) + "-" + std::to_string(way + 1)] && !ways_is_free[way])
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				{
					std::lock_guard<std::mutex> lock(Mtx);
					transits_is_free["left " + std::to_string(way) + "-" + std::to_string(way + 1)] = false;
					ways_is_free[way] = false;
					std::cout << train->get_name() << " in transit: left " << std::to_string(way) + "-" + std::to_string(way + 1) << std::endl;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(train->get_length() * 1000 / train->get_speed()));
				ways_is_free[way] = true;
				
				if (i>0)
					transits_is_free["left " + std::to_string(way-1) + "-" + std::to_string(way)] = true;
				++way;
				std::this_thread::sleep_for(std::chrono::milliseconds((lenght_of_transit - train->get_length()) * 1000 / train->get_speed()));
			}
			while (!ways_is_free[way])
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			{
				std::lock_guard<std::mutex> lock(Mtx);
				ways_is_free[way] = false;
				std::cout << train->get_name() << " in end way " << way << std::endl;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(train->get_length() * 1000 / train->get_speed()));
			transits_is_free["left " + std::to_string(way - 1) + "-" + std::to_string(way)] = true;
			std::this_thread::sleep_for(std::chrono::milliseconds((1000 - train->get_length() - way*50) * 1000 / train->get_speed()));
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(way * 50 * 1000 / train->get_speed()));
			for (int i = 0; i < abs(train->get_start_way() - train->get_end_way()); ++i) {
				while (!transits_is_free["right " + std::to_string(way + 1) + "-" + std::to_string(way)] && !ways_is_free[way]) {
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				}
					
				{
					std::lock_guard<std::mutex> lock(Mtx);
					transits_is_free["right " + std::to_string(way + 1) + "-" + std::to_string(way)] = false;
					ways_is_free[way] = false;
					std::cout << train->get_name() << " in transit: right " << std::to_string(way + 1) + "-" + std::to_string(way) << std::endl;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(train->get_length() * 1000 / train->get_speed()));
				ways_is_free[way] = true;
				if (i > 0)
					transits_is_free["right " + std::to_string(way) + "-" + std::to_string(way-1)] = true;
				++way;
				std::this_thread::sleep_for(std::chrono::milliseconds((lenght_of_transit - train->get_length()) * 1000 / train->get_speed()));
			}
			while (!ways_is_free[way])
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			{
				std::lock_guard<std::mutex> lock(Mtx);
				ways_is_free[way] = false;
				std::cout << train->get_name() << " in end way " << way << std::endl;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(train->get_length() * 1000 / train->get_speed()));
			transits_is_free["right " + std::to_string(way) + "-" + std::to_string(way - 1)] = true;
			std::this_thread::sleep_for(std::chrono::milliseconds((1000 - train->get_length() - way * 50) * 1000 / train->get_speed()));
		}
	}
	else if (way > train->get_end_way()) {
		if (train->get_side() == "to right") {
			
			for (int i = 0; i < abs(train->get_start_way() - train->get_end_way()); ++i) {
				while (!transits_is_free["left " + std::to_string(way) + "-" + std::to_string(way - 1)] && !ways_is_free[way])
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				{
					std::lock_guard<std::mutex> lock(Mtx);
					transits_is_free["left " + std::to_string(way) + "-" + std::to_string(way - 1)] = false;
					ways_is_free[way] = false;
					std::cout << train->get_name() << " in transit: left " << std::to_string(way) + "-" + std::to_string(way - 1) << std::endl;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(train->get_length() * 1000 / train->get_speed()));
				ways_is_free[way] = true;
				if (i > 0)
					transits_is_free["left " + std::to_string(way+1) + "-" + std::to_string(way)] = true;
				--way;
				std::this_thread::sleep_for(std::chrono::milliseconds((lenght_of_transit - train->get_length()) * 1000 / train->get_speed()));
			}
			while (!ways_is_free[way])
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			{
				std::lock_guard<std::mutex> lock(Mtx);
				ways_is_free[way] = false;
				std::cout << train->get_name() << " in end way " << way << std::endl;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(train->get_length() * 1000 / train->get_speed()));
			transits_is_free["left " + std::to_string(way + 1) + "-" + std::to_string(way)] = true;
			std::this_thread::sleep_for(std::chrono::milliseconds((1000 - train->get_length() - way * 50) * 1000 / train->get_speed()));
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds((1000 - way * 50) * 1000 / train->get_speed()));
			for (int i = 0; i < abs(train->get_start_way() - train->get_end_way()); ++i) {
				while (!transits_is_free["right " + std::to_string(way - 1) + "-" + std::to_string(way)] && !ways_is_free[way])
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				{
					std::lock_guard<std::mutex> lock(Mtx);
					transits_is_free["right " + std::to_string(way - 1) + "-" + std::to_string(way)] = false;
					ways_is_free[way] = false;
					std::cout << train->get_name() << " in transit: right " << std::to_string(way - 1) + "-" + std::to_string(way) << std::endl;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(train->get_length() * 1000 / train->get_speed()));
				ways_is_free[way] = true;
				if (i > 0)
					transits_is_free["right " + std::to_string(way) + "-" + std::to_string(way+1)] = true;
				--way;
				std::this_thread::sleep_for(std::chrono::milliseconds((lenght_of_transit - train->get_length()) * 1000 / train->get_speed()));
			}
			while (!ways_is_free[way])
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			{
				std::lock_guard<std::mutex> lock(Mtx);
				ways_is_free[way] = false;
				std::cout << train->get_name() << " in end way " << way << std::endl;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(train->get_length() * 1000 / train->get_speed()));
			transits_is_free["right " + std::to_string(way) + "-" + std::to_string(way + 1)] = true;
			std::this_thread::sleep_for(std::chrono::milliseconds((1000 - train->get_length() - way * 50) * 1000 / train->get_speed()));
		}
	}
	else
		std::this_thread::sleep_for(std::chrono::milliseconds(lenght_of_way*1000/train->get_speed()));
	
		
	{
		std::lock_guard<std::mutex> lock(Mtx);
		ways_is_free[way] = true;
		ways_is_free[train->get_end_way()] = true;
		std::cout << train->get_name() << " left the station \n";
	}

}