#include<iostream>
#include<pthread.h>
#include<chrono>
#include<time.h>
#include"skip_list.h"
#include<cstdlib>

const int NUM_THREADS = 1;
const int TEST_COUNT = 150000;

#define FILE_PATH "./store"

using namespace std;

SkipList<int, string> skiplist(18);

void* insert_element(void* pthread_id) {
	long tid;
	tid = (long)(pthread_id);
	cout << tid << endl;
	for (int count = 0; count < TEST_COUNT; count++) {
		skiplist.insert_element(rand() % TEST_COUNT, "a");
	}
	//skiplist.dump_file();
	pthread_exit(NULL);
}

void* search_element(void* pthread_id) {
	long tid;
	tid = (long)(pthread_id);
	cout << tid << endl;
	for (int count = 0; count < TEST_COUNT; count++) {
		skiplist.search_element(rand() % TEST_COUNT);
	}
	pthread_exit(NULL);
}

int main() {
	srand((unsigned)time(NULL));
	{
		pthread_t threads[NUM_THREADS];
		int rc;
		auto start = chrono::high_resolution_clock::now();
		for (int i = 0; i < NUM_THREADS; i++) {
			cout << "create thread" << i << endl;
			rc = pthread_create(&threads[i], NULL, insert_element, (void*) i);
			if (rc){
				cout << "unable to create thread" << endl;
				exit(-1);
			}
			for(int i = 0; i < NUM_THREADS; i++)
				if (pthread_join(threads[i], NULL) != 0) {
					cout << "join error" << endl;
					exit(3);
				}
		}
		auto finish = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed = finish - start;
		cout << "insert elapse =" << elapsed.count() << endl;
	}
	getchar();
	{
		pthread_t threads[NUM_THREADS];
		int rc;
		auto start = chrono::high_resolution_clock::now();
		for (int i = 0; i < NUM_THREADS; i++) {
			cout << "create thread" << i << endl;
			rc = pthread_create(&threads[i], NULL, search_element, (void*)i);
			if (rc) {
				cout << "unable to create thread" << endl;
				exit(-1);
			}
			for (int i = 0; i < NUM_THREADS; i++)
				if (pthread_join(threads[i], NULL) != 0) {
					cout << "join error" << endl;
					exit(3);
				}
		}
		auto finish = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed = finish - start;
		cout << "search elapse =" << elapsed.count() << endl;
	}
	pthread_exit(NULL);
	return 0;
}