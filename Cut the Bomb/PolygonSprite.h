//
//  PolygonSprite.h
//  Cut the Bomb
//
//  Created by jordan Cortet on 10/07/13.
//
//

#ifndef __Cut_the_Bomb__PolygonSprite__
#define __Cut_the_Bomb__PolygonSprite__

#include <iostream>
#include "Box2D.h"
#include "PRFilledPolygon.h"
#include "GB2ShapeCache-x.h"

class PolygonSprite : public PRFilledPolygon
{
public:
    PolygonSprite();
    virtual ~PolygonSprite();
    
    static PolygonSprite* spriteWithFile(string filename, b2Body *body, bool original);
    static PolygonSprite* spriteWithTexture(cocos2d::CCTexture2D *texture, b2Body *body, bool original);
    static PolygonSprite* spriteWithWorld(b2World *world);
    
    bool initWithTexture(cocos2d::CCTexture2D *texture, b2Body *body, bool original);
    bool initWithFile(string filename, b2Body *body, bool original);
    virtual bool initWithWorld(b2World *world);
    
    b2Body* createBodyForWorld(b2World *world, b2Vec2 position, float rotation, float density, float friction, float restitution, string name);
    
    void activateCollisions();
    void deactivateCollisions();
    
    virtual void setPosition(cocos2d::CCPoint &position);
    virtual cocos2d::CCAffineTransform nodeToParentTransform(void);
    
    
    bool getSliceEntered();
    bool getSliceExited();
    b2Vec2 getEntryPoint();
    b2Vec2 getExitPoint();
    double getSliceEntryTime();
    
    void setSliceEntered(bool value);
    void setSliceExited(bool value);
    void setEntryPoint(b2Vec2 value);
    void setExitPoint(b2Vec2 value);
    void setSliceEntryTime(double value);
    
private:
    
    CC_SYNTHESIZE(b2Body *, _body, Body);
    CC_SYNTHESIZE(bool, _original, Original);
    CC_SYNTHESIZE(b2Vec2, _centriod, Centriod);
    
    bool sliceEntered;
    bool sliceExited;
    
    b2Vec2 entryPoint;
    b2Vec2 exitPoint;
    
    double sliceEntryTime;
};


#endif /* defined(__Cut_the_Bomb__PolygonSprite__) */
