#pragma once
#include <string>
#include <cstdlib>

class train {

	int length;
	int speed;
	int start_way;
	int end_way; 
	int current_way;
	std::string side;
	std::string train_name;

public:

	train(int max_len, int ways_count, std::string name);
	~train();
	int get_length();
	int get_speed();
	int get_start_way();
	int get_end_way();
	std::string get_name();
	std::string get_side();
	void set_current_way(int way);
};