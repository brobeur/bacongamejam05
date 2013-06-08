//cpp walltrap

#include "GameState.h"
#include "WallTrap.h"

USING_NS_CC;
USING_NS_CC_EXT;


WallTrap::WallTrap() :
   m_bLight(false)
{

}

WallTrap::~WallTrap()
{

}
#define kWallTrapTex "ninjasprite.png"
#define kWallTrapDarkTex "ninjaspritedark.png"
WallTrap* WallTrap::createHack(bool light)
{
   WallTrap* w = WallTrap::create();
   if (w && w->initWithFile(light ? kWallTrapDarkTex : kWallTrapTex)) {
      // set the scale to match the size in height
      CCSize s = CCDirector::sharedDirector()->getWinSize();
      w->setScaleY(.6 * s.height / w->getContentSize().height);
      w->m_bLight = light;
      w->toggleOn(STATE->isLightOn());

      return w;
   }
   return NULL;
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


