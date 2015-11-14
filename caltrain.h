#include <pthread.h>

struct station {
	int passengers_waiting; //count of waiting passengers
	int empty_places;       //count of empty seats on train
	int people_on_board;    //count of people waiting that boarded
	pthread_cond_t train_in_station;    //condition variable to check that a train is in the station
	pthread_cond_t train_ready;         //condition variable to check that a train is full or no passengers are waiting
	pthread_mutex_t station_lock;       //mutual exclusion
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
