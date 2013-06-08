#ifndef __BGWALLTRAP_SCENE_H__
#define __BGWALLTRAP_SCENE_H__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "RSCCBMacros.h"




class WallTrap : public cocos2d::CCSprite
{
 public:
   CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(WallTrap, create);
   WallTrap();
   virtual ~WallTrap();
   static WallTrap* createHack(bool light);
   //bool init();
   void toggleOn(bool on);
   bool isDangerous();
   void setWidth(float width);
   float getWidth(){return m_fWidth;}
 protected:
   bool m_bLightOn;
   bool m_bLight;

   float m_fWidth;
};

#endif
