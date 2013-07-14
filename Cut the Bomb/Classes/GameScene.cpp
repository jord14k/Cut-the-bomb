//
//  HelloWorldScene.cpp
//  Cut the Bomb
//
//  Created by jordan Cortet on 10/07/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//
#include "GameScene.h"


#define MOTION_STREAK_TAG 1

using namespace cocos2d;
using namespace CocosDenshion;

int comparator(const void *a, const void *b) {
    const b2Vec2 *va = (const b2Vec2 *)a;
    const b2Vec2 *vb = (const b2Vec2 *)b;
    
    if (va->x > vb->x) {
        return 1;
    } else if (va->x < vb->x) {
        return -1;
    }
    return 0;
}
BombBallSprite *bomb2;
GameScene::GameScene()
{
    GB2ShapeCache::sharedGB2ShapeCache()->addShapesWithFile("spritesPhysics.plist");
    
    setTouchEnabled( true );
    setAccelerometerEnabled( true );

    CCSize screen = CCDirector::sharedDirector()->getWinSize();
    m_iCount = 0;
    
    // init physics
    this->initPhysics();
    
    BombBallSprite *bomb = BombBallSprite::spriteWithWorld(world);
    bomb->activateCollisions();
    
    this->addChild(bomb);
    
    scheduleUpdate();
    this->_raycastCallback = new RaycastCallback();
}

GameScene::~GameScene()
{
    delete world;
    world = NULL;
    
    delete m_debugDraw;
}

void GameScene::initPhysics()
{

    CCSize s = CCDirector::sharedDirector()->getWinSize();

    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    world = new b2World(gravity);

    // Do we want to let bodies sleep?
    world->SetAllowSleeping(true);

    world->SetContinuousPhysics(true);

     m_debugDraw = new GLESDebugDraw( PTM_RATIO );
     world->SetDebugDraw(m_debugDraw);

    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
            flags += b2Draw::e_jointBit;
            flags += b2Draw::e_aabbBit;
            flags += b2Draw::e_pairBit;
            flags += b2Draw::e_centerOfMassBit;
    m_debugDraw->SetFlags(flags);


    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world->CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2EdgeShape groundBox;

    // bottom

    groundBox.Set(b2Vec2(0,0), b2Vec2(s.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);

    // top
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);

    // left
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(0,0));
    groundBody->CreateFixture(&groundBox,0);

    // right
    groundBox.Set(b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);
}

void GameScene::draw()
{
    //
    // IMPORTANT:
    // This is only for debug purposes
    // It is recommend to disable it
    //
    CCLayer::draw();

    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

    kmGLPushMatrix();

    world->DrawDebugData();

    kmGLPopMatrix();
}

float getTimeTick() {
    timeval time;
    gettimeofday(&time, NULL);
    unsigned long secs = (time.tv_sec * 1000);
    return (float) secs;
}

void GameScene::checkAndSliceObjects()
{
    double curTime = getTimeTick();
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) {
            PolygonSprite *sprite = (PolygonSprite*)b->GetUserData();
            
            if (sprite->getSliceEntered() && curTime > sprite->getSliceEntryTime())
            {
                sprite->setSliceEntered(false);
            }
            else if (sprite->getSliceEntered() && sprite->getSliceExited())
            {
                this->splitPolygonSprite(sprite);
            }
        }
    }
}

void GameScene::update(float dt)
{
    //It is recommended that a fixed time step is used with Box2D for stability
    //of the simulation, however, we are using a variable time step here.
    //You need to make an informed choice, the following URL is useful
    //http://gafferongames.com/game-physics/fix-your-timestep/
    
    
    
    int velocityIterations = 8;
    int positionIterations = 1;
    
    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    world->Step(dt, velocityIterations, positionIterations);
    
    this->checkAndSliceObjects();
    //Iterate over the bodies in the physics world
//    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
//    {
//        if (b->GetUserData() != NULL) {
//            //Synchronize the AtlasSprites position and rotation with the corresponding body
//            CCSprite* myActor = (CCSprite*)b->GetUserData();
//            myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
//            myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
//        }    
//    }
    
    
}

