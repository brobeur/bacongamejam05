//Runner.cpp

#include "Runner.h"

USING_NS_CC;
USING_NS_CC_EXT;

Runner::Runner() :
   m_fSpeed(200.)
{

}

Runner::~Runner()
{

}

Runner* Runner::createHack()
{
   Runner* runner = Runner::create();

   // add a ccnode
   CCSprite* sprite = CCSprite::create("lightsout-character.png");
   runner->addChild(sprite);
   CCSize s = CCDirector::sharedDirector()->getWinSize();
   runner->setScale(.3 * s.height / sprite->getContentSize().height);

   // make height a function of window

   return runner;
}

float Runner::getWidth()
{
   CCSprite* s = dynamic_cast<CCSprite*>(m_pChildren->objectAtIndex(0));
   return s->getContentSize().width * s->getScaleX() * getScaleX();
}

float Runner::getHeight()
{
   CCSprite* s = dynamic_cast<CCSprite*>(m_pChildren->objectAtIndex(0));
   return s->getContentSize().height * s->getScaleY() * getScaleY();
}


/*
bool Runner::init()
{
   if (!CCNode::init()) {
      return false;
   }
   return true;
}

*/


RSCCB_DEFINE_BASIC_LOADER(Runner, runner);
