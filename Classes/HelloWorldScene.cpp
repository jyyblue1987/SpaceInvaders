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
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	// Init Variables
	m_nMoveFlag = 0;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	
	// add ship player
	this->ship = Sprite::create("charactor/space-shuttle.png");
	if (this->ship == nullptr)
	{
		problemLoading("'charactor/space-shuttle.png'");
	}
	else
	{
		// position the sprite on the center of the screen
		this->ship->setPosition(Vec2(visibleSize.width / 2 + origin.x, origin.y + 10));
		this->ship->setScale(0.17f);

		// add the sprite as a child to this layer
		this->addChild(this->ship, 0);
	}

	onRegisterTouchEvent();

	scheduleUpdate();

    return true;
}

void HelloWorld::onRegisterTouchEvent()
{
	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Vec2 pos = touch->getLocation();

	if (pos.x < origin.x + visibleSize.width / 2)
		m_nMoveFlag = 1; // Left
	else
		m_nMoveFlag = 2; // Right		
	
	return true;
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
	m_nMoveFlag = 0;
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{
	
}

void HelloWorld::onTouchCancelled(Touch* touch, Event* event)
{
	
}

void HelloWorld::update(float dt) {
	
	/*Vec2 gravity = Vec2(0.0f, -300.0f);
	Vec2 GravityStep = ccpMult(gravity, dt);

	velocity = ccpAdd(velocity, GravityStep);
	Vec2 StepVelocity = ccpMult(velocity, dt);

	position = ccpAdd(position, StepVelocity);
	ember->setPosition(position);
	CCLOG("Entro: %f", position.y);*/

	moveShip(dt);

}

void HelloWorld::moveShip(float dt)
{
	if (m_nMoveFlag == 0)
		return;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto ship_size = this->ship->getContentSize();

	float bound_left = origin.x + 5 + ship_size.width / 2;
	float bound_right = origin.x + visibleSize.width - (5 + ship_size.width / 2);

	float velocity = 50;

	Vec2 cur_pos = this->ship->getPosition();
	float gap = velocity * dt;
	if (m_nMoveFlag == 1)		// Left Move
	{
		// Move Left action		
		cur_pos.x -= gap;
		if (cur_pos.x <= bound_left)
			cur_pos.x = bound_left;
	}
	else
	{		
		// Move Right action		
		cur_pos.x += gap;
		if (cur_pos.x >= bound_right)
			cur_pos.x = bound_right;
	}

	this->ship->setPosition(cur_pos);
}