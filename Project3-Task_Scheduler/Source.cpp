//Required Libraries
#include <fstream>				//For printing to file.
#include <string>				//For naming a file. and output
#include <iostream>

#include <filesystem>			//For getting file system location
#include "ProcessBroker.h"		//Includes <Deque>, <CTime>,

//Functional Prototypes.
void FCFS(Task_Control_Block&);
void RR(Task_Control_Block&);
void Priority(Task_Control_Block&);
void SJF(Task_Control_Block&);
void Print(Task_Control_Block&);

//Contains the operation modes for the CPU
enum class CPU {
	Idle,
	Busy
};

int main() {
	
	Task_Control_Block TCB_Algorithm;			//Create Process Obj
	TCB_Algorithm.Generate();					//Generate Random Process IDS

	FCFS(TCB_Algorithm);
	RR(TCB_Algorithm);
	Priority(TCB_Algorithm);
	SJF(TCB_Algorithm);
	Print(TCB_Algorithm);
	return 0;
}

void FCFS(Task_Control_Block &TCB_Algorithm) {
	CPU		Status = CPU::Idle;					//Initialize CPU mode.
	int		completed_process = 0;				//Counter to keep track of processes.
	int		time = 0;							//Keep track of tick time.

	std::deque<Task> ready;

	while (TCB_Algorithm.Process[0].ProcessCompletion < TCB_Algorithm.Process[0].ProcessSize) {
		//Display information of current tick (Ommited).

		//Load Packet
		if (!TCB_Algorithm.Process[0].PID_Copy.empty()) {
			if (TCB_Algorithm.Process[0].PID_Copy.front().wall_start == time) {
				Task packet = TCB_Algorithm.Process[0].PID_Copy.front();

				if (Status == CPU::Idle) {
					//Immediately process packet.
					packet.Status = cond::Run;
					Status = CPU::Busy;
					//First packet to process
					ready.push_front(packet);
				}
				else {
					//Put packet into back of ready queue.
					packet.Status = cond::Ready;
					ready.push_back(packet);
				}
				TCB_Algorithm.Process[0].PID_Copy.pop_front();
			}

		}

		//Process and update packets
		if (Status == CPU::Busy && !ready.empty()) {

			if (ready.front().CPU_Counter >= ready.front().CPU_Time) {
				completed_process++;	//Increment counter.
				ready.front().wall_end = time;
				ready.front().wall = ready.front().wall_end - ready.front().wall_start;
				//Put completed object in broker, update completion counter
				TCB_Algorithm.Process[0].ProcessCompletion++;
				TCB_Algorithm.Process[0].PID_Result.push_back(ready.front());
				ready.pop_front();
			}
		}
		else;			//Wait

		//Process The top packet.
		if (!ready.empty() && Status == CPU::Busy) {
			if (ready.front().CPU_Counter < ready.front().CPU_Time) {
				ready.front().CPU_Counter++;
			}
		}
		time++;
	}
	printf("Completed FCFS Sequence!\n");

}

