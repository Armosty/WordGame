#include <cocos2d.h>
#pragma once

using namespace cocos2d;

class WordTask
{
	std::string task;
	std::string answer;
	EventKeyboard::KeyCode key;
	int successes;
	int appears;

	static bool isAlphabetKey(EventKeyboard::KeyCode keyCode);
	static bool isRussianWord(std::string word, std::string additionalSumbols = "");
	static EventKeyboard::KeyCode russianLetterToKeyCode(std::string letter);

public:
	WordTask() = default;
	~WordTask() = default;

	int init(std::string task, std::string answer, std::string key, int successes, int appears);
	bool isKeyCorrect(EventKeyboard::KeyCode keyCode);

	EventKeyboard::KeyCode getKey() { return key; }
	std::string getAnswer() { return answer; }
	std::string getTask() { return task; }
	int getSuccesses() { return successes; }
	int getAppears() { return appears; }

	int setKey(EventKeyboard::KeyCode);
	int setAnswer(std::string);
	int setTask(std::string);

	void successesInc() { successes++; }
	void appearsInc() { appears++; }
};