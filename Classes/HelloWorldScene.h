#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "RSCCBMacros.h"

// include Chipmunk headers
#include "chipmunk.h"

class ChipmunkPhysicsSprite : public cocos2d::CCSprite
{
public:
   ChipmunkPhysicsSprite();
   virtual ~ChipmunkPhysicsSprite();
   void setPhysicsBody(cpBody* body);
   virtual bool isDirty(void);
   virtual cocos2d::CCAffineTransform nodeToParentTransform(void);
private:
   cpBody* m_pBody;    // strong ref
};

// HelloWorld Layer
class HelloWorld : public cocos2d::CCLayer {
public:
   HelloWorld();
   ~HelloWorld();
   //bool init();
   static cocos2d::CCScene* scene();
   CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(HelloWorld, create);
    
   virtual void keyPressed(int keyCode);
   virtual void keyReleased(int keyCode);

   void extraSetup();

   void initPhysics();
   void addNewSpriteAtPosition(cocos2d::CCPoint p);
   virtual void update(float dt);
   virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
   virtual void didAccelerate(cocos2d::CCAcceleration* pAccelerationValue);

   static HelloWorld* createHack();

   void updateUI(int health, int dist, bool visible);
private:
   //cocos2d::CCTexture2D* m_pSpriteTexture; // weak ref
   cocos2d::CCLabelTTF* m_pHealthLabel;
   cocos2d::CCLabelTTF* m_pDistLabel;
   cocos2d::CCLayer* m_pTopLayer;
   cpSpace* m_pSpace; // strong ref
   cpShape* m_pWalls[4];
};

RSCCB_DECLARE_LAYER_LOADER(HelloWorld)

#endif // __HELLOWORLD_SCENE_H__
