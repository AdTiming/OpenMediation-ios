// Copyright 2020 ADTIMING TECHNOLOGY COMPANY LIMITED
// Licensed under the GNU Lesser General Public License Version 3

#import <Foundation/Foundation.h>
#import "OMBidCustomEvent.h"
#import "OMInMobiClass.h"

NS_ASSUME_NONNULL_BEGIN

@interface OMInMobiBid : NSObject<OMBidCustomEvent>

+ (void)bidWithNetworkItem:(OMBidNetworkItem*)networkItem adFormat:(NSString*)format adSize:(CGSize)size responseCallback:(void(^)(NSDictionary *bidResponseData))callback;
@end

NS_ASSUME_NONNULL_END
