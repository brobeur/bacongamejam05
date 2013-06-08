//
//  TitleScreenScene.cpp
//  cocoschipmunktest
//
//  Created by Michael Chinen on 13/06/06.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#include "RSCCBMacros.h"

#include "TitleScreen.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

TitleScreen::TitleScreen()
{
}

TitleScreen::~TitleScreen()
{
}

CCScene* TitleScreen::scene()
{
   // 'scene' is an autorelease object.
   CCScene *scene = CCScene::create();

   // 'layer' is an autorelease object.
   TitleScreen *layer = TitleScreenLoader::loadFromFile();//TitleScreen::create();

   // add layer as a child to scene
   scene->addChild(layer);


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
