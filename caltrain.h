#include <pthread.h>

struct station {
	int passengers_waiting;
	int empty_places;
	int people_on_board;
	pthread_cond_t train_in_station;
	pthread_cond_t train_ready;
	pthread_mutex_t station_lock;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
