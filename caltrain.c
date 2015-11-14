#include <pthread.h>
#include "caltrain.h"

void station_init(struct station *station) //initialize station
{
	station->passengers_waiting = 0;        //no waiting passengers at the beginning
	station->empty_places = 0;              //no available empty places
	station->people_on_board = 0;           //no people on board the train
	//mutex and condition variables init
	pthread_mutex_init(&station->station_lock,NULL);
    pthread_cond_init(&station->train_in_station,NULL);
    pthread_cond_init(&station->train_ready,NULL);
}

void station_load_train(struct station *station, int count)
{
    pthread_mutex_lock(&station->station_lock); //locks before critical section
	station->empty_places = count;              //available empty seats = count (empty seats on train)
	while((station->empty_places >0 && station->passengers_waiting >0) || station->people_on_board!=0){
        //while the train has empty places and there are passengers or
        pthread_cond_broadcast(&station->train_in_station);         //announce train has arrived
        pthread_cond_wait(&station->train_ready, &station->station_lock);   //wait for the train to be ready
	}
	station->empty_places = 0;      //after train leaves available seats ae 0
	pthread_mutex_unlock(&station->station_lock);   //unlock after critical section
}

void station_wait_for_train(struct station *station)
{
    pthread_mutex_lock(&station->station_lock);     //locks before critical section
    station->passengers_waiting++;                  //passenger arrives, waiting count increases
    while(station->empty_places <=0){
        //wait for train to arrive while there's no empty seats
        pthread_cond_wait(&station->train_in_station, &station->station_lock);
    }
    station->empty_places--;                        //passenger rides decrement empty seats
    station->passengers_waiting--;                  //passenger rides decrement passengers waiting
    station->people_on_board++;                     //passenger rides increment passengers to board
    pthread_mutex_unlock(&station->station_lock);   //unlock after critical section
}

void station_on_board(struct station *station)
{
    pthread_mutex_lock(&station->station_lock);         //locks before critical section
	station->people_on_board--;                         //decrement number of passengers to board as passenger boarded
	/*send signal that train ready but train checks if there are empty
	seats and passengers waiting or people to board else it leaves*/
    pthread_cond_signal(&station->train_ready);
	pthread_mutex_unlock(&station->station_lock);      //unlock after critical section
}

