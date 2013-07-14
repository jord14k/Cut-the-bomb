//
//  RayCastCallback.h
//  Cut the Bomb
//
//  Created by jordan Cortet on 13/07/13.
//
//

#ifndef Cut_the_Bomb_RayCastCallback_h
#define Cut_the_Bomb_RayCastCallback_h

#import "Box2D.h"
#import "PolygonSprite.h"

class RaycastCallback : public b2RayCastCallback
{
public:
    RaycastCallback(){
    }
    
    float32 ReportFixture(b2Fixture *fixture,const b2Vec2 &point,const b2Vec2 &normal,float32 fraction)
    {
        PolygonSprite *ps = (PolygonSprite*)fixture->GetBody()->GetUserData();
        
        if (!ps->getSliceEntered())
        {
            ps->setSliceEntered(true);
            
            //you need to get the point coordinates within the shape
            ps->setEntryPoint(ps->getBody()->GetLocalPoint(point));
            
            ps->setSliceEntryTime(getTimeTick() + 1);
            CCLOG("Slice Entered at world coordinates:(%f,%f), polygon coordinates:(%f,%f)", point.x*PTM_RATIO, point.y*PTM_RATIO, ps->getEntryPoint().x*PTM_RATIO, ps->getEntryPoint().y*PTM_RATIO);
        }
        else if (!ps->getSliceExited())
        {
            ps->setExitPoint(ps->getBody()->GetLocalPoint(point));
            ps->setSliceExited(true);
            
            CCLOG("Slice Exited at world coordinates:(%f,%f), polygon coordinates:(%f,%f)", point.x*PTM_RATIO, point.y*PTM_RATIO, ps->getExitPoint().x*PTM_RATIO, ps->getExitPoint().y*PTM_RATIO);
        }
        return 1;
    }
    
    float getTimeTick() {
        timeval time;
        gettimeofday(&time, NULL);
        unsigned long secs = (time.tv_sec * 1000);
        return (float) secs;
    }
    
};

#endif
