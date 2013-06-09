#ifndef __PICKUP_SCENE_H__
#define __PICKUP_SCENE_H__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "RSCCBMacros.h"

class Pickup : public cocos2d::CCSprite
{
 public:
   CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(Pickup, create);
   Pickup();
   virtual ~Pickup();
   static Pickup* createHack(bool light);
   //bool init();
   void toggleOn(bool on);
   bool isDangerous();
   void update(float dt);
   void draw();
 protected:
   bool m_bLightOn;
   bool m_bLight;

   float m_fSecsExisted;
};

#endif
