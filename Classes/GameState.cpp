
#include "rs_util.h"
#include "rs_geometry.h"

#include "GameState.h"
#include "Runner.h"
#include "WallTrap.h"
#include "HelloWorldScene.h"
#include "BackgroundPanel.h"
USING_NS_CC;
USING_NS_CC_EXT;

#define kAccelRate (8)

GameState* GameState::pInst;

GameState:: GameState()
{

}

void GameState::init()
{
   m_fDeadSecs = 0.;
   m_fZoom = 1.;
   m_bLightOn = false;
   m_pLayer = NULL;
   m_pRunner = NULL;
   m_fDistToNextTrap = 400.;

   m_fSecondsAlive = 0.;
   m_fMaxSecondsAlive = 0.;
   m_fHealth = kMaxHealth; // number of seconds you can take hits
   m_fDist = 0.;
   m_fMaxDist = 0.;
   m_bDead = false;
}

static float time_to_dist(float t)
{
   // the position equation is
   // x = kAccelRate * t^2 + kInitialSpeed * t
   return kAccelRate * t * t + kInitialSpeed * t;
}

void GameState::restartGame()
{
   // only restart if the user had time to see the death screen
   if (m_bDead && m_fDeadSecs < 1.5)
      return;

   m_fDeadSecs = 0.;
   m_fHealth = kMaxHealth; // number of seconds you can take hits
   m_pRunner->resetSpeed();
   m_fSecondsAlive *= 0.5;
   m_fDist = time_to_dist(m_fSecondsAlive);

   m_pRunner->addSpeed(m_fSecondsAlive * .5 *  kAccelRate);


   m_bDead = false;

   // get rid of previous traps
   for (int i = 0; i < (int)m_vTraps.size(); i++) {
      WallTrap* trap;
      trap = m_vTraps[i];

      trap->removeFromParent();
      trap->release();
      m_vTraps.erase(m_vTraps.begin() + i);
      i--; 
   }   

   m_pTopLayer->removeAllChildren();
   m_pRunner->setHealth(1.);
}

void GameState::update(float dt)
{
   CCSize s = CCDirector::sharedDirector()->getWinSize();
   if (m_bDead) {
      m_fDeadSecs += dt;
   }

   // if health is gone you're dead
   if (m_fHealth <= 0 && !m_bDead) {
      CCLabelTTF* deadLabel = CCLabelTTF::create("GAME OVER", "fonts/Minecraftia.ttf", 40);
      deadLabel->setPosition(ccp(s.width / 2, s.height * .9));
      deadLabel->setColor(ccc3(255,0,0));
      m_pTopLayer->addChild(deadLabel);

      char buf[256];
      snprintf(buf, 255, "Distance reached: %i pixels", (int)m_fDist);
      deadLabel = CCLabelTTF::create(buf, "fonts/Minecraftia.ttf", 28);
      deadLabel->setPosition(ccp(s.width / 2, s.height * .6));
      deadLabel->setColor(ccc3(255,0,0));

      m_pTopLayer->addChild(deadLabel);
      m_bDead = true;

      // retry from half way
      snprintf(buf, 255, "Click to retry from %i pixels", (int)time_to_dist(m_fSecondsAlive * .5));

      deadLabel = CCLabelTTF::create(buf, "fonts/Minecraftia.ttf", 28);
      deadLabel->setPosition(ccp(s.width / 2, s.height * .2));
      deadLabel->setColor(ccc3(255,0,0));
      m_pTopLayer->addChild(deadLabel);

      return;
   }


   // if we need more objects add shit
   float farX = 0;
   if (m_vObjs.size()) {
      farX = m_vObjs[m_vObjs.size() -1]->getPosition().x;
      farX += m_vObjs[m_vObjs.size() -1]->getScale() * m_vObjs[m_vObjs.size() -1]->getContentSize().width * .5;
   }

   if (farX < s.width) {

      // add shit
      BackgroundPanel* pan = BackgroundPanel::createHack();

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

      if (pos.x + (m_vObjs[i]->getContentSize().width * 
                   m_vObjs[i]->getScaleX() * .5) < 0) {
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
      if (trap->isDangerous() && !m_bDead) {
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
            owLabel = CCLabelTTF::create("OWW!", "fonts/Minecraftia.ttf", 40);
            owLabel->setPosition(ccp(runnerCube.pos.x, runnerCube.pos.y + runnerCube.size.y * .7));

            CCFiniteTimeAction *seq = CCSequence::createWithTwoActions(
                                                                       CCMoveTo::create(.1f, ccp(runnerCube.pos.x, runnerCube.pos.y + runnerCube.size.y * .9 + 100)),
                                                                       CCCallFuncN::create(owLabel, callfuncN_selector(CCNode::removeFromParentAndCleanup))
                                                                       
                                                                       );
            m_pLayer->addChild(owLabel);
            owLabel->setColor(ccc3(255,0,0));
            owLabel->runAction(seq);

            // shake them
            seq = CCSequence::createWithTwoActions(                             CCMoveTo::create(dt,
                                                                                                 ccp(trapCube.pos.x + randint(15) - 7, trapCube.pos.y + randint(15) - 7)),
                                                                                CCMoveTo::create(dt,                                                                    ccp(trapCube.pos.x, trapCube.pos.y)));
 
            trap->runAction(seq);
            
            // subtract health
            m_fHealth -= dt;
            m_pRunner->setHealth( m_fHealth / kMaxHealth);

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
      float runnerWidth = m_pRunner->getWidth();

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
      m_fDistToNextTrap = runnerWidth * 1.2 + trapWidth + randfloat(.5) * s.width / m_fZoom;      
   }

   // faster and faster at 1 pixel / s*s
   if (!m_bDead) {
      m_fDist += m_pRunner->getSpeed() * dt;
      m_fSecondsAlive += dt;
   }

   if (m_pRunner->getSpeed() < 2000.)
      m_pRunner->addSpeed(dt * kAccelRate);

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

