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
		// for allowing å as ¸
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

// cheks symbol for correctness, allows only letters, also á, þ, õ, ú, æ, and ý
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
	std::string allowedLetters = u8"àáâãäå¸æçèéêëìíîïðñòóôõö÷øùúûüýþÿÀÁÂÃÄÅ¨ÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞß";
	

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
	if (letter == u8"À" || letter == u8"à")
		return EventKeyboard::KeyCode::KEY_F;
	if (letter == u8"Á" || letter == u8"á")
		return EventKeyboard::KeyCode::KEY_LESS_THAN;
	if (letter == u8"Â" || letter == u8"â")
		return EventKeyboard::KeyCode::KEY_D;
	if (letter == u8"Ã" || letter == u8"ã")
		return EventKeyboard::KeyCode::KEY_U;
	if (letter == u8"Ä" || letter == u8"ä")
		return EventKeyboard::KeyCode::KEY_L;
	if (letter == u8"Å" || letter == u8"Å")
		return EventKeyboard::KeyCode::KEY_T;
	if (letter == u8"¨" || letter == u8"¸")
		return EventKeyboard::KeyCode::KEY_GRAVE;
	if (letter == u8"Æ" || letter == u8"æ")
		return EventKeyboard::KeyCode::KEY_SEMICOLON;
	if (letter == u8"Ç" || letter == u8"ç")
		return EventKeyboard::KeyCode::KEY_P;
	if (letter == u8"È" || letter == u8"è")
		return EventKeyboard::KeyCode::KEY_B;
	if (letter == u8"É" || letter == u8"é")
		return EventKeyboard::KeyCode::KEY_Q;
	if (letter == u8"Ê" || letter == u8"ê")
		return EventKeyboard::KeyCode::KEY_R;
	if (letter == u8"Ë" || letter == u8"ë")
		return EventKeyboard::KeyCode::KEY_K;
	if (letter == u8"Ì" || letter == u8"ì")
		return EventKeyboard::KeyCode::KEY_V;
	if (letter == u8"Í" || letter == u8"í")
		return EventKeyboard::KeyCode::KEY_Y;
	if (letter == u8"Î" || letter == u8"î")
		return EventKeyboard::KeyCode::KEY_J;
	if (letter == u8"Ï" || letter == u8"ï")
		return EventKeyboard::KeyCode::KEY_G;
	if (letter == u8"Ð" || letter == u8"ð")
		return EventKeyboard::KeyCode::KEY_H;
	if (letter == u8"Ñ" || letter == u8"ñ")
		return EventKeyboard::KeyCode::KEY_C;
	if (letter == u8"Ò" || letter == u8"ò")
		return EventKeyboard::KeyCode::KEY_N;
	if (letter == u8"Ó" || letter == u8"ó")
		return EventKeyboard::KeyCode::KEY_E;
	if (letter == u8"Ô" || letter == u8"ô")
		return EventKeyboard::KeyCode::KEY_A;
	if (letter == u8"Õ" || letter == u8"õ")
		return EventKeyboard::KeyCode::KEY_LEFT_BRACKET;
	if (letter == u8"Ö" || letter == u8"ö")
		return EventKeyboard::KeyCode::KEY_W;
	if (letter == u8"×" || letter == u8"÷")
		return EventKeyboard::KeyCode::KEY_X;
	if (letter == u8"Ø" || letter == u8"ø")
		return EventKeyboard::KeyCode::KEY_I;
	if (letter == u8"Ù" || letter == u8"ù")
		return EventKeyboard::KeyCode::KEY_O;
	if (letter == u8"Ú" || letter == u8"ú")
		return EventKeyboard::KeyCode::KEY_RIGHT_BRACKET;
	if (letter == u8"Û" || letter == u8"û")
		return EventKeyboard::KeyCode::KEY_S;
	if (letter == u8"Ü" || letter == u8"ü")
		return EventKeyboard::KeyCode::KEY_M;
	if (letter == u8"Ý" || letter == u8"ý")
		return EventKeyboard::KeyCode::KEY_QUOTE;
	if (letter == u8"Þ" || letter == u8"þ")
		return EventKeyboard::KeyCode::KEY_GREATER_THAN;
	if (letter == u8"ß" || letter == u8"ÿ")
		return EventKeyboard::KeyCode::KEY_Z;
	return EventKeyboard::KeyCode::KEY_SPACE;
}
