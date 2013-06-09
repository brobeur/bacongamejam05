
#include "rs_util.h"
#include "rs_geometry.h"

#include "SimpleAudioEngine.h"
#include "GameState.h"
#include "Runner.h"
#include "WallTrap.h"
#include "Pickup.h"
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
   m_fSecsSinceSwitch = 0.;
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

#define kNewbieSecs 60.
// opacity is out of 255
#define kNewbieOpacity 60
int GameState::inactiveOpacity()
{
   return SIMP_MAX(0., 1. - (m_fSecondsAlive / kNewbieSecs)) * kNewbieOpacity;
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

   // remove most children
   CCNode* junk;
   while ((junk = m_pTopLayer->getChildByTag(-1))) {
      junk->removeFromParent();
   }

   m_pRunner->setHealth(1.);
}

/*
static float s_fMessageDist[] =
   {
      200,
      1000,
      5000,
      10000
   };

static const char* s_sMessage[] =
   {
      "Hello"
   };

void GameState::updateTutorial()
{
   if (m_bDead)
      return;

   // first message at 100
   for (int i = 0; i < kNumMessages; i++) {
      if (!m_bMessageShown[i] && m_fDist > s_fMessageDist[i]) {
      CCLabelTTF* label = CCLabelTTF::create(s_sMessage[i], "fonts/Minecraftia.ttf", 40);
      label->setPosition(ccp(s.width / 2, s.height * .9));
      label->setColor(ccc3(255,255,0));
      m_pTopLayer->addChild(label);

      }
   }
}
*/


void GameState::update(float dt)
{
   m_fSecsSinceSwitch += dt;
   WallTrap::setTimeSinceToggle(m_fSecondsAlive);

   m_pLayer->updateUI((int)(m_fHealth * 20), (int)m_fDist, !m_bDead);

   CCSize s = CCDirector::sharedDirector()->getWinSize();
   if (m_bDead) {
      m_fDeadSecs += dt;
   }

   // if health is gone you're dead
   if (m_fHealth <= 0 && !m_bDead) {
      // cache best score
      m_fMaxDist = SIMP_MAX(m_fDist, m_fMaxDist);
      m_fMaxSecondsAlive = SIMP_MAX(m_fSecondsAlive, m_fMaxSecondsAlive);

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
      if (m_fMaxDist != m_fDist) 
         snprintf(buf, 255, "Best so far: %i pixels", (int)m_fMaxDist);
      else
         snprintf(buf, 255, "New best!");
      deadLabel = CCLabelTTF::create(buf, "fonts/Minecraftia.ttf", 28);
      deadLabel->setPosition(ccp(s.width / 2, s.height * .4));
      deadLabel->setColor(ccc3(255,0,0));

      m_pTopLayer->addChild(deadLabel);

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

      trap->update(dt);
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

            char soundName[32] = {0};
            snprintf(soundName, 31, "sounds/Hit_Hurt%i.wav", randint(4));
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(soundName);

         }
      }
      if (pos.x + trap->getWidth() * .5 < 0) {
         trap->removeFromParent();
         trap->release();
         m_vTraps.erase(m_vTraps.begin() + i);
         i--; 
      }
   }

   for (int i = 0; i < (int)m_vPickups.size(); i++) {
      bool pickedUp = false;
      Pickup* pickup;
      pickup = m_vPickups[i];

      pickup->update(dt);
      CCPoint pos = pickup->getPosition();
      pos.x -= dt * m_pRunner->getSpeed();
      pickup->setPosition(pos); 

      // check for overlap
      if (pickup->isDangerous() && !m_bDead) {
         RSCube pickupCube;
         pickupCube.pos.x = pos.x;
         pickupCube.pos.y = pos.y;
         pickupCube.pos.z = 0.;
         pickupCube.size.x = pickup->getScaleX() * pickup->getContentSize().width;
         pickupCube.size.y = pickup->getScaleY() * pickup->getContentSize().height;
         pickupCube.size.z = 1.;

         int res;
         if (rs_cube_overlaps(&runnerCube, &pickupCube, &res)) {
            // collide
            // flash red or something
            CCLabelTTF* owLabel;
            
            owLabel = CCLabelTTF::create("NOM", "fonts/Minecraftia.ttf", 40);
            owLabel->setPosition(ccp(runnerCube.pos.x, runnerCube.pos.y + runnerCube.size.y * .7));

            CCFiniteTimeAction *seq = CCSequence::createWithTwoActions(
                                                                       CCMoveTo::create(.5f, ccp(runnerCube.pos.x, runnerCube.pos.y + runnerCube.size.y * .8)),
                                                                       CCCallFuncN::create(owLabel, callfuncN_selector(CCNode::removeFromParentAndCleanup))
                                                                       
                                                                       );
            m_pLayer->addChild(owLabel);
            owLabel->setColor(ccc3(30,140,87));
            owLabel->runAction(seq);

            // shake them
            seq = CCSequence::createWithTwoActions(                             CCMoveTo::create(dt,
                                                                                                 ccp(pickupCube.pos.x + randint(15) - 7, pickupCube.pos.y + randint(15) - 7)),
                                                                                CCMoveTo::create(dt,                                                                    ccp(pickupCube.pos.x, pickupCube.pos.y)));
 
            pickup->runAction(seq);
            
            // subtract health
            m_iPickups += 1;
            m_fHealth = SIMP_MIN(kMaxHealth, m_fHealth + dt);
            //m_pRunner->setHealth( m_fHealth / kMaxHealth);

            char soundName[32] = {0};
            snprintf(soundName, 31, "sounds/Pickup_Coin%i.wav", randint(5));
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(soundName);
            pickedUp = true;
         }
      }
      if (pickedUp || 
          (pos.x + pickup->getContentSize().width * pickup->getScaleX() * .5 < 0)) {
         pickup->removeFromParent();
         pickup->release();
         m_vPickups.erase(m_vPickups.begin() + i);
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

      // after the last trap add some bacons?
      // low chance of 1 2 or 3 bacons
      int numPickups = randint(26) - 20; 
      for (int i = 0; i < numPickups; i++) {
         Pickup* pickup = Pickup::createHack(randint(2));
         pickup->setPosition(ccp(trap->getPosition().x + trapWidth * .5 + m_fDistToNextTrap * (i + .5) / numPickups,
                                 s.height * .3));
         if (m_pLayer) {
            m_pLayer->addChild(pickup);
            pickup->retain();
            m_vPickups.push_back(pickup);
         }
         
      }

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
   m_fSecsSinceSwitch = 0;
   m_bLightOn = !m_bLightOn;
   for (unsigned int i = 0; i < m_vObjs.size(); i++) {
      m_vObjs[i]->toggleOn(m_bLightOn);
   }

   for (int i = 0; i < (int)m_vTraps.size(); i++) {
      m_vTraps[i]->toggleOn(m_bLightOn);
   }

   for (int i = 0; i < (int)m_vPickups.size(); i++) {
      m_vPickups[i]->toggleOn(m_bLightOn);
   }

   const char* soundName = (m_bLightOn ?
                            "sounds/Lights0.wav" :
                            "sounds/Lights1.wav");
                            
   CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(soundName);

   CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(m_bLightOn ? 1. : .4);

}

GameState* GameState::instance()
{
   if (!pInst) {
      pInst = new GameState();
      pInst->init();
   }
   return pInst;
}

