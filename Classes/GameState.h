//gamestaet
#ifndef __GAMESTATEBGJ__
#define __GAMESTATEBGJ__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>

class HelloWorld;
class Runner;
class BackgroundPanel;
class WallTrap;
class Pickup;

#define STATE (GameState::instance())

#define kMaxHealth (2.)
#define kInitialSpeed 200.

class GameState
{
 public:
   static GameState* instance();

   void toggleLight();
   void setLayer(HelloWorld* layer){m_pLayer = layer;}
   void setTopLayer(cocos2d::CCLayer* layer){m_pTopLayer = layer;}
   void setRunner(Runner* r);
   void update(float dt);
   bool isLightOn(){return m_bLightOn;}

   void restartGame();
   bool isDead(){return m_bDead;}
   int inactiveOpacity();
   bool canRestart();
   float secsSinceSwitch() {return m_fSecsSinceSwitch;}
 private:
   static GameState* pInst;
   GameState();
   void init();

   bool m_bLightOn;
   cocos2d::CCLayer* m_pTopLayer;
   HelloWorld* m_pLayer;
   Runner* m_pRunner;
   //cocos2d::CCArray* m_pEntities;
   std::vector<BackgroundPanel*> m_vObjs;
   std::vector<Pickup*> m_vPickups;
   std::vector<WallTrap*> m_vTraps;

   float m_fZoom;

   float m_fDistToNextTrap;

   float m_fSecsSinceSwitch;
   float m_fSecondsAlive;
   float m_fMaxSecondsAlive;
   float m_fHealth; // number of seconds you can take hits
   float m_fDist; // number of seconds you can take hits
   float m_fMaxDist; // number of seconds you can take hits
   bool m_bDead;

   // timer to prevent sudden click
   float m_fDeadSecs;

   int  m_iPickups;
};

#endif
