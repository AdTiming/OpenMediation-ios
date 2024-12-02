// Copyright 2020 ADTIMING TECHNOLOGY COMPANY LIMITED
// Licensed under the GNU Lesser General Public License Version 3

#import "OpenMediation.h"
#import "OMConfig.h"
#import "OMNetworkUmbrella.h"
#import "OMToolUmbrella.h"
#import "OMMediations.h"
#import "OMUserData.h"
#import "OMEventManager.h"
#import "OMInterstitial.h"
#import "OMRewardedVideo.h"
#import "OMCrossPromotion.h"
#import "OMImpressionDataRouter.h"

@interface OMRewardedVideo()
- (void)preload;
@end

@interface OMInterstitial()
- (void)preload;
@end

@interface OMCrossPromotion()
- (void)preload;
@end

static OpenMediationAdFormat initAdFormats = OpenMediationAdFormatNone;

#define SDKInitCheckInterval 3.0

#define TagMaxLength    48


static NSTimer *SDKInitCheckTimer = nil;

@implementation OpenMediation

+ (void)setUseCacheAdFormat:(OpenMediationAdFormat)useCacheAdFormat {
    [OMConfig sharedInstance].useCacheAdFormat = useCacheAdFormat;
}

+ (void)initWithAppKey:(NSString*)appKey {
    [self initWithAppKey:appKey baseHost:@"https://ads.test.mises.site"];
}
    
+ (void)initWithAppKey:(NSString*)appKey baseHost:(nonnull NSString *)host {
    if (!initAdFormats) {
        [self initWithAppKey:appKey baseHost:host adFormat:(OpenMediationAdFormatRewardedVideo|OpenMediationAdFormatInterstitial|OpenMediationAdFormatCrossPromotion) completionHandler:NULL];
    } else {
        [self initWithAppKey:appKey baseHost:host adFormat:initAdFormats completionHandler:NULL];
    }
}

+ (void)initWithAppKey:(NSString *)appKey adFormat:(OpenMediationAdFormat)initAdTypes {
    [self initWithAppKey:appKey baseHost:@"https://ads.test.mises.site" adFormat:initAdTypes completionHandler:NULL];
}

/// Initializes OpenMediation's SDK with the requested ad types.
+ (void)initWithAppKey:(NSString *)appKey baseHost:(NSString*)host adFormat:(OpenMediationAdFormat)initAdTypes
     completionHandler:(void (^ _Nullable)(NSError* _Nullable))completionHandler {
    [self initWithAppKey:appKey baseHost:host completionHandler:^(NSError * _Nullable error) {
        if (!error) {
            if (initAdTypes & OpenMediationAdFormatInterstitial) {
                [[OMInterstitial sharedInstance]preload];
            }
            if (initAdTypes & OpenMediationAdFormatRewardedVideo) {
                [[OMRewardedVideo sharedInstance]preload];
            }
            if (initAdTypes & OpenMediationAdFormatCrossPromotion) {
                [[OMCrossPromotion sharedInstance]preload];
            }
        }
        if (completionHandler != NULL) {
            completionHandler(error);
        }
    }];
    
    if (SDKInitCheckTimer) {
        [SDKInitCheckTimer invalidate];
        SDKInitCheckTimer = nil;
    }
    SDKInitCheckTimer = [NSTimer scheduledTimerWithTimeInterval:SDKInitCheckInterval target:self selector:@selector(checkSDKInit) userInfo:nil repeats:NO];
    [[NSRunLoop currentRunLoop] addTimer:SDKInitCheckTimer forMode:NSRunLoopCommonModes];
}


+ (void)checkSDKInit {
    if ([[OMConfig sharedInstance].appKey length]>0 && ![OpenMediation isInitialized] && [OMNetMonitor sharedInstance].netStatus) {
        [self initWithAppKey:[OMConfig sharedInstance].appKey baseHost:[OMConfig sharedInstance].baseHost];
    }
}

+ (void)initWithAppKey:(NSString*)appKey baseHost:(NSString*)host completionHandler:(initCompletionHandler)completionHandler {
    OMConfig *config = [OMConfig sharedInstance];
    if (config.initState == OMInitStateInitializing || [config initSuccess]) {
        if ([config initSuccess]) {
            completionHandler(nil);
        }
        return;
    }
    OMLogI(@"OpenMediation SDK init Version %@",OPENMEDIATION_SDK_VERSION);
    [[OMNetMonitor sharedInstance] startMonitor];
    [OMInitRequest configureWithAppKey:appKey baseHost:host completionHandler:^(NSError *error) {
        if (!error) {
            [self settingWithConfig];
            [self sendConversionData];
            [[OMEventManager sharedInstance]addEvent:INIT_COMPLETE extraData:nil];
            completionHandler(nil);
        } else {
            [[OMEventManager sharedInstance]addEvent:INIT_FAILED extraData:@{@"msg":OM_SAFE_STRING(error.localizedDescription)}];
            OMLogI(@"OpenMediation SDK init error: %@",error.localizedDescription);
            completionHandler(error);
        }
    }];
}

