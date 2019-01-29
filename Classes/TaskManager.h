#pragma once
#include "WordTask.h"

class TaskManager
{
	WordTask *listOfTasks;
	int numTasks;
	int currentTask;

	void swap(WordTask &, WordTask &);

public:

	int getNumTasks();
	void setNumTasks(int);
	int init(int numTasks = 3);
	WordTask* getTask(int);
	WordTask* getRandTask();
	void restart();
	void save();
	void statistics();

	TaskManager();
	~TaskManager();
};
