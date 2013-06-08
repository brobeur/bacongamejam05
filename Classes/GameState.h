//gamestaet
#ifndef __GAMESTATEBGJ__
#define __GAMESTATEBGJ__

#include <vector>

class HelloWorld;
class Runner;
class BackgroundPanel;
class WallTrap;

#define STATE (GameState::instance())

class GameState
{
 public:
   static GameState* instance();

   void toggleLight();
   void setLayer(HelloWorld* layer){m_pLayer = layer;}
   void setRunner(Runner* r){m_pRunner = r;}
   void update(float dt);
   bool isLightOn(){return m_bLightOn;}
 private:
   static GameState* pInst;
   GameState();
   void init();

   bool m_bLightOn;
   HelloWorld* m_pLayer;
   Runner* m_pRunner;
   //cocos2d::CCArray* m_pEntities;
   std::vector<BackgroundPanel*> m_vObjs;
   std::vector<WallTrap*> m_vLightTraps;
   std::vector<WallTrap*> m_vDarkTraps;
   std::vector<WallTrap*> m_vTraps;

   float m_fZoom;

   float m_fDistToNextTrap;
};

#endif
