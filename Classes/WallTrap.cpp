//cpp walltrap

#include "GameState.h"
#include "WallTrap.h"

USING_NS_CC;
USING_NS_CC_EXT;


WallTrap::WallTrap() :
   m_bLight(false),
   m_fSecsExisted(0.)
{

}

WallTrap::~WallTrap()
{

}
#define kWallTrapTex "enemy1_0.png"
#define kWallTrapDarkTex "enemy2_0.png"
static CCGLProgram *pBWShaderProgram;
static GLint myUniformLocation;
static GLint alphaUniformLocation;

WallTrap* WallTrap::createHack(bool light)
{
   WallTrap* w = WallTrap::create();
   if (w && w->initWithFile(light ? kWallTrapDarkTex : kWallTrapTex)) {
      // set the scale to match the size in height
      CCSize s = CCDirector::sharedDirector()->getWinSize();
      w->setScaleY(.6 * s.height / w->getContentSize().height);
      w->m_bLight = light;
      w->toggleOn(STATE->isLightOn());

      if (!pBWShaderProgram) {
         pBWShaderProgram = new CCGLProgram();
         pBWShaderProgram->initWithVertexShaderFilename("Swirl.vsh", "Swirl.fsh");
         pBWShaderProgram->autorelease();
         pBWShaderProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
         pBWShaderProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
         pBWShaderProgram->link();
         pBWShaderProgram->updateUniforms();
         CCShaderCache::sharedShaderCache()->addProgram(pBWShaderProgram, "kSwirlShaderProgram");

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

void WallTrap::setTimeSinceToggle(float secs)
{
   if (pBWShaderProgram) {
      pBWShaderProgram->use();
      glUniform1f(myUniformLocation, secs);
   }
}

void WallTrap::update(float dt)
{
   m_fSecsExisted += dt;
}

void WallTrap::setWidth(float width)
{
   // set xscale
   m_fWidth = width;

   // for now just scale, later occlude draw via percentage
   setScaleX(width / getContentSize().width);
}

bool WallTrap::isDangerous()
{
   // don't give me shit about xor or whatever other things
   // i could do in one line cause i'm an idiot
   return (m_bLightOn && m_bLight) ||
      (!m_bLightOn && !m_bLight);
}
//bool init();
void WallTrap::toggleOn(bool on)
{
   m_bLightOn = on;
   // later do complete invisible?
   // for now opacity for debug
   setOpacity(isDangerous() ? 255 : STATE->inactiveOpacity());
}

void WallTrap::draw()
{
 
   if (pBWShaderProgram) {
      pBWShaderProgram->use();
      glUniform1f(myUniformLocation, m_fSecsExisted);
      glUniform1f(alphaUniformLocation, getOpacity() / 255.);
   }

   // don't draw when dead
   CCSprite::draw();
}
