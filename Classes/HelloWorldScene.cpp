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

#include "HelloWorldScene.h"
#include "ui/CocosGUI.h"

static char* const s_labelFontName = "fonts/arial.ttf";
static const int s_labelFontSize = 24;

static const Vec2 s_scenePadding = Vec2(5, 5);

static char* const s_hearth = u8"\u2665";
static const int s_hearthSize = 30;

static const int s_maxLives = 5;

static const int s_scoreChange = 5;

static const float minHideTime = 1.5f;
static const float maxHideTime = 6.0f;

Scene* HelloWorld::createScene()
{
	auto scene = Scene::create();
	auto layer = HelloWorld::create();
	scene->addChild(layer);

	auto backgroundImage = Sprite::create("Images/background.jpg");
	// normalizing image to fullscreen
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 scale = Vec2(visibleSize.width / backgroundImage->getContentSize().width,
					  visibleSize.height / backgroundImage->getContentSize().height);
	backgroundImage->setAnchorPoint(Vec2(0, 0));
	backgroundImage->setScale(scale.x, scale.y);
	scene->addChild(backgroundImage, -1);

	// if image dont load properly
	auto layerColor = LayerColor::create(Color4B::WHITE);
	scene->addChild(layerColor, -2);
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	// get hishscore from file
	highScore = std::stoi(FileUtils::getInstance()->getStringFromFile("Data/highscore.txt"));

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	taskHideTime = (maxHideTime + minHideTime) / 2;

	taskLabel = CreateTextLabel("Temp", s_labelFontName, s_labelFontSize, Vec2());

	static int maxTasksCount = 20;
	tm.init(maxTasksCount);
	updateTask(0);

	// create score label
	static int s_scoreLeftOffset = 35;
	CreateTextLabel(u8"Счёт: ", s_labelFontName, s_labelFontSize, 
				    origin + Vec2(s_scoreLeftOffset + s_scenePadding.x, 
						visibleSize.height - s_labelFontSize / 2 - s_scenePadding.y));
	scoreLabel = CreateTextLabel("0", s_labelFontName, s_labelFontSize,
		origin + Vec2(s_scoreLeftOffset + s_scenePadding.x + s_labelFontSize * 3,
			visibleSize.height - s_labelFontSize / 2 - s_scenePadding.y));
	setScore(0);

	win = false;
	InitLives(s_maxLives);

	// add keypressed listener
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto pauseItem = MenuItemImage::create("Images/pause_normal.png", "Images/pause_selected.png",
		CC_CALLBACK_1(HelloWorld::showPauseCallback, this));
	static const int s_replayItemOffset = 40;
	pauseItem->setAnchorPoint(Vec2(0.5f, 1.0f));
	pauseItem->setPosition(origin + Vec2(visibleSize.width / 2, visibleSize.height - s_scenePadding.y));

	auto menu = Menu::create(pauseItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	audio = SimpleAudioEngine::getInstance();
	audio->preloadEffect("Sounds/success.mp3");
	audio->preloadEffect("Sounds/miss.mp3");

    return true;
}

Label* HelloWorld::CreateTextLabel(std::string text, char* ff, int fSize, Vec2 pos)
{
	Label *label = Label::createWithTTF(text, ff, fSize);
	label->setPosition(pos);
	label->setColor(Color3B::BLACK);
	this->addChild(label, 1, "text_label");
	return label;
}

void HelloWorld::MoveObject(Sprite *obj, Vec2 moveto, Vec2 scaleby)
{
	auto acMoveTo = MoveTo::create(taskHideTime, moveto);
	auto acScaleBy = ScaleBy::create(taskHideTime / 2, scaleby.x, scaleby.y);
	obj->runAction(Spawn::createWithTwoActions(acMoveTo, acScaleBy));
	scheduleOnce(schedule_selector(HelloWorld::wrongAnswer), taskHideTime);
	static const float updateDelay = 0.2f;
	scheduleOnce(schedule_selector(HelloWorld::updateTask), taskHideTime + updateDelay);
}

void HelloWorld::InitLives(int livesCount)
{
	this->livesCount = livesCount;

	if (livesArray.size() == 0)
	{
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		Size visibleSize = Director::getInstance()->getVisibleSize();

		livesArray = Vector<Label*>(livesCount);
		Vec2 pos = origin + Vec2(visibleSize.width - s_hearthSize / 2,
			visibleSize.height - s_hearthSize / 2) - s_scenePadding;
		while (livesCount > 0)
		{
			Label *hearth = CreateTextLabel(s_hearth, s_labelFontName, s_hearthSize, pos);
			hearth->setColor(Color3B::RED);
			livesArray.pushBack(hearth);

			pos.x -= s_hearthSize;
			livesCount--;
		}
		livesArray.reverse();
	}
	else
	{
		for (int i = 0; i < livesArray.size(); i++)
		{
			livesArray.at(i)->setColor(Color3B::RED);
		}
	}
}

void HelloWorld::ShowAnswer()
{
	taskLabel->setString(task->getAnswer());
	taskLabel->stopAllActions();
	unscheduleAllCallbacks();
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (task->isKeyCorrect(keyCode))
	{
		if (scoreCanChange)
		{
			ShowAnswer();
			task->successesInc();

			float speedMultiplicator = (minHideTime + maxHideTime) / taskHideTime;

			addScore(task->getAnswer().length() * s_scoreChange * speedMultiplicator);
			scheduleOnce(schedule_selector(HelloWorld::updateTask), 1);
			scoreCanChange = false;

			audio->stopAllEffects();
			audio->playEffect("Sounds/success.mp3");
		}
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		exit(0);
	}
	else
	{
		if (scoreCanChange)
		{
			wrongAnswer(0);

			audio->stopAllEffects();
			audio->playEffect("Sounds/miss.mp3");
		}
	}
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::menuReplayCallback(Ref* pSender)
{
	// delay to prepare for playing
	Sleep(1500);
	Director::getInstance()->popScene();
	setScore(0);
	tm.restart();
	updateTask(0);
	InitLives(s_maxLives);
}

void HelloWorld::updateTask(float dt)
{
	WordTask *nextTask = tm.getRandTask();
	
	if (nextTask == nullptr)
	{
		win = true;
		scheduleOnce(schedule_selector(HelloWorld::showEnd), 0);
	}
	else
	{
		task = nextTask;
		taskLabel->setString(task->getTask());
		task->appearsInc();
		initPosMove();
		scoreCanChange = true;
	}
}

void HelloWorld::initPosMove()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	static int s_wordOffsetFromTop = 100;
	Vec2 taskPos = Vec2(origin.x + visibleSize.width / 2, 
						origin.y + visibleSize.height - s_wordOffsetFromTop);
	taskLabel->setPosition(taskPos);
	taskLabel->setScale(1);

	Vec2 taskMoveTo = origin + Vec2(visibleSize.width / 2, -visibleSize.height / 4);
	Vec2 taskScaleBy = Vec2(2, 2);
	MoveObject((Sprite*)taskLabel, taskMoveTo, taskScaleBy);
}

void HelloWorld::addScore(int score)
{
	setScore(this->score + score);
}

void HelloWorld::setScore(int score)
{
	this->score = score;
	scoreLabel->setString(std::to_string(score));
}

void HelloWorld::pauseAllTaskActions()
{
	taskLabel->pauseSchedulerAndActions();
}

void HelloWorld::resumeAllTaskActions()
{
	taskLabel->resumeSchedulerAndActions();
}

void HelloWorld::wrongAnswer(float dt)
{
	if (livesCount > 0)
	{
		livesArray.at(--livesCount)->setColor(Color3B::GRAY);

		static const int s_xDifference = 20;
		static const float s_moveTime = 0.1f;

		auto moveToLeftAction = MoveBy::create(s_moveTime / 3, Vec2(-s_xDifference, 0));
		auto moveToRightAction = MoveBy::create(s_moveTime / 3, Vec2(2 * s_xDifference, 0));
		auto moveBackAction = MoveBy::create(s_moveTime / 3, Vec2(-s_xDifference, 0));

		auto moveSequence = Sequence::create(
			moveToLeftAction, 
			moveToRightAction, 
			moveBackAction,
			nullptr);

		taskLabel->runAction(moveSequence);
	}
	else
	{
		ShowAnswer();
		unscheduleAllSelectors();
		scheduleOnce(schedule_selector(HelloWorld::showEnd), 1);
	}
}

void HelloWorld::createEndScene()
{
	endScene = Scene::create();

	std::string labelWord = u8"Конец :(";

	if (win)
	{
		labelWord = u8"Выигрыш!!";
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	static const int s_endLabelOffset = 60;
	Vec2 textPos = origin + Vec2(visibleSize.width / 2, visibleSize.height / 2 + s_endLabelOffset);
	static const int s_endLabelScale = 3;
	Label *lbEnd = Label::createWithTTF(labelWord, s_labelFontName, s_endLabelScale * s_labelFontSize);
	lbEnd->setPosition(textPos);
	endScene->addChild(lbEnd);

	static const int s_scoreLabelOffset = 20;
	Vec2 scorePos = textPos - Vec2(0, lbEnd->getContentSize().height + s_scoreLabelOffset);
	Label *lbScore = Label::createWithTTF(u8"Счёт: " + std::to_string(score), s_labelFontName, s_labelFontSize);
	lbScore->setPosition(scorePos);
	endScene->addChild(lbScore);

	std::string highScoreString = u8"Рекорд: " + std::to_string(highScore);

	if (score > highScore)
	{
		highScoreString = u8"Новый рекорд!";
		highScore = score;

		FileUtils::getInstance()->writeStringToFile(std::to_string(highScore), "Data/highscore.txt");
	}

	static const int s_highScoreLabelOffset = 10;
	Vec2 highScorePos = scorePos - Vec2(0, lbScore->getContentSize().height + s_highScoreLabelOffset);
	Label *lbhScore = Label::createWithTTF(highScoreString, s_labelFontName, s_labelFontSize);
	lbhScore->setPosition(highScorePos);
	endScene->addChild(lbhScore);

	auto replayItem = MenuItemImage::create("Images/replay_normal.png", "Images/replay_selected.png",
		CC_CALLBACK_1(HelloWorld::menuReplayCallback, this));
	static const int s_replayItemOffset = 40;
	replayItem->setPosition(Vec2(highScorePos.x, highScorePos.y - lbhScore->getContentSize().height - s_replayItemOffset));

	auto menu = Menu::create(replayItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	endScene->addChild(menu, 1);

	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event) {
		if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
		{
			exit(0);
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, endScene);
}

void HelloWorld::createPauseLayer()
{
	taskLabel->pauseSchedulerAndActions();
	this->pauseSchedulerAndActions();
	taskLabel->setString(std::string(task->getTask().length(), '_'));

	sliderChanged = false;

	if (pauseLayer == nullptr)
	{
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		Size visibleSize = Director::getInstance()->getVisibleSize();

		static const int s_fontScaleFactorTitle = 3;
		static const int s_fontScaleFactor = 2;

		pauseLayer = Layer::create();
		pauseLayer->setScale(0.8f);
		pauseLayer->setOpacity(50);

		auto layerColor = LayerColor::create(Color4B::YELLOW);
		pauseLayer->addChild(layerColor, -2);

		Vec2 pauseTextPos = origin + Vec2(visibleSize.width / 2, visibleSize.height - s_scenePadding.y);
		Label *pauseTextLabel = Label::createWithTTF(u8"Пауза", s_labelFontName, s_fontScaleFactorTitle * s_labelFontSize);
		pauseTextLabel->setAnchorPoint(Vec2(0.5f, 1.0f));
		pauseTextLabel->setPosition(pauseTextPos);
		pauseTextLabel->setColor(Color3B::ORANGE);
		pauseLayer->addChild(pauseTextLabel);

		static const int speedOffset = 130;
		Vec2 speedTextPos = pauseTextPos - Vec2(0, pauseTextLabel->getContentSize().height + speedOffset);
		Label *speedTextLabel = Label::createWithTTF(u8"Скорость:", s_labelFontName, s_fontScaleFactor * s_labelFontSize);
		speedTextLabel->setPosition(speedTextPos);
		speedTextLabel->setColor(Color3B::ORANGE);
		pauseLayer->addChild(speedTextLabel);

		static const int sliderOffset = 30;
		Vec2 sliderPos = speedTextPos - Vec2(0, speedTextLabel->getContentSize().height / 2 + sliderOffset);
		auto slider = ui::Slider::create();
		slider->setPosition(sliderPos);
		slider->loadBarTexture("Images/slider_back.png");
		slider->loadSlidBallTextures("Images/slider_ball.png", "Images/slider_ball.png", "Images/slider_ball.png");
		slider->loadProgressBarTexture("Images/slider_fill.png");
		slider->setPercent(50);
		slider->addTouchEventListener([&](Ref* event, ui::Widget::TouchEventType type) {
			switch (type)
			{
			case ui::Widget::TouchEventType::ENDED:
			{
				int percent = ((ui::Slider*)pauseLayer->getChildByName("slider"))->getPercent();
				int reversedPercent = 100 - percent;
				float timeUnderMinValue = (maxHideTime - minHideTime) * reversedPercent / 100.0f;
				taskHideTime = minHideTime + timeUnderMinValue;

				// restart current word
				taskLabel->cleanup();
				this->unscheduleAllCallbacks();
				// for applying move after pause
				sliderChanged = true;

				break;
			}
			default:
				break;
			}
		});
		pauseLayer->addChild(slider, 0, "slider");

		static const int exitOffset = 200;
		Vec2 exitPos = sliderPos - Vec2(0, slider->getContentSize().height / 2 + exitOffset);
		auto exitButton = ui::Button::create("Images/exit_normal.png", "Images/exit_selected.png", "Images/exit_normal.png");
		exitButton->setPosition(exitPos);
		exitButton->setTitleText(u8"Продолжить");
		exitButton->setTitleColor(Color3B::YELLOW);
		exitButton->setTitleFontName(s_labelFontName);
		exitButton->setTitleFontSize(s_labelFontSize);
		exitButton->addTouchEventListener([&](Ref* event, ui::Widget::TouchEventType type) {
			switch (type)
			{
			case ui::Widget::TouchEventType::ENDED:
			{
				taskLabel->resumeSchedulerAndActions();
				this->resumeSchedulerAndActions();
				pauseLayer->setVisible(false);
				for (int i = 0; i < pauseLayer->getChildrenCount(); i++)
				{
					pauseLayer->getChildren().at(i)->setVisible(false);
				}
				taskLabel->setString(task->getTask());

				if (sliderChanged)
				{
					initPosMove();
				}

				break;
			}
			default:
				break;
			}
		});
		pauseLayer->addChild(exitButton);

		this->addChild(pauseLayer, 5);
	}
	else
	{
		pauseLayer->setVisible(true);
		for (int i = 0; i < pauseLayer->getChildrenCount(); i++)
		{
			pauseLayer->getChildren().at(i)->setVisible(true);
		}
	}
}

void HelloWorld::showEnd(float dt)
{
	createEndScene();
	Director::getInstance()->pushScene(endScene);
}

void HelloWorld::showPauseCallback(Ref* sender)
{
	createPauseLayer();
}
