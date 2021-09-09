// Copyright 2020 ADTIMING TECHNOLOGY COMPANY LIMITED
// Licensed under the GNU Lesser General Public License Version 3

#import "OMBid.h"
#import "OMBidNetworkItem.h"
#import "OMBidResponse.h"
#import "OMEventManager.h"
#import "OMToolUmbrella.h"
#import "OMMediations.h"

@implementation OMBid

- (void)bidWithNetworkItems:(NSArray*)networkItems adFormat:(NSString*)format adSize:(CGSize)size {
    _bidNetworkItems = networkItems;
    _bidResponses = [NSMutableDictionary dictionary];
    _bidding = YES;
    
    NSInteger bidMaxTimeOut = 0;
    
    __weak __typeof(self) weakSelf = self;
    dispatch_group_t group = dispatch_group_create();

    for (OMBidNetworkItem *networkItem in networkItems) {
        if (bidMaxTimeOut < networkItem.maxTimeOutMS) {
            bidMaxTimeOut = networkItem.maxTimeOutMS;
        }
        
        void(^bidBlock)(void) = ^(void) {
            if (weakSelf && [weakSelf.delegate respondsToSelector:@selector(omBidRequest:)]) {
                [weakSelf.delegate omBidRequest:networkItem.extraData[@"instanceID"]];
            }
            NSString *className = [NSString stringWithFormat:@"OM%@Bid",networkItem.adnName];
            Class bidClass = NSClassFromString(className);
            if (bidClass && [bidClass respondsToSelector:@selector(bidWithNetworkItem:adFormat:adSize:responseCallback:)]) {
                [bidClass bidWithNetworkItem:networkItem adFormat:format adSize:size responseCallback:^(NSDictionary *bidResponseData) {
                        if (weakSelf && bidResponseData) {
                            @synchronized (weakSelf) {
                                OMBidResponse *bidResponse = [OMBidResponse buildResponseWithData:bidResponseData];

                                [weakSelf.bidResponses setObject:bidResponse forKey:networkItem.extraData[@"instanceID"] ];
                            }
                        }
                        dispatch_group_leave(group);
                    }];
            } else {
                dispatch_group_leave(group);
            }
        };
        
        if (!OM_STR_EMPTY(networkItem.adnName)) {
            dispatch_group_enter(group);
            OMAdNetwork adnID = [networkItem.extraData[@"adnID"] integerValue];
            if (![[OMMediations sharedInstance]adnSDKInitialized:adnID]) {
                [[OMMediations sharedInstance]initAdNetworkSDKWithId:adnID
                                                       completionHandler:^(NSError * _Nullable error) {
                    if (!error) {
                        bidBlock();
                    } else {
                        OMBidResponse *bidResponse = [OMBidResponse buildResponseWithError:[NSString stringWithFormat:@"%@ init failed %@",networkItem.adnName,OM_SAFE_STRING(error.localizedDescription)]];

                        [weakSelf.bidResponses setObject:bidResponse forKey:networkItem.extraData[@"instanceID"] ];
                        dispatch_group_leave(group);
                    }
                }];
            } else {
                bidBlock();
            }
            

        }
    }
    
    if (_bidTimer) {
        [_bidTimer invalidate];
    }
    if (bidMaxTimeOut >0) {
        _bidTimer = [NSTimer scheduledTimerWithTimeInterval:(bidMaxTimeOut/1000.0) target:[OMWeakObject proxyWithTarget:self] selector:@selector(bidTimeOut) userInfo:nil repeats:NO];
        [[NSRunLoop currentRunLoop] addTimer:self.bidTimer forMode:NSRunLoopCommonModes];
    }
    
    dispatch_group_notify(group, dispatch_get_main_queue(), ^{
        if (weakSelf) {
            [weakSelf bidComplete];
        }
    });
}


- (void)bidTimeOut {
    @synchronized (self) {
            for (OMBidNetworkItem *networkItem in _bidNetworkItems) {
            if (![_bidResponses objectForKey:networkItem.extraData[@"instanceID"]]) {
                OMBidResponse *bidResponse = [OMBidResponse buildResponseWithError:@"Bid time out"];
                [_bidResponses setObject:bidResponse forKey:networkItem.extraData[@"instanceID"]];
            }
        }
    }
    [self bidComplete];
}

- (void)bidComplete {
    
    @synchronized (self) {
            if (_bidTimer) {
            [_bidTimer invalidate];
            _bidTimer = nil;
        }
        if (self.bidding && self.delegate && [self.delegate respondsToSelector:@selector(omBidComplete:)]) {
            self.bidding = NO;
            [self.delegate omBidComplete:[self.bidResponses copy]];
        }
    }

}

@end
