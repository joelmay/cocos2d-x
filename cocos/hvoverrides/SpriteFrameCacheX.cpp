//
//  SpriteFrameCacheX.cpp
//  MetroSimple
//
//  Created by Joel May on 6/16/15.
//
//

#include <string>
#include <iostream>
#include "SpriteFrameCacheX.h"
#include "SpriteFrameX.h"
#include "NestedSpriteFrame.h"
//#include "rapidxml.hpp"

using namespace std;
using namespace cocos2d;
using namespace hv;



//namespace cocos2d {
//    extern void setSharedSpriteFrameCacheSingleton(SpriteFrameCache* overrideInstance);
//}

SpriteFrameCacheX::SpriteFrameCacheX(void)
{
    printf("ctor");
}

SpriteFrameCacheX* SpriteFrameCacheX::getInstance()
{
    static bool bFirstCall = true;
    
    if (bFirstCall){
        auto sharedCache = new (std::nothrow) SpriteFrameCacheX();
        sharedCache->init();
        setInstance(sharedCache);
        //setSharedSpriteFrameCacheSingleton(sharedCache);
        bFirstCall = false;
    }
    
    return static_cast<SpriteFrameCacheX*>(cocos2d::SpriteFrameCache::getInstance());
}

int SpriteFrameCacheX::parseAttrInt(rapidxml::xml_node<char>* pSpriteNode, const char *attrName, int defaultVal)
{
    auto attrNode = pSpriteNode->first_attribute(attrName);

    if (attrNode != nullptr){
        //std::string name  = idVal->name();
        std::string value = attrNode->value();
        return atoi(value.c_str());
    }
    return defaultVal;
}

float SpriteFrameCacheX::parseAttrFloat(rapidxml::xml_node<char>* pSpriteNode, const char *attrName, float defaultVal)
{
    auto attrNode = pSpriteNode->first_attribute(attrName);

    if (attrNode != nullptr){
        std::string value = attrNode->value();
        return atof(value.c_str());
    }
    return defaultVal;
}

bool SpriteFrameCacheX::parseAttrBool(rapidxml::xml_node<char>* pSpriteNode, const char *attrName, bool defaultVal)
{
    auto attrNode = pSpriteNode->first_attribute(attrName);

    if (attrNode != nullptr){
        std::string value = attrNode->value();
        if (value == "0") return false;
        if (value == "1") return true;
        switch(value.at(0)){
            case '0': case 'f': case 'F': return false;
            case '1': case 't': case 'T': return false;
            default: return defaultVal;
        }
    }
    return defaultVal;
}

std::string SpriteFrameCacheX::parseAttrString(rapidxml::xml_node<char>* pSpriteNode, const char *attrName, const char *szDefaultVal)
{
    auto attrNode = pSpriteNode->first_attribute(attrName);

    if (attrNode != nullptr){
        std::string value = attrNode->value();
        return value;
    }
    return szDefaultVal;
}


void SpriteFrameCacheX::addSpriteFramesWithFileX(const std::string& plist, const std::string& atlasPng)
{
    CCASSERT(plist.size()>0, "plist filename should not be nullptr");
    
    // If this is actually a plist file (extension = .plist), then call the
    // similar function in the parent class.
    auto iDot = plist.find_last_of('.');
    if (iDot != std::string::npos){
        auto extension = plist.substr(iDot+1);
        if (extension == "plist"){
            this->addSpriteFramesWithFile(plist);
            return;
        }
    }
    
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(plist);
    if (fullPath.size() == 0)
    {
        // return if plist file doesn't exist
        CCLOG("cocos2d: SpriteFrameCache: can not find %s", plist.c_str());
        return;
    }
    
    if (_loadedFileNames->find(plist) == _loadedFileNames->end())
    {
        ////////////////////////////////////////////////////
        // Create texture.
        string texturePath = plist;
        
        if (atlasPng.length() > 0){
            texturePath = FileUtils::getInstance()->fullPathForFilename(atlasPng);
        }
        else{
            texturePath = plist;
            
            // remove .xxx
            size_t startPos = texturePath.find_last_of(".");
            texturePath = texturePath.erase(startPos);
            
            // append .png
            texturePath = texturePath.append(".png");
        }
        
        Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(texturePath.c_str());
        CCLOG("cocos2d: SpriteFrameCache: Trying to use file %s as texture", texturePath.c_str());
        
        if (texture != nullptr){
            
            //Data data = FileUtils::getInstance()->getDataFromFile(fullPath);
            std::string text = FileUtils::getInstance()->getStringFromFile(fullPath);
            
            using namespace rapidxml;
            rapidxml::xml_document<> doc;    // character type defaults to char
            const char *txt = text.c_str();
            
            //doc.parse<<#int Flags#>>(<#char *text#>)
            doc.parse<0>((char *)txt);
            
            cout << "Name of my first node is: " << doc.first_node()->name() << "\n";
            auto topNode = doc.first_node();
            
            auto firstNode  = topNode->first_node();
            
            /////////////////////////////////////////////////////////
            // First, parse non-nested nodes.
            auto spriteNode = firstNode;
            while (spriteNode != nullptr){
                
                auto subframeNode = spriteNode->first_node("frame");
                
                bool bNestedNode = subframeNode != nullptr;
                
                if (!bNestedNode){
                    SpriteFrameX* spriteFrame = parseFrame(spriteNode, texture);
                    _spriteFrames.insert(spriteFrame->getName(), spriteFrame);
                }
                
                spriteNode = spriteNode->next_sibling();
            }
            
            /////////////////////////////////////////////////////////
            // Second, parse Nested nodes.
            spriteNode = firstNode;
            while (spriteNode != nullptr){
                
                auto subframeNode = spriteNode->first_node("frame");
                
                bool bNestedNode = subframeNode != nullptr;
                
                if (bNestedNode){
                    
                    SpriteFrameX* spriteFrame = parseNestedFrame(spriteNode, texture);
                    _spriteFrames.insert(spriteFrame->getName(), spriteFrame);
                }
                
                spriteNode = spriteNode->next_sibling();
            }
            
            _loadedFileNames->insert(plist);
        }
        else
        {
            CCLOG("cocos2d: SpriteFrameCache: Couldn't load texture");
        }
    }
}

