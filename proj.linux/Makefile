EXECUTABLE = crosstest

INCLUDES = -I.. -I../Classes

SOURCES = main.cpp \
        ../Classes/AppDelegate.cpp \
        ../Classes/HelloWorldScene.cpp \
        ../Classes/Runner.cpp \
        ../Classes/TitleScreen.cpp \
        ../Classes/GameState.cpp \
        ../Classes/BackgroundPanel.cpp \
        ../Classes/WallTrap.cpp \
        ../Classes/rs_geometry.c \
        ../Classes/rs_util.c

COCOS_ROOT = ../../..



include $(COCOS_ROOT)/cocos2dx/proj.linux/cocos2dx.mk

COCOS_LIBS = $(LIB_DIR)/libcocos2d.so
COCOS_LIBS += $(LIB_DIR)/libcocosdenshion.so

include $(COCOS_ROOT)/cocos2dx/proj.linux/cocos2dx.mk

SHAREDLIBS += -lcocos2d -lextension -lcocosdenshion -lchipmunk -lGL 
STATIC_LIBS += $(LIB_DIR)/libextension.a
INCLUDES += -I$(COCOS_ROOT)/extensions
INCLUDES += -I$(COCOS_ROOT)/CocosDenshion/include
INCLUDES += -I$(COCOS_ROOT)/external/chipmunk/include/chipmunk


$(TARGET): $(OBJECTS) $(STATICLIBS) $(COCOS_LIBS) $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_LINK)$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(SHAREDLIBS) $(STATICLIBS)

$(OBJ_DIR)/%.o: %.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@

$(OBJ_DIR)/%.o: ../%.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@

$(OBJ_DIR)/%.o: ../%.c $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@