//Round Robin Algorithm
void RR(Task_Control_Block& TCB_Algorithm) {
	CPU		Status = CPU::Idle;					//Initialize CPU mode.
	int		completed_process = 0;				//Counter to keep track of processes.
	int		time = 0;							//Keep track of tick time.
	const int	QUANTUM		= 10;				//Time Quantum
	int			quantum_c	= 0;				//Counter for Quantum

	std::deque<Task> ready;

	while (TCB_Algorithm.Process[1].ProcessCompletion < TCB_Algorithm.Process[1].ProcessSize) {
		//Display information of current tick (See first sample).

		//Load Packet
		if (!TCB_Algorithm.Process[1].PID_Copy.empty()) {
			if (TCB_Algorithm.Process[1].PID_Copy.front().wall_start == time) {
				Task packet = TCB_Algorithm.Process[1].PID_Copy.front();

				if (Status == CPU::Idle) {
					//Immediately process packet.
					packet.Status = cond::Run;
					Status = CPU::Busy;
					//First packet to process
					ready.push_front(packet);
				}
				else {
					//Put packet into back of ready queue.
					packet.Status = cond::Ready;
					ready.push_back(packet);
				}
				TCB_Algorithm.Process[1].PID_Copy.pop_front();
			}

		}

		//Process and update packets
		if (Status == CPU::Busy && !ready.empty()) {

			if (ready.front().CPU_Counter >= ready.front().CPU_Time) {
				completed_process++;	//Increment counter.
				quantum_c = 0;			//Reset Time Quantum
				ready.front().wall_end = time;
				ready.front().wall = ready.front().wall_end - ready.front().wall_start;
				//Put completed object in broker, update completion counter
				TCB_Algorithm.Process[1].ProcessCompletion++;
				TCB_Algorithm.Process[1].PID_Result.push_back(ready.front());
				ready.pop_front();
			}
		}
		else;			//Wait

		//Process The top packet. If it has exceeded the quantum, shuffle the ready queue.
		if (!ready.empty() && Status == CPU::Busy) {
			if (ready.front().CPU_Counter < ready.front().CPU_Time) {

				//Reset quantum counter and move current packet to back.
				if (quantum_c > QUANTUM) {
					ready.push_back(ready.front());
					ready.pop_front();
					quantum_c = 0;
				}
				//Update Quantum Counter and the CPU_Counter.
				quantum_c++;
				ready.front().CPU_Counter++;
			}
		}
		time++;

	}
	printf("Completed RR Sequence!\n");
}

//Priority Algorithm
void Priority(Task_Control_Block& TCB_Algorithm) {
	CPU		Status = CPU::Idle;					//Initialize CPU mode.
	int		completed_process = 0;				//Counter to keep track of processes.
	int		time = 0;							//Keep track of tick time.

	std::deque<Task> ready;

	while (TCB_Algorithm.Process[2].ProcessCompletion < TCB_Algorithm.Process[2].ProcessSize) {
		//Load Packet
		if (!TCB_Algorithm.Process[2].PID_Copy.empty()) {
			if (TCB_Algorithm.Process[2].PID_Copy.front().wall_start == time) {
				Task packet = TCB_Algorithm.Process[2].PID_Copy.front();

				if (Status == CPU::Idle) {
					//Immediately process packet.
					packet.Status = cond::Run;
					Status = CPU::Busy;
					//First packet to process
					ready.push_front(packet);
				}
				else {
					//Put packet into back of ready queue.
					packet.Status = cond::Ready;
					ready.push_back(packet);
				}
				TCB_Algorithm.Process[2].PID_Copy.pop_front();
			}

		}

		//Process and update packets
		if (Status == CPU::Busy && !ready.empty()) {

			if (ready.front().CPU_Counter >= ready.front().CPU_Time) {
				completed_process++;	//Increment counter.
				ready.front().wall_end = time;
				ready.front().wall = ready.front().wall_end - ready.front().wall_start;
				//Put completed object in broker, update completion counter
				TCB_Algorithm.Process[2].ProcessCompletion++;
				TCB_Algorithm.Process[2].PID_Result.push_back(ready.front());
				ready.pop_front();
				//Sort ready queue based on priority.
				std::sort(ready.begin(), ready.end(), [](const Task& a, const Task& b) {
					return a.Priority < b.Priority;
				});
			}
		}
		else;			//Wait

		//Process The top packet.
		if (!ready.empty() && Status == CPU::Busy) {
			if (ready.front().CPU_Counter < ready.front().CPU_Time) {
				ready.front().CPU_Counter++;
			}
		}
		time++;
	}
	printf("Completed Priority Sequence!\n");
}