SpriteFrameX* SpriteFrameCacheX::parseFrame(rapidxml::xml_node<char>* spriteNode, Texture2D *texture)
{
    std::string idName = parseAttrString(spriteNode, "id");
    int         intId  = parseAttrInt   (spriteNode, "intId");
    int         x      = parseAttrInt   (spriteNode, "x");
    int         y      = parseAttrInt   (spriteNode, "y");
    int         w      = parseAttrInt   (spriteNode, "width");
    int         h      = parseAttrInt   (spriteNode, "height");
    int         atlasW = parseAttrInt   (spriteNode, "atlasWidth", -1);
    int         atlasH = parseAttrInt   (spriteNode, "atlasHeight", -1);
    int         regX   = parseAttrInt   (spriteNode, "regX");
    int         regY   = parseAttrInt   (spriteNode, "regY");
    
    bool bRotated = parseAttrBool(spriteNode, "sideways");
    
    printf("atlas w,h: %d,%d\n", atlasW, atlasH);
    
    Rect frame(x,y,w,h);
    Vec2 offset(regX,regY);
    Size sourceSize(atlasW, atlasH);
    
    // create frame
    SpriteFrameX *spriteFrame = SpriteFrameX::createWithTexture(idName,
                                                               intId,
                                                               texture,
                                                               frame,
                                                               bRotated,
                                                               offset,
                                                               sourceSize
                                                              );
    return spriteFrame;
}

SpriteFrameX* SpriteFrameCacheX:: parseNestedFrame(rapidxml::xml_node<char>* spriteNode, Texture2D *texture)
{
    std::string idName = parseAttrString(spriteNode, "id");
    int         intId  = parseAttrInt   (spriteNode, "intId");
    
    NestedSpriteFrame* spriteFrame = NestedSpriteFrame::createWithTexture(idName, intId, texture);
    
    auto frameSubNode = spriteNode->first_node("frame");
    
    while (frameSubNode != nullptr){
        
        // TODO: This will be where we handle multiple movieclip frames in on sprite.
        auto spriteRefNode = frameSubNode->first_node("spriteRef");
        
        while (spriteRefNode != nullptr){
            std::string refID    = parseAttrString(spriteRefNode, "refID");
            int         refIntID = parseAttrInt   (spriteRefNode, "refIntID");
            
            float a  = parseAttrFloat   (spriteRefNode, "a" , 1.0f);
            float b  = parseAttrFloat   (spriteRefNode, "b" , 0.0f);
            float c  = parseAttrFloat   (spriteRefNode, "c" , 0.0f);
            float d  = parseAttrFloat   (spriteRefNode, "d" , 1.0f);
            float tx = parseAttrFloat   (spriteRefNode, "tx", 0.0f);
            float ty = parseAttrFloat   (spriteRefNode, "ty", 0.0f);
            
            float alpha = parseAttrFloat   (spriteRefNode, "alpha", 1.0f);
            
            Mat4 m(
                   a   , b   , 0.0f, 0.0f,
                   c   , d   , 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   tx  , ty  , 0.0f, 1.0f
                   );
            
            // Don't actually link the subsprites frame to the container sprite frame
            // yet.  The child sprite might actually be another nested sprite
            // and not created yet.
            spriteFrame->addChild(refID, refIntID, m, alpha);
            
            spriteRefNode = spriteRefNode->next_sibling();
        }
        frameSubNode = frameSubNode->next_sibling();
    }
    
    return spriteFrame;
}
