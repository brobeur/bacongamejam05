

#ifndef __TITLESCREEN_SCENE_H__
#define __TITLESCREEN_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "RSCCBMacros.h"

// TitleScreen Layer
class TitleScreen : public cocos2d::CCLayer,
                    public cocos2d::extension::CCBSelectorResolver
{
public:
   TitleScreen();
   ~TitleScreen();
   bool init();
   static cocos2d::CCScene* scene();
   CREATE_FUNC(TitleScreen);

   virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
   virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char * pSelectorName);

   void onStart(CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
   void onQuit(CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
   void onResetScore(CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    
private:
};

RSCCB_DECLARE_LAYER_LOADER(TitleScreen)

#endif // __HELLOWORLD_SCENE_H__
