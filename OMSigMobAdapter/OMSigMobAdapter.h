// Copyright 2020 ADTIMING TECHNOLOGY COMPANY LIMITED
// Licensed under the GNU Lesser General Public License Version 3

#import <Foundation/Foundation.h>
#import "OMMediationAdapter.h"
#import "OMSigMobClass.h"

NS_ASSUME_NONNULL_BEGIN

static NSString * const SigMobAdapterVersion = @"2.0.7";

@interface OMSigMobAdapter : NSObject<OMMediationAdapter>

+ (NSString*)adapterVerison;
+ (void)initSDKWithConfiguration:(NSDictionary *)configuration completionHandler:(OMMediationAdapterInitCompletionBlock)completionHandler;

+(void)setConsent:(BOOL)consent;
+(void)setUserAge:(NSInteger)userAge;
+(void)setUserAgeRestricted:(BOOL)restricted;
@end

NS_ASSUME_NONNULL_END
