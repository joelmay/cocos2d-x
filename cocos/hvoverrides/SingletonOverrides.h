//
//  SingletonOverrides.h
//  cocos2d_libs
//
//  Created by Joel May on 7/8/15.
//
//

#ifndef __cocos2d_libs__SingletonOverrides__
#define __cocos2d_libs__SingletonOverrides__

namespace hv {
    
    class SingletonOverrides {
    public:
        static SingletonOverrides* getInstance();
        
        void createAll(void);
    };
}

#endif /* defined(__cocos2d_libs__SingletonOverrides__) */
