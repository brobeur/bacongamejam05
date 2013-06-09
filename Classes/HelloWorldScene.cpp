//
//  HelloWorldScene.cpp
//  cocoschipmunktest
//
//  Created by Michael Chinen on 13/06/06.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#include "RSCCBMacros.h"

#include "GameState.h"
#include "TitleScreen.h"
#include "Runner.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

enum {
   kTagParentNode = 1,
};

// callback to remove Shapes from the Space
void removeShape( cpBody *body, cpShape *shape, void *data )
{
   cpShapeFree( shape );
}

ChipmunkPhysicsSprite::ChipmunkPhysicsSprite()
   : m_pBody(NULL)
{

}

ChipmunkPhysicsSprite::~ChipmunkPhysicsSprite()
{
   cpBodyEachShape(m_pBody, removeShape, NULL);
   cpBodyFree( m_pBody );
}

void ChipmunkPhysicsSprite::setPhysicsBody(cpBody * body)
{
   m_pBody = body;
}

// this method will only get called if the sprite is batched.
// return YES if the physics values (angles, position ) changed
// If you return NO, then nodeToParentTransform won't be called.
bool ChipmunkPhysicsSprite::isDirty(void)
{
   return true;
}

CCAffineTransform ChipmunkPhysicsSprite::nodeToParentTransform(void)
{
   float x = m_pBody->p.x;
   float y = m_pBody->p.y;

   if ( isIgnoreAnchorPointForPosition() ) {
      x += m_obAnchorPointInPoints.x;
      y += m_obAnchorPointInPoints.y;
   }

   // Make matrix
   float c = m_pBody->rot.x;
   float s = m_pBody->rot.y;

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

HelloWorld::HelloWorld()
{
}

HelloWorld::~HelloWorld()
{
   // manually Free rogue shapes
   for( int i=0;i<4;i++) {
      cpShapeFree( m_pWalls[i] );
   }
    
   cpSpaceFree( m_pSpace );
}

CCScene* HelloWorld::scene()
{
   // 'scene' is an autorelease object.
   CCScene *scene = CCScene::create();

   // 'layer' is an autorelease object.
   HelloWorld *layer = HelloWorld::createHack();
   //HelloWorld *layer = HelloWorldLoader::loadFromFile();//HelloWorld::create();
   // add layer as a child to scene
   scene->addChild(layer);


   layer->extraSetup();
   // only valid after extraSetup
   scene->addChild(layer->m_pTopLayer);

   // return the scene
   return scene;
}

/*
bool HelloWorld::init()
{
   if (!CCLayer::init())
      {
         return false;
      }

   // enable events
   setAccelerometerEnabled(true);

   CCSize s = CCDirector::sharedDirector()->getWinSize();

   // title
   CCLabelTTF *label = CCLabelTTF::create("Multi touch the screen", "Marker Felt", 36);
   label->setPosition(ccp( s.width / 2, s.height - 30));
   this->addChild(label, -1);

   // init physics
   initPhysics();

#if 1
   // Use batch node. Faster
   CCSpriteBatchNode *parent = CCSpriteBatchNode::create("grossini_dance_atlas.png", 100);
   m_pSpriteTexture = parent->getTexture();
#else
   // doesn't use batch node. Slower
   m_pSpriteTexture = CCTextureCache::sharedTextureCache()->addImage("grossini_dance_atlas.png");
   CCNode *parent = CCNode::node();
#endif
   addChild(parent, 0, kTagParentNode);

   addNewSpriteAtPosition(ccp(200,200));

   //setKeyboardEnabled(true);

   
   m_pLabel = CCLabelTTF::create("Please press any key: ", "Arial", 22);
   m_pLabel->setPosition(ccp(s.width / 2, s.height / 2));
   addChild(m_pLabel, 0);
    
   m_pLabel->retain();
   


   return true;
}*/

void HelloWorld::keyPressed(int keyCode)
{
   CCLog("Key with keycode %d pressed", keyCode);

   // if ascii a/z print it
   if (keyCode >= 'A' && keyCode <= 'z') {
      //char buf[128];
      //snprintf(buf, 127, "%s%c", m_pLabel->getString(), (char)keyCode);
      //m_pLabel->setString(buf);
   }

}

void  HelloWorld::keyReleased(int keyCode)
{
   CCLog("Key with keycode %d released", keyCode);
}

HelloWorld* HelloWorld::createHack()
{
   HelloWorld* h = HelloWorld::create();
   return h;
}

void HelloWorld::initPhysics()
{
   CCSize s = CCDirector::sharedDirector()->getWinSize();

   // init chipmunk
   cpInitChipmunk();

   m_pSpace = cpSpaceNew();

   m_pSpace->gravity = cpv(0, -100);

   //
   // rogue shapes
   // We have to free them manually
   //
   // bottom
   m_pWalls[0] = cpSegmentShapeNew( m_pSpace->staticBody, cpv(0,0), cpv(s.width,0), 0.0f);

   // top
   m_pWalls[1] = cpSegmentShapeNew( m_pSpace->staticBody, cpv(0,s.height), cpv(s.width,s.height), 0.0f);

   // left
   m_pWalls[2] = cpSegmentShapeNew( m_pSpace->staticBody, cpv(0,0), cpv(0,s.height), 0.0f);

   // right
   m_pWalls[3] = cpSegmentShapeNew( m_pSpace->staticBody, cpv(s.width,0), cpv(s.width,s.height), 0.0f);

   for( int i=0;i<4;i++) {
      m_pWalls[i]->e = 1.0f;
      m_pWalls[i]->u = 1.0f;
      cpSpaceAddStaticShape(m_pSpace, m_pWalls[i] );
   }
}


// 
void HelloWorld::extraSetup()
{
   setTouchEnabled(true);
   
   initPhysics();
   CCSize s = CCDirector::sharedDirector()->getWinSize();

   
   Runner* runner = Runner::createHack();//
   //Runner* runner = RunnerLoader::loadFromFile();
   if (runner) {
   runner->setPosition(ccp(s.width * .16, s.height * .4));
   addChild(runner, 0);
   scheduleUpdate();

   STATE->setLayer(this);
   STATE->setRunner(runner);


   //cocos2d::CCTexture2D* m_pSpriteTexture; // weak ref
   // weak ref
   m_pTopLayer = CCLayer::create();
   m_pTopLayer->setPosition(getPosition());
   m_pTopLayer->setContentSize(getContentSize());
   STATE->setTopLayer(m_pTopLayer);

   CCSprite* heart = CCSprite::create("heart.png");
   heart->setScale(.05 * s.height / heart->getContentSize().height);
   heart->setPosition(ccp(s.width * .14, s.height * .84));
   m_pTopLayer->addChild(heart);


   m_pHealthLabel = CCLabelTTF::create("HP", "fonts/Minecraftia.ttf", 20);
   m_pHealthLabel->setPosition(ccp(heart->getScaleX() * heart->getContentSize().width + s.width * .14, s.height * .85));
   m_pHealthLabel->setColor(ccc3(255, 255, 255));
   m_pHealthLabel->setTag(51);
   m_pTopLayer->addChild(m_pHealthLabel);

   m_pDistLabel = CCLabelTTF::create("HP", "fonts/Minecraftia.ttf", 20);
   m_pDistLabel->setPosition(ccp(s.width * .2, s.height * .8));
   m_pDistLabel->setColor(ccc3(255, 255, 255));
   m_pDistLabel->setTag(50);
   m_pTopLayer->addChild(m_pDistLabel);
   


   CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("BGJ-done.mp3", true);

   } else {
   CCLog("RUNNELOADFAIL");

   }

}

void HelloWorld::updateUI(int health, int dist, bool visible)
{
   m_pHealthLabel->setVisible(visible);
   m_pDistLabel->setVisible(visible);

   if (visible) {
      //cache cause i don't think they do
      static int lastHealth = -1;
      char buf[128] = {0};
      if (lastHealth != health) {
         snprintf(buf, 127, "%i", health);
         m_pHealthLabel->setString(buf);
      }
      snprintf(buf, 127, "%i PIXELS", dist);
      m_pDistLabel->setString(buf);
   }
}



void HelloWorld::update(float delta)
{
   // Should use a fixed size step based on the animation interval.
   int steps = 2;
   float dt = CCDirector::sharedDirector()->getAnimationInterval()/(float)steps;

   for(int i=0; i<steps; i++){
      cpSpaceStep(m_pSpace, dt);
   }

   STATE->update(delta);
}

void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
   //Add a new body/atlas sprite at the touched location
   CCSetIterator it;
   CCTouch* touch;

   for( it = touches->begin(); it != touches->end(); it++) 
      {
         touch = (CCTouch*)(*it);

         if(!touch)
            break;

         CCPoint location = touch->getLocationInView();

         location = CCDirector::sharedDirector()->convertToGL(location);

         //addNewSpriteAtPosition( location );
         if (STATE->isDead()) {
            if (STATE->canRestart()) {
               STATE->restartGame();
               // just quit
               CCScene *pScene = TitleScreen::scene();

               CCDirector::sharedDirector()->replaceScene(pScene);
            }
         } else
            STATE->toggleLight();
         
      }
}

void HelloWorld::didAccelerate(CCAcceleration* pAccelerationValue)
{
   static float prevX=0, prevY=0;

#define kFilterFactor 0.05f

   float accelX = (float) pAccelerationValue->x * kFilterFactor + (1- kFilterFactor)*prevX;
   float accelY = (float) pAccelerationValue->y * kFilterFactor + (1- kFilterFactor)*prevY;

   prevX = accelX;
   prevY = accelY;

   CCPoint v = ccp( accelX, accelY);
   v = ccpMult(v, 200);
   m_pSpace->gravity = cpv(v.x, v.y);
}


RSCCB_DEFINE_BASIC_LOADER(HelloWorld, second_screen);