void GameScene::onEnter() {
    CCNode::onEnter();
    this->setTouchEnabled(true);
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
}

void GameScene::resetMotionStreak() {
    this->removeChildByTag(MOTION_STREAK_TAG, true);
    CCMotionStreak* streak = CCMotionStreak::create(0.5f, 1, 10, ccc3(255, 255, 0), "line.png");
    this->addChild(streak, 5, MOTION_STREAK_TAG);
}

void GameScene::addMotionStreakPoint(cocos2d::CCPoint point) {
    CCMotionStreak* streak = (CCMotionStreak*)this->getChildByTag(MOTION_STREAK_TAG);
    streak->setPosition(point);
    
    if (++m_iCount>100) {
        int r = rand()%256;
        int b = rand()%265;
        int g = rand()%256;
        streak->setColor(ccc3(r, b, g));
        m_iCount = 0;
    }
}

bool GameScene::ccTouchBegan(cocos2d::CCTouch *ptouch, cocos2d::CCEvent *pEvent) {
    
    this->resetMotionStreak();
    CCPoint point = this->convertTouchToNodeSpace(ptouch);
    this->addMotionStreakPoint(point);

    this->_startPoint = point;
    this->_endPoint = point;
    
    return true;
}

void GameScene::ccTouchMoved(cocos2d::CCTouch *ptouch, cocos2d::CCEvent *pEvent) {
    
    CCPoint point = this->convertTouchToNodeSpace(ptouch);
    this->addMotionStreakPoint(point);
    
    this->_endPoint = point;
    
    if (ccpLengthSQ(ccpSub(_startPoint, _endPoint)) > 25)
    {
        world->RayCast(_raycastCallback,
                       b2Vec2(_startPoint.x / PTM_RATIO, _startPoint.y / PTM_RATIO),
                       b2Vec2(_endPoint.x / PTM_RATIO, _endPoint.y / PTM_RATIO));
        
        world->RayCast(_raycastCallback,
                       b2Vec2(_endPoint.x / PTM_RATIO, _endPoint.y / PTM_RATIO),
                       b2Vec2(_startPoint.x / PTM_RATIO, _startPoint.y / PTM_RATIO));
        _startPoint = _endPoint;
    }
    
}

void GameScene::ccTouchEnded(cocos2d::CCTouch *ptouch, cocos2d::CCEvent *pEvent) {
    
    this->clearSlices();
}

void GameScene::ccTouchCancelled(cocos2d::CCTouch *ptouch, cocos2d::CCEvent *pEvent) {
    
     CCPoint point = this->convertTouchToNodeSpace(ptouch);
    this->_endPoint = point;
}


CCScene* GameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    CCLayer* layer = new GameScene();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}

b2Vec2* GameScene::arrangeVertices(b2Vec2* vertices, int count) {
    float determinant;
    int iCounterClockWise = 1;
    int iClockWise = count - 1;
    int i;
    
    b2Vec2 referencePointA,referencePointB;
    b2Vec2 *sortedVertices = (b2Vec2*)calloc(count, sizeof(b2Vec2));
    
    //sort all vertices in ascending order according to their x-coordinate so you can get two points of a line
    qsort(vertices, count, sizeof(b2Vec2), comparator);
    
    sortedVertices[0] = vertices[0];
    referencePointA = vertices[0];          //leftmost point
    referencePointB = vertices[count-1];    //rightmost point
    
    //you arrange the points by filling our vertices in both clockwise and counter-clockwise directions using the determinant function
    for (i=1;i<count-1;i++)
    {
        determinant = calculate_determinant_2x3(referencePointA.x, referencePointA.y, referencePointB.x, referencePointB.y, vertices[i].x, vertices[i].y);
        if (determinant<0)
        {
            sortedVertices[iCounterClockWise++] = vertices[i];
        }
        else
        {
            sortedVertices[iClockWise--] = vertices[i];
        }//endif
    }//endif
    
    sortedVertices[iCounterClockWise] = vertices[count-1];
    return sortedVertices;
}

