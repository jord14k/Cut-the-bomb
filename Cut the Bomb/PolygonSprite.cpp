//
//  PolygonSprite.cpp
//  Cut the Bomb
//
//  Created by jordan Cortet on 10/07/13.
//
//

#include "PolygonSprite.h"

#define PTM_RATIO 32

PolygonSprite::PolygonSprite()
{
    
}

PolygonSprite::~PolygonSprite()
{
    
}

PolygonSprite* PolygonSprite::spriteWithFile(string filename, b2Body *body, bool original)
{
    PolygonSprite *pobPolygonSprite = new PolygonSprite();
    if (pobPolygonSprite && pobPolygonSprite->initWithFile(filename, body, original))
    {
        pobPolygonSprite->autorelease();
        return pobPolygonSprite;
    }
    CC_SAFE_DELETE(pobPolygonSprite);
    return NULL;
}

PolygonSprite* PolygonSprite::spriteWithTexture(CCTexture2D *texture, b2Body *body, bool original)
{
    PolygonSprite *pobPolygonSprite = new PolygonSprite();
    if (pobPolygonSprite && pobPolygonSprite->initWithTexture(texture, body, original))
    {
        pobPolygonSprite->autorelease();
        return pobPolygonSprite;
    }
    CC_SAFE_DELETE(pobPolygonSprite);
    return NULL;
}

PolygonSprite* PolygonSprite::spriteWithWorld(b2World *world)
{
    PolygonSprite *pobPolygonSprite = new PolygonSprite();
    if (pobPolygonSprite && pobPolygonSprite->initWithWorld(world))
    {
        pobPolygonSprite->autorelease();
        return pobPolygonSprite;
    }
    CC_SAFE_DELETE(pobPolygonSprite);
    return NULL;
}

bool PolygonSprite::initWithFile(string filename, b2Body *body, bool original)
{
    CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(filename.c_str());
    return initWithTexture(texture, body, original);
}

bool PolygonSprite::initWithTexture(cocos2d::CCTexture2D *texture, b2Body *body, bool original)
{
    b2Fixture *originalFixture = body->GetFixtureList();
    b2PolygonShape *shape = (b2PolygonShape*)originalFixture->GetShape();
    int vertextCount = shape->GetVertexCount();
    vector<Vector2d> points;
    
    for (int i = 0; i < vertextCount; i++)
    {
        Vector2d p = Vector2d(shape->GetVertex(i).x * PTM_RATIO, shape->GetVertex(i).y * PTM_RATIO);
        points.push_back(p);
    }
    PRFilledPolygon::initWithPointsAndTexture(points, texture);
    _body = body;
    _body->SetUserData(this);
    _original = original;
    _centriod = _body->GetLocalCenter();
    this->setAnchorPoint(ccp(_centriod.x * PTM_RATIO / texture->getContentSize().width,
                             _centriod.y * PTM_RATIO / texture->getContentSize().height));
    return true;
}

bool PolygonSprite::initWithWorld(b2World *world)
{
    return true;
}

void PolygonSprite::setPosition(cocos2d::CCPoint &position)
{
    CCNode::setPosition(position);
    _body->SetTransform(b2Vec2(position.x/PTM_RATIO, position.y*PTM_RATIO), _body->GetAngle());
}

b2Body* PolygonSprite::createBodyForWorld(b2World *world, b2Vec2 position, float rotation, float density, float friction, float restitution, string name)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = position;
    bodyDef.angle = rotation;
    b2Body *body = world->CreateBody(&bodyDef);
    
    b2FixtureDef fixtureDef;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;
    fixtureDef.filter.categoryBits = 0;
    fixtureDef.filter.maskBits = 0;
    
    GB2ShapeCache *sc = GB2ShapeCache::sharedGB2ShapeCache();
    sc->addFixturesToBody(body, name);
    this->setAnchorPoint(sc->anchorPointForShape(name));
    
//    b2PolygonShape shape;
//    shape.Set(vertices, vertexCount);
//    fixtureDef.shape = &shape;
//    body->CreateFixture(&fixtureDef);
    
    return body;
}

void PolygonSprite::activateCollisions()
{
    b2Fixture *fixture = _body->GetFixtureList();
    b2Filter filter = fixture->GetFilterData();
    filter.categoryBits = 0x0001;
    filter.maskBits = 0x0001;
    fixture->SetFilterData(filter);
}

void PolygonSprite::deactivateCollisions()
{
    b2Fixture *fixture = _body->GetFixtureList();
    b2Filter filter = fixture->GetFilterData();
    filter.categoryBits = 0;
    filter.maskBits = 0;
    fixture->SetFilterData(filter);
}

CCAffineTransform PolygonSprite::nodeToParentTransform(void)
{
    b2Vec2 pos  = _body->GetPosition();
    
    float x = pos.x * PTM_RATIO;
    float y = pos.y * PTM_RATIO;
    
    if ( isIgnoreAnchorPointForPosition() ) {
        x += m_obAnchorPointInPoints.x;
        y += m_obAnchorPointInPoints.y;
    }
    
    // Make matrix
    float radians = _body->GetAngle();
    float c = cosf(radians);
    float s = sinf(radians);
    
    if( ! m_obAnchorPointInPoints.equals(CCPointZero) ){
        x += c*-m_obAnchorPointInPoints.x + -s*-m_obAnchorPointInPoints.y;
        y += s*-m_obAnchorPointInPoints.x + c*-m_obAnchorPointInPoints.y;
    }
    
    // Rot, Translate Matrix
    m_sTransform = CCAffineTransformMake( c,  s,
                                         -s,    c,
                                         x,    y );
    
    return m_sTransform;
}



bool PolygonSprite::getSliceEntered() { return this->sliceEntered; }
bool PolygonSprite::getSliceExited() { return this->sliceExited; }
b2Vec2 PolygonSprite::getEntryPoint() { return entryPoint; }
b2Vec2 PolygonSprite::getExitPoint() { return exitPoint; }
double PolygonSprite::getSliceEntryTime() { return sliceEntryTime; }

void PolygonSprite::setSliceEntered(bool value) { this->sliceEntered = value; }
void PolygonSprite::setSliceExited(bool value) { this->sliceExited = value; }
void PolygonSprite::setEntryPoint(b2Vec2 value) { this->entryPoint = value; }
void PolygonSprite::setExitPoint(b2Vec2 value) { this->exitPoint = value; }
void PolygonSprite::setSliceEntryTime(double value) { this->sliceEntryTime = value; }
