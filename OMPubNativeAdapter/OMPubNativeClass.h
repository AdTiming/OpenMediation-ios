// Copyright 2020 ADTIMING TECHNOLOGY COMPANY LIMITED
// Licensed under the GNU Lesser General Public License Version 3

#ifndef OMPubNativeClass_h
#define OMPubNativeClass_h
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

typedef void (^HyBidCompletionBlock)(BOOL);

typedef enum {
    BANNER_POSITION_UNKNOWN,
    BANNER_POSITION_TOP,
    BANNER_POSITION_BOTTOM
} HyBidBannerPosition;

typedef enum {
    HyBidLogLevelNone,
    HyBidLogLevelError,
    HyBidLogLevelWarning,
    HyBidLogLevelInfo,
    HyBidLogLevelDebug,
} HyBidLogLevel;

@interface HyBid : NSObject

+ (void)setCoppa:(BOOL)enabled;
+ (void)setTestMode:(BOOL)enabled;
+ (void)initWithAppToken:(NSString *)appToken completion:(HyBidCompletionBlock)completion;
+ (void)setLocationUpdates:(BOOL)enabled;
+ (void)setLocationTracking:(BOOL)enabled;
+ (void)setAppStoreAppID:(NSString *)appID;
+ (NSString *)sdkVersion;

@end


@interface HyBidAdSize: NSObject

@property (nonatomic, assign, readonly) NSInteger width;
@property (nonatomic, assign, readonly) NSInteger height;
@property (nonatomic, strong, readonly) NSString *layoutSize;

@property (class, nonatomic, readonly) HyBidAdSize *SIZE_320x50;
@property (class, nonatomic, readonly) HyBidAdSize *SIZE_300x250;
@property (class, nonatomic, readonly) HyBidAdSize *SIZE_300x50;
@property (class, nonatomic, readonly) HyBidAdSize *SIZE_320x480;
@property (class, nonatomic, readonly) HyBidAdSize *SIZE_1024x768;
@property (class, nonatomic, readonly) HyBidAdSize *SIZE_768x1024;
@property (class, nonatomic, readonly) HyBidAdSize *SIZE_728x90;
@property (class, nonatomic, readonly) HyBidAdSize *SIZE_160x600;
@property (class, nonatomic, readonly) HyBidAdSize *SIZE_250x250;
@property (class, nonatomic, readonly) HyBidAdSize *SIZE_300x600;
@property (class, nonatomic, readonly) HyBidAdSize *SIZE_320x100;
@property (class, nonatomic, readonly) HyBidAdSize *SIZE_480x320;
@property (class, nonatomic, readonly) HyBidAdSize *SIZE_INTERSTITIAL;
@property (class, nonatomic, readonly) HyBidAdSize *SIZE_NATIVE;

- (BOOL)isEqualTo:(HyBidAdSize *)hyBidAdSize;

@end

@interface HyBidAd : NSObject

@property (nonatomic, readonly) NSNumber *eCPM;

@end


@class HyBidAd;
@class HyBidAdRequest;

@protocol HyBidAdRequestDelegate <NSObject>

- (void)requestDidStart:(HyBidAdRequest *)request;
- (void)request:(HyBidAdRequest *)request didLoadWithAd:(HyBidAd *)ad;
- (void)request:(HyBidAdRequest *)request didFailWithError:(NSError *)error;

@end

@class HyBidAdPresenter;

@protocol HyBidAdPresenterDelegate<NSObject>

- (void)adPresenter:(HyBidAdPresenter *)adPresenter
      didLoadWithAd:(UIView *)adView;
- (void)adPresenterDidClick:(HyBidAdPresenter *)adPresenter;
- (void)adPresenter:(HyBidAdPresenter *)adPresenter
       didFailWithError:(NSError *)error;

@end

@class HyBidSignalDataProcessor, HyBidAd;

@protocol HyBidSignalDataProcessorDelegate<NSObject>

- (void)signalDataDidFinishWithAd:(HyBidAd *)ad;
- (void)signalDataDidFailWithError:(NSError *)error;

@end

@class HyBidAdSize;

@class HyBidAdView;

@protocol HyBidAdViewDelegate<NSObject>

- (void)adViewDidLoad:(HyBidAdView *)adView;
- (void)adView:(HyBidAdView *)adView didFailWithError:(NSError *)error;
- (void)adViewDidTrackImpression:(HyBidAdView *)adView;
- (void)adViewDidTrackClick:(HyBidAdView *)adView;

@end

@interface HyBidAdView : UIView <HyBidAdRequestDelegate, HyBidAdPresenterDelegate>

@property (nonatomic, strong) HyBidAdRequest *adRequest;
@property (nonatomic, strong) HyBidAd *ad;
@property (nonatomic, weak) NSObject <HyBidAdViewDelegate> *delegate;
@property (nonatomic, assign) BOOL isMediation;
@property (nonatomic, strong) HyBidAdSize *adSize;
@property (nonatomic, assign) BOOL autoShowOnLoad;
@property (nonatomic) HyBidBannerPosition bannerPosition;
@property (nonatomic, assign) BOOL isAutoCacheOnLoad;

- (instancetype)initWithSize:(HyBidAdSize *)adSize NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithCoder:(NSCoder *)coder NS_UNAVAILABLE;
- (instancetype)initWithFrame:(CGRect)frame NS_UNAVAILABLE;
- (void)loadWithZoneID:(NSString *)zoneID andWithDelegate:(NSObject<HyBidAdViewDelegate> *)delegate;
- (void)loadWithZoneID:(NSString *)zoneID withPosition:(HyBidBannerPosition)bannerPosition andWithDelegate:(NSObject<HyBidAdViewDelegate> *)delegate;
- (void)setupAdView:(UIView *)adView;
- (void)renderAd;
- (void)renderAdWithContent:(NSString *)adContent withDelegate:(NSObject<HyBidAdViewDelegate> *)delegate;
- (void)startTracking;
- (void)stopTracking;
- (void)prepare;
- (void)show;
- (HyBidAdPresenter *)createAdPresenter;


@end

@class HyBidContentInfoView;
@class HyBidNativeAdRenderer;


typedef enum {
    TWO_DECIMAL_PLACES,
    THREE_DECIMAL_PLACES,
} HyBidKeywordMode;

@interface HyBidHeaderBiddingUtils : NSObject

+ (NSString *)eCPMFromAd:(HyBidAd *)ad withDecimalPlaces:(HyBidKeywordMode)decimalPlaces;

@end

@interface HyBidLogger : NSObject

// Method to filter logging with the level passed as the paramter
+ (void)setLogLevel:(HyBidLogLevel)logLevel;

@end

NS_ASSUME_NONNULL_END

#endif /* OMPubNativeClass_h */
