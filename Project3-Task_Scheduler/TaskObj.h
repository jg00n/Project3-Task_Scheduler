#pragma once
//Task Codes
enum class cond {
	Ready,
	Run
};

struct Task {
	//Task Properties

	int ID = 0;				//Process ID (Task Name)
	int CPU_Time = 0;		//total run time spend executed by the CPU. 
	int Priority = 0;		//Priority of process
	cond Status = cond::Ready;
	//Temp Counters.
	int CPU_Counter = 0;
	//Record Time
	int wall		= 0;	//Run Time
	int wall_start	= 0;	//Start Tick
	int wall_end	= 0;	//End Tick
};