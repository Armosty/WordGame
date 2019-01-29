#include <string>
#include "WordTask.h"

static int s_wrongKeyError = -1;
static int s_wrongAnswerTextError = -2;
static int s_differentAnswerTaskError = -3;
static int s_wrongTaskTextError = -4;

int WordTask::init(std::string task, std::string answer, std::string key, int successes, int appears) 
{
	this->key = russianLetterToKeyCode(key);
	this->answer = answer;
	this->task = task;
	this->successes = successes;
	this->appears = appears;

	if (!isAlphabetKey(this->key))
	{
		return s_wrongKeyError;
	}

	if (!isRussianWord(answer))
	{
		return s_wrongAnswerTextError;
	}

	// count lengths equality and different letters, allows 1 '_' symbol
	if (task.length() != answer.length())
	{
		return s_differentAnswerTaskError;
	}

	int counter = 0;
	for (int a = 0, t = 0; a < answer.size(); a++, t++)
	{
		if (answer[a] != task[t])
		{
			if (task[t] == '_')
			{
				counter++;
			}
			else
			{
				return s_differentAnswerTaskError;
			}
		}
	}

	// one different letter for hideen letter
	if (counter > 1)
	{
		return s_differentAnswerTaskError;
	}
	
	return 0;
}

bool WordTask::isKeyCorrect(EventKeyboard::KeyCode keyCode)
{
	return this->key == keyCode || 
		// for allowing � as �
		this->key == EventKeyboard::KeyCode::KEY_GRAVE && keyCode == EventKeyboard::KeyCode::KEY_T;
}

int WordTask::setKey(EventKeyboard::KeyCode key)
{
	if (!isAlphabetKey(key))
	{
		return s_wrongKeyError;
	}

	this->key = key;
	return 0;
}

int WordTask::setAnswer(std::string answer)
{
	if (!isRussianWord(answer))
	{
		return s_wrongAnswerTextError;
	}

	this->answer = answer;
	return 0;
}

int WordTask::setTask(std::string task)
{
	if (!isRussianWord(task, "_"))
	{
		return s_wrongTaskTextError;
	}

	this->task = task;
	return 0;
}

// cheks symbol for correctness, allows only letters, also �, �, �, �, �, and �
bool WordTask::isAlphabetKey(EventKeyboard::KeyCode keyCode)
{
	return keyCode >= EventKeyboard::KeyCode::KEY_A && keyCode <= EventKeyboard::KeyCode::KEY_Z ||
		   keyCode >= EventKeyboard::KeyCode::KEY_CAPITAL_A && keyCode <= EventKeyboard::KeyCode::KEY_CAPITAL_Z ||
		   keyCode == EventKeyboard::KeyCode::KEY_LESS_THAN || 
		   keyCode == EventKeyboard::KeyCode::KEY_GREATER_THAN ||
		   keyCode == EventKeyboard::KeyCode::KEY_LEFT_BRACKET || 
		   keyCode == EventKeyboard::KeyCode::KEY_RIGHT_BRACKET ||
		   keyCode == EventKeyboard::KeyCode::KEY_QUOTE || 
		   keyCode == EventKeyboard::KeyCode::KEY_SEMICOLON;
}

// cheks for all symbols of word from russian letters
bool WordTask::isRussianWord(std::string word, std::string additionalSymbols)
{
	std::string allowedLetters = u8"�������������������������������������Ũ��������������������������";
	

	for (int i = 0; i < word.size(); i++)
	{
		if (allowedLetters.find(word[i]) == std::string::npos &&
			additionalSymbols.find(word[i]) == std::string::npos)
		{
			return false;
		}
	}
	return true;
}

EventKeyboard::KeyCode WordTask::russianLetterToKeyCode(std::string letter)
{
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_F;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_LESS_THAN;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_D;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_U;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_L;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_T;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_GRAVE;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_SEMICOLON;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_P;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_B;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_Q;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_R;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_K;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_V;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_Y;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_J;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_G;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_H;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_C;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_N;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_E;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_A;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_LEFT_BRACKET;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_W;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_X;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_I;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_O;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_RIGHT_BRACKET;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_S;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_M;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_QUOTE;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_GREATER_THAN;
	if (letter == u8"�" || letter == u8"�")
		return EventKeyboard::KeyCode::KEY_Z;
	return EventKeyboard::KeyCode::KEY_SPACE;
}
