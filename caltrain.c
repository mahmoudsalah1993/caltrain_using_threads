#include <pthread.h>
#include "caltrain.h"


void station_init(struct station *station)
{
	station->passengers_waiting = 0;
	station->empty_places = 0;
	station->people_on_board = 0;
	pthread_mutex_init(&station->station_lock,NULL);
    pthread_cond_init(&station->train_in_station,NULL);
    pthread_cond_init(&station->train_ready,NULL);
}

void station_load_train(struct station *station, int count)
{
    pthread_mutex_lock(&station->station_lock);
	station->empty_places = count;
	while((station->empty_places >0 && station->passengers_waiting >0) || station->people_on_board!=0){
        pthread_cond_broadcast(&station->train_in_station);
        pthread_cond_wait(&station->train_ready, &station->station_lock);
	}
	station->empty_places = 0;
	pthread_mutex_unlock(&station->station_lock);
}

void station_wait_for_train(struct station *station)
{
    pthread_mutex_lock(&station->station_lock);
    station->passengers_waiting++;
    while(station->empty_places <=0){
        pthread_cond_wait(&station->train_in_station, &station->station_lock);
    }
    station->empty_places--;
    station->passengers_waiting--;
    station->people_on_board++;
    pthread_mutex_unlock(&station->station_lock);
}

void station_on_board(struct station *station)
{
    pthread_mutex_lock(&station->station_lock);
	station->people_on_board--;
    pthread_cond_signal(&station->train_ready);
	pthread_mutex_unlock(&station->station_lock);
}
