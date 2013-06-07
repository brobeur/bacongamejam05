//
//  RSCCBMacros.h
//  doghouse
//
//  Created by Michael Chinen on 13/04/10.
//
//

#ifndef doghouse_RSCCBMacros_h
#define doghouse_RSCCBMacros_h

#define RSCCB_DECLARE_LAYER_LOADER(className) \
          RSCCB_DECLARE_LOADER(CCLayerLoader, className)
#define RSCCB_DECLARE_BASIC_LOADER(className) \
          RSCCB_DECLARE_LOADER(CCNodeLoader, className)
#define RSCCB_DECLARE_LOADER(loaderClass, className) \
class className ## Loader : public cocos2d::extension::loaderClass { \
public: \
   CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(className ## Loader, loader); \
   static className* loadFromFile(); \
protected: \
   CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(className); \
};

#define RSCCB_DEFINE_BASIC_LOADER(className, ccbiName)   \
className* className##Loader::loadFromFile()\
{\
   cocos2d::extension::CCNodeLoaderLibrary * ccNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();\
   \
   ccNodeLoaderLibrary->registerCCNodeLoader( #className, className##Loader::loader());\
   \
   cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);\
   ccbReader->autorelease();\
   \
   cocos2d::CCNode * node = ccbReader->readNodeGraphFromFile( #ccbiName ".ccbi"); \
   return (className*) node; \
}

#define RSCCB_DECLARE_CUSTOMCCBI_LOADER(className) \
class className ## Loader : public cocos2d::extension::CCNodeLoader { \
public: \
   CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(className ## Loader, loader); \
   static className* loadFromFile(const char* fn); \
protected: \
   CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(className); \
};

#define RSCCB_DEFINE_CUSTOMCCBI_LOADER(className)   \
className* className##Loader::loadFromFile(const char* fn)\
{\
   cocos2d::extension::CCNodeLoaderLibrary * ccNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();\
   \
   ccNodeLoaderLibrary->registerCCNodeLoader( #className, className##Loader::loader());\
   \
   cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);\
   ccbReader->autorelease();\
   \
   cocos2d::CCNode * node = ccbReader->readNodeGraphFromFile(fn); \
   return (className*) node; \
}

// meant to be called from within onAssignCCBMemberVariable
#define RSCCB_GLUE(mName, cType) \
   CCB_MEMBERVARIABLEASSIGNER_GLUE(this, #mName, cType, this->mName);



#endif
