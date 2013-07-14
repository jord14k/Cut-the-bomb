//
//  HelloWorldScene.h
//  Cut the Bomb
//
//  Created by jordan Cortet on 10/07/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//
#ifndef __Cut_the_Bomb__GameScene__
#define __Cut_the_Bomb__GameScene__

#define calculate_determinant_2x2(x1,y1,x2,y2) x1*y2-y1*x2
#define calculate_determinant_2x3(x1,y1,x2,y2,x3,y3) x1*y2+x2*y3+x3*y1-y1*x2-y2*x3-y3*x1

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "Box2D.h"
#include "GLES-Render.h"
#include "PolygonSprite.h"
#include "BombBallSprite.h"
#include "SimpleAudioEngine.h"
#include "GB2ShapeCache-x.h"
#include "RayCastCallback.h"

class GameScene : public cocos2d::CCLayer {
public:
    ~GameScene();
    GameScene();
    
    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();
    
    void initPhysics();

    virtual void draw();
    void update(float dt);
    
    void onEnter();
    bool ccTouchBegan(cocos2d::CCTouch *ptouch, cocos2d::CCEvent *pEvent);
    void ccTouchMoved(cocos2d::CCTouch *ptouch, cocos2d::CCEvent *pEvent);
    void ccTouchEnded(cocos2d::CCTouch *ptouch, cocos2d::CCEvent *pEvent);
    void ccTouchCancelled(cocos2d::CCTouch *ptouch, cocos2d::CCEvent *pEvent);
    void clearSlices ();
    void checkAndSliceObjects();
    
    b2Vec2* arrangeVertices(b2Vec2* vertices, int count);
    void splitPolygonSprite(PolygonSprite* sprite);
    bool areVerticesAcceptable(b2Vec2* vertices, int count);
    b2Body* createBodyWithPosition(b2Vec2 position, float rotation, b2Vec2* vertices, int32 count, float density, float friction, float restitution);
    
private:
    
    int m_iCount;
    void resetMotionStreak();
    void addMotionStreakPoint(cocos2d::CCPoint point);
    
    CCPoint _startPoint;
    CCPoint _endPoint;
    
    b2World* world;
    GLESDebugDraw *m_debugDraw;
    RaycastCallback *_raycastCallback;
};

#endif // __HELLO_WORLD_H__
