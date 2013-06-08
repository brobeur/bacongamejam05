#ifndef __BGPANEL_SCENE_H__
#define __BGPANEL_SCENE_H__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "RSCCBMacros.h"




class BackgroundPanel : public cocos2d::CCSprite
{
 public:
   CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(BackgroundPanel, create);
   BackgroundPanel();
   virtual ~BackgroundPanel();
   static BackgroundPanel* createHack();
   //bool init();
   void toggle();
 private:
   bool m_bLightOn;
};

#endif