void GameScene::clearSlices ()
{
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) {
            PolygonSprite *sprite = (PolygonSprite*)b->GetUserData();
            sprite->setSliceEntered(false);
            sprite->setSliceExited(false);
        }
    }
}

void GameScene::splitPolygonSprite(PolygonSprite* sprite) {
    
    //declare & initialize variables to be used for later
    PolygonSprite *newSprite1, *newSprite2;
    
    //our original shape's attributes
    b2Fixture *originalFixture = sprite->getBody()->GetFixtureList();
    b2PolygonShape *originalPolygon = (b2PolygonShape*)originalFixture->GetShape();
    int vertexCount = originalPolygon->GetVertexCount();
    
    //our determinant(to be described later) and iterator
    float determinant;
    int i;
    
    //you store the vertices of our two new sprites here
    b2Vec2 *sprite1Vertices = (b2Vec2*)calloc(24, sizeof(b2Vec2));
    b2Vec2 *sprite2Vertices = (b2Vec2*)calloc(24, sizeof(b2Vec2));
    b2Vec2 *sprite1VerticesSorted, *sprite2VerticesSorted;
    
    //you store how many vertices there are for each of the two new sprites here
    int sprite1VertexCount = 0;
    int sprite2VertexCount = 0;
    
    //step 1:
    //the entry and exit point of our cut are considered vertices of our two new shapes, so you add these before anything else
    sprite1Vertices[sprite1VertexCount++] = sprite->getEntryPoint();
    sprite1Vertices[sprite1VertexCount++] = sprite->getExitPoint();
    sprite2Vertices[sprite2VertexCount++] = sprite->getEntryPoint();
    sprite2Vertices[sprite2VertexCount++] = sprite->getExitPoint();
    
    //step 2:
    //iterate through all the vertices and add them to each sprite's shape
    for (i=0; i<vertexCount; i++)
    {
        //get our vertex from the polygon
        b2Vec2 point = originalPolygon->GetVertex(i);
        
        //you check if our point is not the same as our entry or exit point first
        b2Vec2 diffFromEntryPoint = point - sprite->getEntryPoint();
        b2Vec2 diffFromExitPoint = point - sprite->getExitPoint();
        
        if ((diffFromEntryPoint.x == 0 && diffFromEntryPoint.y == 0) || (diffFromExitPoint.x == 0 && diffFromExitPoint.y == 0))
        {
        }
        else
        {
            determinant = calculate_determinant_2x3(sprite->getEntryPoint().x, sprite->getEntryPoint().y, sprite->getExitPoint().x, sprite->getExitPoint().y, point.x, point.y);
            
            if (determinant > 0)
            {
                //if the determinant is positive, then the three points are in clockwise order
                sprite1Vertices[sprite1VertexCount++] = point;
            }
            else
            {
                //if the determinant is 0, the points are on the same line. if the determinant is negative, then they are in counter-clockwise order
                sprite2Vertices[sprite2VertexCount++] = point;
                
            }//endif
        }//endif
    }//endfor
    
    //step 3:
    //Box2D needs vertices to be arranged in counter-clockwise order so you reorder our points using a custom function
    sprite1VerticesSorted = this->arrangeVertices(sprite1Vertices, sprite1VertexCount);
    sprite2VerticesSorted = this->arrangeVertices(sprite2Vertices, sprite2VertexCount);
    
    //step 4:
    //Box2D has some restrictions with defining shapes, so you have to consider these. You only cut the shape if both shapes pass certain requirements from our function
    bool sprite1VerticesAcceptable = this->areVerticesAcceptable(sprite1VerticesSorted, sprite1VertexCount);
    bool sprite2VerticesAcceptable = this->areVerticesAcceptable(sprite2VerticesSorted, sprite2VertexCount);
    
    //step 5:
    //you destroy the old shape and create the new shapes and sprites
    if (sprite1VerticesAcceptable && sprite2VerticesAcceptable)
    {
        //create the first sprite's body
//        b2Body *body1 = [self createBodyWithPosition:sprite.body->GetPosition() rotation:sprite.body->GetAngle() vertices:sprite1VerticesSorted vertexCount:sprite1VertexCount density:originalFixture->GetDensity() friction:originalFixture->GetFriction() restitution:originalFixture->GetRestitution()];
        
        b2Body *body1 = this->createBodyWithPosition(sprite->getBody()->GetPosition(),
                                                     sprite->getBody()->GetAngle(),
                                                     sprite1VerticesSorted,
                                                     sprite1VertexCount,
                                                     originalFixture->GetDensity(),
                                                     originalFixture->GetFriction(),
                                                     originalFixture->GetRestitution());
        //create the first sprite
        
        newSprite1 = BombBallSprite::spriteWithTexture(sprite->getTexture(), body1, false);
        
        //create the second sprite's body
//        b2Body *body2 = [self createBodyWithPosition:sprite.body->GetPosition() rotation:sprite.body->GetAngle() vertices:sprite2VerticesSorted vertexCount:sprite2VertexCount density:originalFixture->GetDensity() friction:originalFixture->GetFriction() restitution:originalFixture->GetRestitution()];
        
        b2Body *body2 = this->createBodyWithPosition(sprite->getBody()->GetPosition(),
                                                     sprite->getBody()->GetAngle(),
                                                     sprite2VerticesSorted,
                                                     sprite2VertexCount,
                                                     originalFixture->GetDensity(),
                                                     originalFixture->GetFriction(),
                                                     originalFixture->GetRestitution());
        
        //create the second sprite
        newSprite2 = BombBallSprite::spriteWithTexture(sprite->getTexture(), body2, false);
        this->addChild(newSprite2);
        
        bomb2 = BombBallSprite::spriteWithWorld(world);
        
        bomb2->activateCollisions();
        CCPoint posi = CCPointMake(20/PTM_RATIO, 20/PTM_RATIO);
        bomb2->setPosition(posi);
        
        this->addChild(bomb2);
        //you don't need the old shape & sprite anymore so you either destroy it or squirrel it away
        if (sprite->getOriginal())
        {
            CCPoint po = CCPoint(256, -256);
            sprite->deactivateCollisions();
            sprite->setPosition(po);    //cast them faraway
            sprite->setSliceEntered(false);
            sprite->setSliceExited(false);
            sprite->getEntryPoint().SetZero();
            sprite->getExitPoint().SetZero();
        }
        else
        {
            world->DestroyBody(sprite->getBody());
            this->removeChild(sprite, true);
        }
    }
    else
    {
        sprite->setSliceEntered(false);
        sprite->setSliceExited(false);
    }
    
    //free up our allocated vectors
    free(sprite1VerticesSorted);
    free(sprite2VerticesSorted);
    free(sprite1Vertices);
    free(sprite2Vertices);
}

bool GameScene::areVerticesAcceptable(b2Vec2* vertices, int count) {
    return true;
}

b2Body* GameScene::createBodyWithPosition(b2Vec2 position, float rotation, b2Vec2* vertices, int32 count, float density, float friction, float restitution) {
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = position;
    bodyDef.angle = rotation;
    b2Body *body = world->CreateBody(&bodyDef);
    
    b2FixtureDef fixtureDef;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;
    
    b2PolygonShape shape;
    shape.Set(vertices, count);
    fixtureDef.shape = &shape;
    body->CreateFixture(&fixtureDef);
    
    return body;
}
