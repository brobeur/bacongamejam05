//Runner.cpp

#include "rs_util.h"
#include "GameState.h"
#include "Runner.h"

USING_NS_CC;
USING_NS_CC_EXT;

Runner::Runner() :
   m_fSpeed(kInitialSpeed)
{

}

Runner::~Runner()
{

}
void Runner::resetSpeed()
{
   m_fSpeed = kInitialSpeed;
}
Runner* Runner::createHack()
{
   Runner* runner = Runner::create();

   // add a ccnode
   CCSprite* sprite = CCSprite::create("runner/00.png");
   runner->addChild(sprite);

   // There are other several ways of storing + adding frames, 
   // this is the most basic using one image per frame.
   CCAnimation * anim = CCAnimation::create();

   anim->addSpriteFrameWithFileName("runner/00.png");
   anim->addSpriteFrameWithFileName("runner/01.png");
   anim->addSpriteFrameWithFileName("runner/02.png");
   anim->addSpriteFrameWithFileName("runner/03.png");

   anim->setDelayPerUnit(.1);
   anim->setLoops(1);

   CCAnimate *theAnim = CCAnimate::create(anim); 
   sprite->runAction(CCRepeatForever::create(theAnim));

   CCSize s = CCDirector::sharedDirector()->getWinSize();
   runner->setScale(.3 * s.height / sprite->getContentSize().height);

   // use a shader program for grey
   CCGLProgram *pBWShaderProgram = new CCGLProgram();
   pBWShaderProgram->initWithVertexShaderFilename("Grey.vsh", "Grey.fsh");
   pBWShaderProgram->autorelease();
   pBWShaderProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
   pBWShaderProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
   pBWShaderProgram->link();
   pBWShaderProgram->updateUniforms();
   CCShaderCache::sharedShaderCache()->addProgram(pBWShaderProgram, "kGreyShaderProgram");

   sprite->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey("kGreyShaderProgram"));
   sprite->getShaderProgram()->use();
   /*
   GLint myUniformLocation = glGetUniformLocation(sprite->getShaderProgram()->getProgram, "greyness");
   glUniform1f(myUniformLocation, 0.);
*/
   // make height a function of window
   return runner;
}

void Runner::setHealth(float ratio)
{
   CCSprite* s = dynamic_cast<CCSprite*>(m_pChildren->objectAtIndex(0));
   s->setOpacity(SIMP_MAX(0., ratio) * 128 + 127);

   /*
   GLint myUniformLocation = glGetUniformLocation(sprite->getShaderProgram()->getProgram, "greyness");
   glUniform1f(myUniformLocation, 1. - SIMP_MAX(0., ratio));

   */
}

float Runner::getWidth()
{
   static float minWidth = 10000;
   CCSprite* s = dynamic_cast<CCSprite*>(m_pChildren->objectAtIndex(0));
   // use the smaller since the sprites are so varying in width
   minWidth = eval_min(minWidth,
                       s->getContentSize().width * s->getScaleX() * getScaleX());
   return minWidth;
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
