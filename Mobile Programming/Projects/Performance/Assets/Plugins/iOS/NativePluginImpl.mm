
#import "NativePluginImpl.h"
#import "NativeUtils.h"


@implementation NativePluginService

-(id) init
{
    self = [super init];
    m_sStatus = @"Created";
    m_bInitialized = true;
    return self;
}

- (NSString *)GetStatus
{
    return m_sStatus;
}

- (int) GetInt
{
    return 1;
}

- (float) GetFloat
{
    return 0.1f;
}

- (bool) IsReady
{
    return m_bInitialized;
}

@end

static NativePluginService* s_oNativeObject = nil;

// When native code plugin is implemented in .mm / .cpp file, then functions
// should be surrounded with extern "C" block to conform C function naming rules
extern "C" {

    void _Init()
	{
		if(s_oNativeObject == nil)
        {
            s_oNativeObject = [[NativePluginService alloc] init];
        }
	}
	
    const char* _GetStatus()
    {
        if(s_oNativeObject == nil)
        {
            return MakeStringCopy("Error - no native object created");
        }
        
        return MakeStringCopy([[s_oNativeObject GetStatus] UTF8String]);
    }
    
    int _GetIntValue()
    {
        if(s_oNativeObject == nil)
        {
            return 0;
        }
        
        return [s_oNativeObject GetInt];
    }
    
    float _GetFloatValue()
    {
        if(s_oNativeObject == nil)
        {
            return 0.0f;
        }
        
        return [s_oNativeObject GetFloat];
    }
    
    bool _IsReady()
    {
        if(s_oNativeObject == nil)
        {
            return false;
        }
        
        return [s_oNativeObject IsReady];
    }
}

