#include "cocos2d.h"
#include "cocos-ext.h"

#include "rs_util.h"
#include "rs_geometry.h"

#include "GameState.h"
#include "Runner.h"
#include "WallTrap.h"
#include "HelloWorldScene.h"
#include "BackgroundPanel.h"
USING_NS_CC;
USING_NS_CC_EXT;

GameState* GameState::pInst;

GameState:: GameState()
{

}

void GameState::init()
{
   m_fZoom = 1.;
   m_bLightOn = false;
   m_pLayer = NULL;
   m_pRunner = NULL;
   m_fDistToNextTrap = 400.;
}

void GameState::update(float dt)
{
   // if we need more objects add shit
   float farX = 0;
   if (m_vObjs.size()) {
      farX = m_vObjs[m_vObjs.size() -1]->getPosition().x;
      farX += m_vObjs[m_vObjs.size() -1]->getScale() * m_vObjs[m_vObjs.size() -1]->getContentSize().width * .5;
   }

   if (farX < 1000) {

      // add shit
      BackgroundPanel* pan = BackgroundPanel::createHack();
      CCSize s = CCDirector::sharedDirector()->getWinSize();

      // set pos to overlap
      pan->setPosition(ccp(farX + pan->getScale() * pan->getContentSize().width / 2,
                           s.height / 2));
                              
                              
      // set light
      if (m_pLayer) {
         m_pLayer->addChild(pan, -2);
         pan->retain();
         m_vObjs.push_back(pan);
      }
   }

   for (int i = 0; i < (int)m_vObjs.size(); i++) {
      CCPoint pos = m_vObjs[i]->getPosition();
      // fake parallax for backgrounds
      pos.x -= dt * m_pRunner->getSpeed() * .7;
      m_vObjs[i]->setPosition(pos); 

      if (pos.x < -1000) { // FUCKINGBADBHACK
         m_vObjs[i]->removeFromParent();
         m_vObjs[i]->release();
         m_vObjs.erase(m_vObjs.begin() + i);
         i--; 
      }
   }


   // traps
   RSCube runnerCube;
   runnerCube.pos.x = m_pRunner->getPosition().x;
   runnerCube.pos.y = m_pRunner->getPosition().y;
   runnerCube.pos.z = 0.;

   runnerCube.size.x = m_pRunner->getWidth();
   runnerCube.size.y = m_pRunner->getHeight();
   runnerCube.size.z = 1.;

   m_fDistToNextTrap -= m_pRunner->getSpeed() * dt;
   for (int i = 0; i < (int)m_vTraps.size(); i++) {
      WallTrap* trap;
      trap = m_vTraps[i];
      CCPoint pos = trap->getPosition();
      pos.x -= dt * m_pRunner->getSpeed();
      trap->setPosition(pos); 

      // check for overlap
      if (trap->isDangerous()) {
         RSCube trapCube;
         trapCube.pos.x = pos.x;
         trapCube.pos.y = pos.y;
         trapCube.pos.z = 0.;
         trapCube.size.x = trap->getWidth();
         trapCube.size.y = trap->getScaleY() * trap->getContentSize().height;
         trapCube.size.z = 1.;

         int res;
         if (rs_cube_overlaps(&runnerCube, &trapCube, &res)) {
            // collide
            // flash red or something
            CCLabelTTF* owLabel;
            owLabel = CCLabelTTF::create("OWW!", "Minecraftia-Regular", 40);
            owLabel->setPosition(ccp(runnerCube.pos.x, runnerCube.pos.y + runnerCube.size.y * .7));

            CCFiniteTimeAction *seq = CCSequence::createWithTwoActions(
                                                                       CCMoveTo::create(.1f, ccp(runnerCube.pos.x, runnerCube.pos.y + runnerCube.size.y * .9 + 100)),
                                                                       CCCallFuncN::create(owLabel, callfuncN_selector(CCNode::removeFromParentAndCleanup))
                                                                       
                                                                       );
            m_pLayer->addChild(owLabel);
            owLabel->setColor(ccc3(255,0,0));
            owLabel->runAction(seq);

         }
      }
      if (pos.x + trap->getWidth() * .5 < 0) {
         trap->removeFromParent();
         trap->release();
         m_vTraps.erase(m_vTraps.begin() + i);
         i--; 
      }
   }

   // add them
   if (m_fDistToNextTrap <= 0) {
      // add a trap of some length
      // it can't be more than half the view size because
      // the player needs to be able to switch back
      // at most it should be a quarter i guess
      CCSize s = CCDirector::sharedDirector()->getWinSize();
      float trapWidth = (randfloat(.35) + .05) * s.width / m_fZoom;
      float runnerWidth = .1;

      // for now randflip it for light
      WallTrap* trap = WallTrap::createHack(randint(2));
      trap->setWidth(trapWidth);
      trap->setPosition(ccp(s.width / m_fZoom + trapWidth / 2,
                            s.height / 2));
      
      //add to our trap lists
      if (m_pLayer) {
         m_pLayer->addChild(trap);
         trap->retain();
         m_vTraps.push_back(trap);
      }

      // do some 
      m_fDistToNextTrap = runnerWidth + trapWidth + randfloat(.5) * s.width / m_fZoom;      
   }

   // faster and faster at 1 pixel / s*s
   if (m_pRunner->getSpeed() < 2000.)
      m_pRunner->addSpeed(dt * 4);


}

void GameState::toggleLight()
{
   m_bLightOn = !m_bLightOn;
   for (unsigned int i = 0; i < m_vObjs.size(); i++) {
      m_vObjs[i]->toggleOn(m_bLightOn);
   }

   for (int i = 0; i < (int)m_vTraps.size(); i++) {
      m_vTraps[i]->toggleOn(m_bLightOn);
   }
}

GameState* GameState::instance()
{
   if (!pInst) {
      pInst = new GameState();
      pInst->init();
   }
   return pInst;
}

