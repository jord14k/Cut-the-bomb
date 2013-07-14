//
//  BombBallSprite.h
//  Cut the Bomb
//
//  Created by jordan Cortet on 11/07/13.
//
//

#ifndef __Cut_the_Bomb__BombBallSprite__
#define __Cut_the_Bomb__BombBallSprite__

#include <iostream>
#include "PolygonSprite.h"

#define PTM_RATIO 32

class BombBallSprite : public PolygonSprite::PolygonSprite
{
public:
    
    BombBallSprite();
    ~BombBallSprite();
    
    static BombBallSprite* spriteWithWorld(b2World *world);
    bool initWithWorld(b2World *world);
    
private:
    cocos2d::CCTexture2D* m_pSpriteTexture; // weak ref

};

#endif /* defined(__Cut_the_Bomb__BombBallSprite__) */
