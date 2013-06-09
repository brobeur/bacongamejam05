//Runner.h

#ifndef __RUNNER_SCENE_H__
#define __RUNNER_SCENE_H__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "RSCCBMacros.h"


class Runner : public cocos2d::CCNode
{
 public:
   CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(Runner, create);

   Runner();
   virtual ~Runner();
   static Runner* createHack();
   float getSpeed(){return m_fSpeed;}
   void addSpeed(float s){m_fSpeed += s;}
   void resetSpeed();
   float getWidth();
   float getHeight();

   void draw();

   void setHealth(float ratio);//1. best. 0. dead
   //bool init();
 private:
   float m_fSpeed;
};

RSCCB_DECLARE_BASIC_LOADER(Runner);

#endif
