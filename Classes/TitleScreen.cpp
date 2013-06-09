//
//  TitleScreenScene.cpp
//  cocoschipmunktest
//
//  Created by Michael Chinen on 13/06/06.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#include "RSCCBMacros.h"

#include "TitleScreen.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocos2d;
using namespace CocosDenshion;

TitleScreen::TitleScreen()
{
}

TitleScreen::~TitleScreen()
{
}

SEL_MenuHandler TitleScreen::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName) {
   return NULL;    
}


SEL_CCControlHandler TitleScreen::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName) {
   CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onStart", TitleScreen::onStart);
    
   return NULL;
}

void TitleScreen::onStart(CCObject * pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
   // load hello world scene
   CCScene *pScene = HelloWorld::scene();

   CCDirector::sharedDirector()->replaceScene(pScene);
}


CCScene* TitleScreen::scene()
{
   // 'scene' is an autorelease object.
   CCScene *scene = CCScene::create();

   // 'layer' is an autorelease object.
   TitleScreen *layer = TitleScreenLoader::loadFromFile();//TitleScreen::create();

   // add layer as a child to scene
   scene->addChild(layer);

   CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("titlescreenloop.mp3", true);


   // return the scene
   return scene;
}

bool TitleScreen::init()
{
   if (!CCLayer::init())
      {
         return false;
      }


   return true;
}


RSCCB_DEFINE_BASIC_LOADER(TitleScreen, title_screen);