//Shortest Job First
void SJF(Task_Control_Block& TCB_Algorithm) {
	CPU		Status = CPU::Idle;					//Initialize CPU mode.
	int		completed_process = 0;				//Counter to keep track of processes.
	int		time = 0;							//Keep track of tick time.

	std::deque<Task> ready;

	while (TCB_Algorithm.Process[3].ProcessCompletion < TCB_Algorithm.Process[3].ProcessSize) {
		//Display information of current tick (See first sample).

		//Load Packet
		if (!TCB_Algorithm.Process[3].PID_Copy.empty()) {
			if (TCB_Algorithm.Process[3].PID_Copy.front().wall_start == time) {
				Task packet = TCB_Algorithm.Process[3].PID_Copy.front();

				if (Status == CPU::Idle) {
					//Immediately process packet.
					packet.Status = cond::Run;
					Status = CPU::Busy;
					//First packet to process
					ready.push_front(packet);
				}
				else {
					//Put packet into back of ready queue.
					packet.Status = cond::Ready;
					ready.push_back(packet);
					//std::printf("Waiting for processing!\n");
				}
				TCB_Algorithm.Process[3].PID_Copy.pop_front();
			}

		}

		//Process and update packets
		if (Status == CPU::Busy && !ready.empty()) {

			if (ready.front().CPU_Counter >= ready.front().CPU_Time) {
				completed_process++;	//Increment counter.
				ready.front().wall_end = time;
				ready.front().wall = ready.front().wall_end - ready.front().wall_start;
				//Put completed object in broker, update completion counter
				TCB_Algorithm.Process[3].ProcessCompletion++;
				TCB_Algorithm.Process[3].PID_Result.push_back(ready.front());
				ready.pop_front();
				//Sort ready queue based on remaining time.
				std::sort(ready.begin(), ready.end(), [](const Task& a, const Task& b) {
					return (a.CPU_Time - a.CPU_Counter) < (b.CPU_Time - b.CPU_Counter);
					});
			}
		}
		else;			//Wait

		//Process The top packet.
		if (!ready.empty() && Status == CPU::Busy) {
			if (ready.front().CPU_Counter < ready.front().CPU_Time) {
				ready.front().CPU_Counter++;
			}
		}
		time++;
	}
	printf("Completed Shortest Job First Sequence!\n");
}

void Print(Task_Control_Block& TCB_Algorithm) {
	bool started = false, unique = false;
	std::ofstream File_out;
	if (!started) {
		int fileNo = 1;
		while (!unique && fileNo < INT_MAX) {
			std::string name = "Object_" + std::to_string(fileNo) + ".txt";
			std::ifstream ifs;
			ifs.open(name, std::ios::in);// attempt read file to check if it exists
			if (!ifs) {
				unique = true;
			}
			else {
				fileNo++;//increase by one to get a new file name
			}
			ifs.close();
		}
		File_out.open("Object_" + std::to_string(fileNo) + ".txt", std::ios::app);
		started = true;
	}
	for (int i = 0; i < 4; i++) {
		double turn_Average = 0, CPU_Total = 0;
		std::cout <<	"Method:";
		File_out <<		"Method:";
		std::string type;
		if (i == 0) {
			type = "FCFS";
		}
		else if (i ==1) {
			type = "RR";
		}
		else if (i == 2) {
			type = "Priority";
		}
		else if (i == 3) {
			type = "SJF";
		}
		std::cout << type << "\n";
		File_out << type << "\n";
		std::cout <<	"TaskNumber		TaskName	CPU_Time	Turnaround_Time		Start_Tick		End_Tick\n";
		File_out <<		"TaskNumber		TaskName	CPU_Time	Turnaround_Time		Start_Tick		End_Tick\n";

		int j = 0;
		while( j < TCB_Algorithm.Process[i].ProcessSize) {
			Task temp = TCB_Algorithm.Process[i].PID_Result.front();
			TCB_Algorithm.Process[i].PID_Result.pop_front();
			std::cout << temp.ID << "			T" << temp.ID << "		" << temp.CPU_Time << "		" << temp.wall << "			" << temp.wall_start << "			" << temp.wall_end << "\n";
			File_out << temp.ID << "			T" << temp.ID << "		" << temp.CPU_Time << "		" << temp.wall << "			" << temp.wall_start << "			" << temp.wall_end << "\n";
			turn_Average += temp.wall;
			CPU_Total += temp.CPU_Time;
			j++;
		}
		turn_Average = (turn_Average / 10);
		std::cout << "Average turnaround time for " << type << "= " << (turn_Average) << "\n";
		File_out << "Average turnaround time for " << type << "= " << (turn_Average) << "\n";
		std::cout << "Total CPU time for " << type << "= " << (CPU_Total) << "\n\n";
		File_out << "Total CPU time for " << type << "= " << (CPU_Total) << "\n\n";
	}
	File_out.close();
}