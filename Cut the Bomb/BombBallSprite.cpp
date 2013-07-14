//
//  BombBallSprite.cpp
//  Cut the Bomb
//
//  Created by jordan Cortet on 11/07/13.
//
//

#include "BombBallSprite.h"


using namespace cocos2d;

BombBallSprite::BombBallSprite() {
   
}
BombBallSprite::~BombBallSprite() {
    
}

BombBallSprite* BombBallSprite::spriteWithWorld(b2World *world)
{
    BombBallSprite *pobBombBallSprite = new BombBallSprite();
    if (pobBombBallSprite && pobBombBallSprite->initWithWorld(world))
    {
        pobBombBallSprite->autorelease();
        return pobBombBallSprite;
    }
    CC_SAFE_DELETE(pobBombBallSprite);
    return NULL;
}

bool BombBallSprite::initWithWorld(b2World *world)
{

    string name = "obj_bomb1_001";

    CCSize screen = CCDirector::sharedDirector()->getWinSize();
    
    b2Body *body = createBodyForWorld(world, b2Vec2(screen.width/2/PTM_RATIO, screen.height/2/PTM_RATIO), 0, 5.0, .2, .2, name);
    
    PolygonSprite::initWithFile(name + ".png", body, true);
    return true;
}
