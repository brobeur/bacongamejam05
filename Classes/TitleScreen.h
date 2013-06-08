#ifndef __TITLESCREEN_SCENE_H__
#define __TITLESCREEN_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "RSCCBMacros.h"

// TitleScreen Layer
class TitleScreen : public cocos2d::CCLayer {
public:
   TitleScreen();
   ~TitleScreen();
   bool init();
   static cocos2d::CCScene* scene();
   CREATE_FUNC(TitleScreen);
    
private:
};

RSCCB_DECLARE_LAYER_LOADER(TitleScreen)

#endif // __HELLOWORLD_SCENE_H__
