#pragma once
#include <deque>
#include <ctime>
#include <algorithm>
#include "TaskObj.h"

class Task_Control_Block {
	static const int TYPE = 4;				//Number of algorithms to generate
	
	std::deque<Task> PID_MasterCopy;		//Main copy of all processes
	//Copy agent for each algorithm

	struct Agent {
		int ProcessSize = 0;				//Max size of completed tasks 
		int ProcessCompletion = 0;			//Counter to keep track of completed tasks.

		std::deque<Task> PID_Copy;			//Copy of generated process
		std::deque<Task> PID_Result;		//Finished sequence of generated process
	};

public:
	static const int COUNT = 10;			//Process creation size

	Agent Process[TYPE];					//Declare agent copy for algorithm to use
	Task_Control_Block() {

		std::srand(time(NULL));						//Generate random seed.
		//Set the common sizes for each agent.
		for (int i = 0; i < TYPE; i++) {
			Process[i].ProcessSize = COUNT;
		}
	}
	void Generate() {
		int logged_rand[COUNT] = {0};	//Keep Track of unique numbers
		int unique_index = 0;			//Keep Track of Unique IDs
		bool unique;					//Eval if number is unique or not.

		for (int i = 0; i < COUNT; i++) {
			//Create Packet
			Task packet;
			//Sign Packet
			packet.ID = i;					//ID index
			int newItem;
			do {
				unique = true;
				newItem = rand() % 100;
				for (int i = 0; i < COUNT; i++) {
					if (logged_rand[i] == newItem) {
						unique = false;
						break;
					}
				}
				if (unique) {
					logged_rand[unique_index] = newItem;
					unique_index++;
				}
			} while (!unique);
			packet.wall_start = newItem;				//Random Creation Time (Start Time)
			packet.CPU_Time	= rand() % 500 + 50;			//Random cpu time between 100, 1.
			packet.Priority = rand() % COUNT + 1;		//Give packet random priority 1-10
			PID_MasterCopy.push_back(packet);
		}
		std::deque<Task> temp;
		//Sort Process by wall start time.
		std::sort(PID_MasterCopy.begin(), PID_MasterCopy.end(), [](const Task& a, const Task& b) {
			return a.wall_start < b.wall_start;
			});
		//Reset Process Start ID
		for (int i = 0; i < COUNT; i++) {
			printf("%d,", PID_MasterCopy[i].wall_start);
			PID_MasterCopy[i].ID = i+1;
		}
		printf("\n");
		//Issue a copy for each Agent.
		for (int i = 0; i < TYPE; i++) {
			Process[i].PID_Copy = PID_MasterCopy;
			Process[i].ProcessSize = COUNT;
		}
	}

};