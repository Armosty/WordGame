#include "TaskManager.h"
#include "cocos-ext.h"

using namespace rapidjson;

TaskManager::TaskManager()
{
	listOfTasks = nullptr;
	numTasks = 3;
}

TaskManager::~TaskManager()
{
	save();
	statistics();
	delete[] listOfTasks;
	listOfTasks = nullptr;
}

int TaskManager::init(int numTask)
{
	std::string file = FileUtils::getInstance()->getStringFromFile("Data/words.json");

	Document document;
	document.Parse <0>(file.c_str());

	if (document.HasMember("tasks"))
	{
		this->numTasks = MIN(numTask, document["tasks"].Size());
		listOfTasks = new WordTask[this->numTasks];

		for (SizeType i = 0; i < this->numTasks; i++)
		{
			std::string task = document["tasks"][i]["task"].GetString();
			std::string answer = document["tasks"][i]["answer"].GetString();

			int missedPos = 0;
			for (; missedPos < task.length(); missedPos++)
			{
				if (task[missedPos] == '_')
				{
					break;
				}
			}

			std::string key = answer.substr(missedPos, 2);
			int successes = document["tasks"][i]["successes"].GetInt();
			int appears = document["tasks"][i]["appears"].GetInt();

			listOfTasks[i].init(task, answer, key, successes, appears);
		}
	}

	srand(time(0));
	restart();

	return this->numTasks;
}

int TaskManager::getNumTasks()
{
	return numTasks;
}

void TaskManager::setNumTasks(int num)
{
	if (num >= 1)
	{
		numTasks = num;
	}
	else
	{
		numTasks = 1;
	}
}

WordTask* TaskManager::getTask(int count)
{
	if (count >= 0 && count < numTasks)
	{
		return &listOfTasks[count];
	}
	return nullptr;
}

WordTask* TaskManager::getRandTask()
{
	if (currentTask >= numTasks)
	{
		return nullptr;
	}
	return getTask(currentTask++);
}

void TaskManager::swap(WordTask & first, WordTask & second)
{
	WordTask temp = first;
	first = second;
	second = temp;
}

void TaskManager::restart()
{
	static const int arranges = 100;

	for (int i = 0; i < arranges; i++)
	{
		int firstIndex = random(0, numTasks - 1);
		int secondIndex = random(0, numTasks - 1);

		swap(listOfTasks[firstIndex], listOfTasks[secondIndex]);
	}
	currentTask = 0;
}

void TaskManager::save()
{
	std::string result = "{\n\t\"tasks\": [\n";
	for (int i = 0; i < numTasks; i++)
	{
		WordTask task = listOfTasks[i];

		std::string taskText = task.getTask();
		std::string answer = task.getAnswer();
		int missedPos = 0;
		for (; missedPos < taskText.length(); missedPos++)
		{
			if (taskText[missedPos] == '_')
			{
				break;
			}
		}

		result += "\t\t{\n\t\t\t\"task\": \"" + taskText + "\",\n";
		result += "\t\t\t\"answer\": \"" + answer + "\",\n";
		result += "\t\t\t\"key\": \"" + answer.substr(missedPos, 2) + "\",\n";
		result += "\t\t\t\"successes\": " + std::to_string(task.getSuccesses()) + ",\n";
		result += "\t\t\t\"appears\": " + std::to_string(task.getAppears()) + "\n\t\t}";

		if (i + 1 < numTasks)
		{
			result += ",";
		}
		result += "\n";
	}
	result += "\t]\n}";

	FileUtils::getInstance()->writeStringToFile(result, "Data/words.json");
}

void TaskManager::statistics()
{
	std::string result = "";
	for (int i = 0; i < numTasks; i++)
	{
		WordTask task = listOfTasks[i];
		result += task.getAnswer() + ": " + std::to_string(task.getAppears()) + " appears, " + std::to_string(task.getSuccesses()) + " right answers.\n";
	}

	FileUtils::getInstance()->writeStringToFile(result, "Data/stats.txt");
}