+ (void)reinit {
    [OMInitRequest configureWithAppKey:[OMConfig sharedInstance].appKey baseHost:[OMConfig sharedInstance].baseHost completionHandler:^(NSError *error) {
        if (!error) {
            [[OMEventManager sharedInstance]addEvent:REINIT_COMPLETE extraData:nil];
            OMLogI(@"OpenMediation SDK reinit success");
        } else {
            [[OMEventManager sharedInstance]addEvent:REINIT_FAILED extraData:nil];
            OMLogI(@"OpenMediation SDK reinit error: %@",error.localizedDescription);
        }
    }];
}

+ (void)settingWithConfig {
    OMConfig *config = [OMConfig sharedInstance];
    if (config.openDebug) {
        [OMLogMoudle setDebugMode];
    }

    [[OMCrashHandle sharedInstance]sendCrashLog];
    if (!OM_STR_EMPTY(config.erUrl)) {
        [[OMCrashHandle sharedInstance]install];
    }

}

+ (void)sendConversionData {
    if ([OMConfig sharedInstance].conversionData)   {
        NSArray *allKeys = [OMConfig sharedInstance].conversionData.allKeys;
        for (NSNumber *type in allKeys) {
            [OMCDRequest postWithType:[type integerValue] data:[[OMConfig sharedInstance].conversionData objectForKey:type] completionHandler:^(NSDictionary * _Nullable object, NSError * _Nullable error) {
                if (!error) {
                    OMLogD(@"send af conversion data success");
                }
            }];
        }
        [OMConfig sharedInstance].conversionData = [NSMutableDictionary dictionary];
    }
    
}

/// Check that `OpenMediation` has been initialized
+ (BOOL)isInitialized {
    return [OMConfig sharedInstance].initSuccess;
}

#pragma mark - Segments
/// user in-app purchase
+ (void)userPurchase:(CGFloat)amount currency:(NSString*)currencyUnit {

     [[OMUserData sharedInstance]userPurchase:amount currency:currencyUnit];
}

+ (void)setUserAge:(NSInteger)userAge {
    [[OMUserData sharedInstance] setUserAge:userAge];
    //pass user age to adn
    OMConfig *config = [OMConfig sharedInstance];
    for (NSString *adnID in config.adnAppkeyMap) {
        
        Class adapterClass = [[OMMediations sharedInstance] adnAdapterClass:[adnID integerValue]];
        
        if (adapterClass && [adapterClass respondsToSelector:@selector(setUserAge:)]) {
            [adapterClass setUserAge:[OMUserData sharedInstance].userAge];
        }
    }

}

+ (void)setUserGender:(OMGender)userGender {
    [[OMUserData sharedInstance] setUserGender:(NSInteger)userGender];
    
    //pass user gender to adn
    OMConfig *config = [OMConfig sharedInstance];
    for (NSString *adnID in config.adnAppkeyMap) {
        
        Class adapterClass = [[OMMediations sharedInstance] adnAdapterClass:[adnID integerValue]];
        
        if (adapterClass && [adapterClass respondsToSelector:@selector(setUserGender:)]) {
            [adapterClass setUserGender:[OMUserData sharedInstance].userGender];
        }
    }
}


+ (void)setUserID:(NSString*)userID {
    if([userID isKindOfClass:[NSString class]] && userID.length>0) {
        [OMUserData sharedInstance].customUserID = userID;
    }
}

+ (NSString*)getUserID {
    return [OMUserData sharedInstance].customUserID;
}

+ (void)setCustomTag:(NSString*)tag withString:(NSString*)value {
    if (([tag isKindOfClass:[NSString class]] && tag.length>0) && ([value isKindOfClass:[NSString class]] && value.length >0)) {
        [self setTag:tag value:value];
    } else {
        OMLogE(@"Tag or value is invalid");
    }
}

+ (void)setCustomTag:(NSString*)tag withNumber:(NSNumber*)value {
    if (([tag isKindOfClass:[NSString class]] && tag.length>0) && [value isKindOfClass:[NSNumber class]]) {
        [self setTag:tag value:value];
    } else {
        OMLogE(@"Tag or value is invalid");
    }
}

+ (void)setCustomTag:(NSString*)tag withStrings:(NSArray *)values {
    if (([tag isKindOfClass:[NSString class]] && tag.length>0) && ([values isKindOfClass:[NSArray class]] && values.count >0 )) {
        [self setTag:tag value:values];
    } else {
        OMLogE(@"Tag or value is invalid");
    }
}

+ (void)setCustomTag:(NSString*)tag withNumbers:(NSArray *)values {
    if ([tag isKindOfClass:[NSString class]] && ([values isKindOfClass:[NSArray class]] && values.count >0 )) {
        [self setTag:tag value:values];
        
    } else {
        OMLogE(@"Tag or value is invalid");
    }
}

