#pragma once


class train {

	int length;
	int speed;
	int start_way;
	int end_way;

public:

	train(int max_len, int ways_count);
	~train();
	int get_length();
	int get_speed();
	int get_start_way();
	int get_end_way();
};