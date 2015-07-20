//
//  SpriteFrameCacheX.h
//  MetroSimple
//
//  Created by Joel May on 6/16/15.
//
//

#ifndef __MetroSimple__SpriteFrameCacheX__
#define __MetroSimple__SpriteFrameCacheX__

#include "rapidxml.hpp"
#include "cocos2d.h"

namespace hv {
    
class SpriteFrameX;
    
class SpriteFrameCacheX : public cocos2d::SpriteFrameCache {
    
private:
    static int   parseAttrInt  (rapidxml::xml_node<char>* pSpriteNode, const char *attrName, int   defaultVal=0);
    static float parseAttrFloat(rapidxml::xml_node<char>* pSpriteNode, const char *attrName, float defaultVal=0.0f);
    static bool  parseAttrBool (rapidxml::xml_node<char>* pSpriteNode, const char *attrName, bool  defaultVal=false);
    static std::string parseAttrString(rapidxml::xml_node<char>* pSpriteNode, const char *attrName, const char *szDefaultVal="");
    
    SpriteFrameX* parseFrame      (rapidxml::xml_node<char>* spriteNode, cocos2d::Texture2D *texture);
    SpriteFrameX* parseNestedFrame(rapidxml::xml_node<char>* spriteNode, cocos2d::Texture2D *texture);
    
public:
    static SpriteFrameCacheX* getInstance();
    
    SpriteFrameCacheX(void);
    
    void addSpriteFramesWithFileX(const std::string& plist, const std::string& atlasPng="");
    
};
}

#endif /* defined(__MetroSimple__SpriteFrameCacheX__) */
