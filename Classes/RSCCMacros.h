//
//  RSCCMacros.h
//  doghouse
//
//  Created by Michael Chinen on 13/05/17.
//
//

#ifndef doghouse_RSCCMacros_h
#define doghouse_RSCCMacros_h

// use this to ensure you don't fuck up the keys with copy pasta

#define LOADINTKEY(db, baseKey, varName, keyName)  \
   {\
   char szKey[64] = {0};\
   snprintf(szKey, 63, "%s%s", baseKey, keyName);\
   varName = db->getIntegerForKey(szKey, varName);\
   }
#define LOADINT(db, baseKey, varName)                   \
   LOADINTKEY(db, baseKey, varName, #varName)


#define SAVEINTKEY(db, baseKey, varName, keyName)          \
   {\
   char szKey[64] = {0};\
   snprintf(szKey, 63, "%s%s", baseKey, keyName);\
   db->setIntegerForKey(szKey, varName);\
   }
#define SAVEINT(db, baseKey, varName)                   \
   SAVEINTKEY(db, baseKey, varName, #varName)



#endif