+ (void)setTag:(NSString*)tag value:(id)value {
    if (tag.length > TagMaxLength) {
        OMLogE(@"The tag is too long");
        return;
    }

    if ([value isKindOfClass:[NSString class]] && ((NSString*)value).length > TagMaxLength ) {
        OMLogE(@"The value is too long");
        return;
    }
    
    if ([value isKindOfClass:[NSArray class]]) {
        for (NSString*str in value) {
            if ([str isKindOfClass:[NSString class]] && str.length > TagMaxLength ) {
                OMLogE(@"The value is too long");
                return;
            }
        }
    }
    
    OMUserData *userData = [OMUserData sharedInstance];
    
    @synchronized (userData) {
        if (userData.tags.count >= 10) {
            OMLogE(@"The number of tags reaches maximum");
            return;
        }
        userData.tags[tag] = value;
    }
}

+ (void)removeTag:(NSString*)tag {
    OMUserData *userData = [OMUserData sharedInstance];
    @synchronized (userData) {
        [userData.tags removeObjectForKey:tag];
    }
}

+ (NSDictionary*)allCustomTags {
    return [[OMUserData sharedInstance].tags copy];
}

#pragma mark - ROAS
/// calculate each Media Source, Campaign level ROAS, and LTV data
+ (void)sendAFConversionData:(NSDictionary*)conversionInfo {
    if ([self isInitialized]) {
        [OMCDRequest postWithType:0 data:conversionInfo completionHandler:^(NSDictionary * _Nullable object, NSError * _Nullable error) {
            if (!error) {
                OMLogD(@"send af conversion data success");
            }
        }];
    } else {
        if (![OMConfig sharedInstance].conversionData) {
            [OMConfig sharedInstance].conversionData = [NSMutableDictionary dictionary];
        }
        [[OMConfig sharedInstance].conversionData setObject:conversionInfo forKey:@"0"];
    }
}

+ (void)sendAFDeepLinkData:(NSDictionary*)attributionData {
    if ([self isInitialized]) {
        [OMCDRequest postWithType:1 data:attributionData completionHandler:^(NSDictionary * _Nullable object, NSError * _Nullable error) {
            if (!error) {
                OMLogD(@"send af deep link data success");
            }
        }];
    } else {
        if (![OMConfig sharedInstance].conversionData) {
            [OMConfig sharedInstance].conversionData = [NSMutableDictionary dictionary];
        }
        [[OMConfig sharedInstance].conversionData setObject:attributionData forKey:@"1"];
    }
}

#pragma mark - ImpressionData
+ (void)addImpressionDataDelegate:(id<OMImpressionDataDelegate>)delegate {
    [[OMImpressionDataRouter sharedInstance]addDelegate:delegate];
}

///Remove Impression Data delegate
+ (void)rmoveImpressionDataDelegate:(id<OMImpressionDataDelegate>)delegate {
    [[OMImpressionDataRouter sharedInstance]removeDelegate:delegate];
}

#pragma mark - GDPR/CCPA/COPPA
+ (void)setGDPRConsent:(BOOL)consent {
    [[OMUserData sharedInstance] setConsent:consent];
    [[NSUserDefaults standardUserDefaults] setBool:consent forKey:@"OMConsentStatus"];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

+ (OMConsentStatus)currentConsentStatus {
    if (OM_IS_NULL([[NSUserDefaults standardUserDefaults] stringForKey:@"OMConsentStatus"])) {
        return OMConsentStatusUnknown;
    }else if ([[NSUserDefaults standardUserDefaults] boolForKey:@"OMConsentStatus"] == YES) {
        return OMConsentStatusConsented;
    }else{
        return OMConsentStatusDenied;
    }
}

+ (void)setUSPrivacyLimit:(BOOL)privacyLimit {
    [[OMUserData sharedInstance] setUSPrivacy:privacyLimit];
    
}

+ (void)setUserAgeRestricted:(BOOL)restricted {
    [[OMUserData sharedInstance] setUserAgeRestricted:restricted];
}

#pragma mark - Debug
/// current SDK version
+ (NSString *)SDKVersion {
    return OPENMEDIATION_SDK_VERSION;
}

/// A tool to verify a successful integration of the OpenMediation SDK and any additional adapters.
+ (void)validateIntegration{
    [OMMediations validateIntegration];
}

/// log enable,default is YES
+ (void)setLogEnable:(BOOL)logEnable {
    [OMLogMoudle openLog:logEnable];
}

+ (void)setAutoCache:(BOOL)autoCache {
    [OMConfig sharedInstance].autoCache = autoCache;
}

+ (NSArray*)cachedPlacementIds:(NSString*)filter {
    return [[OMConfig sharedInstance] cachedPlacementIds:filter];
}

@end
