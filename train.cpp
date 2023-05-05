#include "Train.h"
#include <cstdlib>

train(int max_len, int ways_count) {
	length = rand() % (max_len-10) + 10;
	speed = rand() % 70+30;
	start_way = rand() % (ways_count-1) + 1;
	end_way = rand() % (ways_count - 1) + 1;
}

int get_length() {
	return length;
}
int get_speed() {
	return speed;
}
int get_start_way() {
	return start_way;
}
int get_end_way() {
	return end_way;
}
