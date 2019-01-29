/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include <cocos2d.h>
#include "SimpleAudioEngine.h"
#include "WordTask.h"
#include "TaskManager.h"

using namespace cocos2d;
using namespace CocosDenshion;

class HelloWorld : public cocos2d::Layer
{
	int livesCount;
	Vector<Label*> livesArray;
	TaskManager tm;
	WordTask* task;
	Label* taskLabel;
	Label* scoreLabel;
	int score;
	int highScore;
	// for prevent score farming
	bool scoreCanChange;

	float taskHideTime = 1.5f;

	bool win;
	Scene *endScene;
	Layer *pauseLayer;
	bool sliderChanged;

	SimpleAudioEngine *audio;

	void updateTask(float dt);
	void initPosMove();
	void addScore(int score);
	void setScore(int score);

	void pauseAllTaskActions();
	void resumeAllTaskActions();

	void wrongAnswer(float dt);
	void createEndScene();
	void createPauseLayer();
	void showEnd(float dt);
	void showPauseCallback(Ref* sender);

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void menuReplayCallback(Ref* pSender);
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

	Label* CreateTextLabel(std::string text, char* ff, int fSize, Vec2 pos);

	void MoveObject(Sprite *ojb, Vec2 moveto, Vec2 scaleby);

	void InitLives(int livesCount);
	void ShowAnswer();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

};

#endif // __HELLOWORLD_SCENE_H__
