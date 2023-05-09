#include "Train.h"


train::train(int max_len, int ways_count, std::string name) {
	length = rand() % (max_len-10) + 10;
	speed = rand() % 50+50;
	start_way = rand() % (ways_count) + 1;
	end_way = rand() % (ways_count) + 1;
	train_name = name;
	if ((rand() % 2) == 0)
		side = "to left";
	else
		side = "to right";
}

train::~train(){}

int train::get_length() {
	return length;
}

int train::get_speed() {
	return speed;
}

int train::get_start_way() {
	return start_way;
}

int train::get_end_way() {
	return end_way;
}

std::string train::get_name() {
	return train_name;
}

std::string train::get_side() {
	return side;
}

void train::set_current_way(int way) {
	current_way = way;
}