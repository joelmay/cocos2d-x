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


void SpriteFrameCacheX::addSpriteFramesWithFileX(const std::string& plist)
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
        
        // remove .xxx
        size_t startPos = texturePath.find_last_of(".");
        texturePath = texturePath.erase(startPos);
        
        // append .png
        texturePath = texturePath.append(".png");
        
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
            int totalAtlasW = parseAttrInt(topNode, "width" , 1024);
            int totalAtlasH = parseAttrInt(topNode, "height", 1024);
            
            auto spriteNode = topNode->first_node();
            
            while (spriteNode != nullptr){
                
                std::string idName = parseAttrString(spriteNode, "id");
//                int         intId  = parseAttrInt   (spriteNode, "intId");
                int         x      = parseAttrInt   (spriteNode, "x");
                int         y      = parseAttrInt   (spriteNode, "y");
                int         w      = parseAttrInt   (spriteNode, "width");
                int         h      = parseAttrInt   (spriteNode, "height");
//                int         atlasW = parseAttrInt   (spriteNode, "atlasWidth");
//                int         atlasH = parseAttrInt   (spriteNode, "atlasHeight");
                int         regX   = parseAttrInt   (spriteNode, "regX");
                int         regY   = parseAttrInt   (spriteNode, "regY");
                
                bool bRotated = parseAttrBool(spriteNode, "sideways");
                
                Rect frame(x,y,w,h);
                Vec2 offset(regX,regY);
                Size sourceSize(totalAtlasW, totalAtlasH);
                
                // create frame
                SpriteFrame *spriteFrame = SpriteFrame::createWithTexture(texture,
                                                             frame,
                                                             bRotated,
                                                             offset,
                                                             sourceSize
                                                             );
                
                _spriteFrames.insert(idName, spriteFrame);
                
                
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
