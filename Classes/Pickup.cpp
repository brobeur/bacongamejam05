#include "GameState.h"
#include "Pickup.h"

USING_NS_CC;
USING_NS_CC_EXT;

Pickup::Pickup() :
   m_bLight(false),
   m_fSecsExisted(0.)
{

}
 
Pickup::~Pickup()
{

}

#define kPickupTex ("bacon1.png")
static CCGLProgram *pBWShaderProgram;
static GLint myUniformLocation;
static GLint alphaUniformLocation;

Pickup* Pickup::createHack(bool light)
{
   Pickup* w = Pickup::create();
   if (w && w->initWithFile(kPickupTex)) {
      // set the scale to match the size in height
      CCSize s = CCDirector::sharedDirector()->getWinSize();
      w->setScale(.2 * s.height / w->getContentSize().height);
      w->m_bLight = light;
      w->toggleOn(STATE->isLightOn());

      if (!pBWShaderProgram) {
         pBWShaderProgram = new CCGLProgram();
         pBWShaderProgram->initWithVertexShaderFilename("Coin.vsh", "Coin.fsh");
         pBWShaderProgram->autorelease();
         pBWShaderProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
         pBWShaderProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
         pBWShaderProgram->link();
         pBWShaderProgram->updateUniforms();
         CCShaderCache::sharedShaderCache()->addProgram(pBWShaderProgram, "kCoinShaderProgram");

      }

      w->setShaderProgram(pBWShaderProgram);
      w->getShaderProgram()->use();

      myUniformLocation = glGetUniformLocation(w->getShaderProgram()->getProgram(), "timeratio");
      glUniform1f(myUniformLocation, 0.);
      alphaUniformLocation = glGetUniformLocation(w->getShaderProgram()->getProgram(), "analpha");

      

      return w;
   }
   return NULL;

}

//bool init();
void Pickup::toggleOn(bool on)
{
   m_bLightOn = on;
   // later do complete invisible?
   // for now opacity for debug
   setOpacity(isDangerous() ? 255 : STATE->inactiveOpacity());
}

void Pickup::update(float dt)
{
   m_fSecsExisted += dt;
}

bool Pickup::isDangerous()
{
   // don't give me shit about xor or whatever other things
   // i could do in one line cause i'm an idiot
   return (m_bLightOn && m_bLight) ||
      (!m_bLightOn && !m_bLight);
}

void Pickup::draw()
{
 
   if (pBWShaderProgram) {
      pBWShaderProgram->use();
      glUniform1f(myUniformLocation, m_fSecsExisted);
      glUniform1f(alphaUniformLocation, getOpacity() / 255.);
   }

   // don't draw when dead
   CCSprite::draw();
}
