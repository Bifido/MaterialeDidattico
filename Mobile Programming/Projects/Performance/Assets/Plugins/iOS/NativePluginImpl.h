#import <Foundation/Foundation.h>


@interface NativePluginService : NSObject
{
    // Keeps track of status
    NSString* m_sStatus;
    
    bool m_bInitialized;
}

- (NSString *)GetStatus;
- (int) GetInt;
- (float) GetFloat;
- (bool) IsReady;

@end