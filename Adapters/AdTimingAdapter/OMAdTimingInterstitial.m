// Copyright 2020 ADTIMING TECHNOLOGY COMPANY LIMITED
// Licensed under the GNU Lesser General Public License Version 3

#import "OMAdTimingInterstitial.h"

@implementation OMAdTimingInterstitial

- (instancetype)initWithParameter:(NSDictionary*)adParameter {
    if (self = [super init]) {
        _pid = [adParameter objectForKey:@"pid"];
    }
    return self;
}

- (void)loadAdWithBidPayload:(NSString *)bidPayload {
    Class interstitialClass = NSClassFromString(@"AdTimingBidInterstitial");
    if (interstitialClass && [interstitialClass respondsToSelector:@selector(sharedInstance)] && [interstitialClass instancesRespondToSelector:@selector(loadWithPlacementID:)] && [_pid length]>0) {
        [[interstitialClass sharedInstance]loadWithPlacementID:_pid payLoad:bidPayload];
        if (interstitialClass && [interstitialClass respondsToSelector:@selector(sharedInstance)] && [interstitialClass instancesRespondToSelector:@selector(addDelegate:)]) {
            [[interstitialClass sharedInstance]addDelegate:self];
        }
    }
}

- (BOOL)isReady {
    BOOL isReady = NO;
    Class interstitialClass = NSClassFromString(@"AdTimingBidInterstitial");
    if (interstitialClass && [interstitialClass respondsToSelector:@selector(sharedInstance)] && [interstitialClass instancesRespondToSelector:@selector(isReady:)] && [_pid length]>0) {
        isReady = [[interstitialClass sharedInstance]isReady:_pid];
    }
    return isReady;
}

- (void)show:(UIViewController *)vc {
    Class interstitialClass = NSClassFromString(@"AdTimingBidInterstitial");
    if ([self isReady]) {
        if (interstitialClass && [interstitialClass respondsToSelector:@selector(sharedInstance)] && [interstitialClass instancesRespondToSelector:@selector(showAdFromRootViewController:placementID:)]) {
            [[interstitialClass sharedInstance]showAdFromRootViewController:vc placementID:_pid];
        }
    }
}

#pragma mark - AdTimingMediatedInterstitialDelegate


- (void)AdTimingBidInterstitialDidLoad:(NSString *)placementID {
    if ([placementID isEqualToString:_pid]) {
        if (_delegate && [_delegate respondsToSelector:@selector(customEvent:didLoadAd:)]) {
            [_delegate customEvent:self didLoadAd:nil];
        }
    }
}

- (void)AdTimingBidInterstitialDidFailToLoad:(NSString *)placementID withError:(NSError *)error {
    if ([placementID isEqualToString:_pid] && _delegate && [_delegate respondsToSelector:@selector(customEvent:didFailToLoadWithError:)]) {
        [_delegate customEvent:self didFailToLoadWithError:error];
    }
}

- (void)AdTimingBidInterstitialDidOpen:(NSString*)placementID {
    if ([placementID isEqualToString:_pid] && _delegate && [_delegate respondsToSelector:@selector(interstitialCustomEventDidOpen:)]) {
        [_delegate interstitialCustomEventDidOpen:self];
    }
}

- (void)AdTimingBidInterstitialDidShow:(NSString*)placementID {
    if ([placementID isEqualToString:_pid] && _delegate && [_delegate respondsToSelector:@selector(interstitialCustomEventDidShow:)]) {
        [_delegate interstitialCustomEventDidShow:self];
    }
}

- (void)AdTimingBidInterstitialDidClick:(NSString*)placementID {
    if ([placementID isEqualToString:_pid] && _delegate && [_delegate respondsToSelector:@selector(interstitialCustomEventDidClick:)]) {
        [_delegate interstitialCustomEventDidClick:self];
    }
}

- (void)AdTimingBidInterstitialDidClose:(NSString*)placementID {
    if ([placementID isEqualToString:_pid] && _delegate && [_delegate respondsToSelector:@selector(interstitialCustomEventDidClose:)]) {
        [_delegate interstitialCustomEventDidClose:self];
    }
}

- (void)AdTimingBidInterstitialDidFailToShow:(NSString*)placementID withError:(NSError *)error {
    if ([placementID isEqualToString:_pid] && _delegate && [_delegate respondsToSelector:@selector(interstitialCustomEventDidFailToShow:error:)]) {
        [_delegate interstitialCustomEventDidFailToShow:self error:error];
    }
}

@end
