//
//  HVDirector.h
//  cocos2d_libs
//
//  Created by Joel May on 7/8/15.
//
//

#ifndef __cocos2d_libs__HVDirector__
#define __cocos2d_libs__HVDirector__

#include "cocos2d.h"

namespace hv {
    
    // TODO: This is probably Apple-specific.
    class Director : public cocos2d::DisplayLinkDirector {
    public:
        static hv::Director* getInstance();
        
        Director(void);
        
        virtual cocos2d::Renderer* createRenderer(void) override;
    };
    
}

#endif /* defined(__cocos2d_libs__HVDirector__) */
