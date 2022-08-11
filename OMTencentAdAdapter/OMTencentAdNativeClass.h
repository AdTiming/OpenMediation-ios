// Copyright 2020 ADTIMING TECHNOLOGY COMPANY LIMITED
// Licensed under the GNU Lesser General Public License Version 3

#ifndef OMTencentAdNativeClass_h
#define OMTencentAdNativeClass_h
#import <UIKit/UIKit.h>
#import "OMTencentAdClass.h"

@class GDTLogoView;
@class GDTMediaView;
@class GDTVideoConfig;
@class GDTUnifiedNativeAdView;
@class GDTSDKDefines;

#if defined(__has_attribute)
#if __has_attribute(deprecated)
#define GDT_DEPRECATED_MSG_ATTRIBUTE(s) __attribute__((deprecated(s)))
#define GDT_DEPRECATED_ATTRIBUTE __attribute__((deprecated))
#else
#define GDT_DEPRECATED_MSG_ATTRIBUTE(s)
#define GDT_DEPRECATED_ATTRIBUTE
#endif
#else
#define GDT_DEPRECATED_MSG_ATTRIBUTE(s)
#define GDT_DEPRECATED_ATTRIBUTE
#endif

#define GDTScreenHeight ([UIScreen mainScreen].bounds.size.height)
#define GDTScreenWidth  ([UIScreen mainScreen].bounds.size.width)
#define GDTTangramSchemePrefix  @"gdtmsg://e.qq.com/"


typedef NS_ENUM(NSInteger, GDTVastAdEventType) {
    GDTVastAdEventTypeUnknow,
    GDTVastAdEventTypeLoaded,
    GDTVastAdEventTypeStarted,
    GDTVastAdEventTypeFirstQuartile,
    GDTVastAdEventTypeMidPoint,
    GDTVastAdEventTypeThirdQuartile,
    GDTVastAdEventTypeComplete,
    GDTVastAdEventTypeAllAdsComplete,
    GDTVastAdEventTypeExposed,
    GDTVastAdEventTypeClicked,
};


typedef NS_ENUM(NSUInteger, GDTVideoPlayPolicy) {
    GDTVideoPlayPolicyUnknow = 0, // 默认值，未设置
    GDTVideoPlayPolicyAuto = 1,   // 用户角度看起来是自动播放
    GDTVideoPlayPolicyManual = 2  // 用户角度看起来是手动播放或点击后播放
};

typedef NS_ENUM(NSUInteger, GDTVideoRenderType) {
    GDTVideoRenderTypeUnknow = 0,
    GDTVideoRenderTypeSDK = 1,
    GDTVideoRenderTypeDeveloper = 2
};

NS_ASSUME_NONNULL_BEGIN


@interface GDTUnifiedNativeAdDataObject : NSObject <GDTAdProtocol>

/**
 广告标题
 */
@property (nonatomic, copy, readonly) NSString *title;

/**
 广告描述
 */
@property (nonatomic, copy, readonly) NSString *desc;

/**
 素材宽度，单图广告代表大图 imageUrl 宽度、多图广告代表小图 mediaUrlList 宽度
 */
@property (nonatomic, readonly) NSInteger imageWidth;

/**
 素材高度，单图广告代表大图 imageUrl 高度、多图广告代表小图 mediaUrlList 高度
 */
@property (nonatomic, readonly) NSInteger imageHeight;

/**
 应用类广告App 图标Url
 */
@property (nonatomic, copy, readonly) NSString *iconUrl;

/**
 广告大图Url, 建议使用 bindImageViews:placeholder: 方法替代
 */
@property (nonatomic, copy, readonly) NSString *imageUrl;

/**
 三小图广告的图片Url集合, 建议使用 bindImageViews:placeholder: 方法替代
 */
@property (nonatomic, copy, readonly) NSArray *mediaUrlList;

/**
 应用类广告的星级（5星制度）
 */
@property (nonatomic, readonly) CGFloat appRating;

/**
 应用类广告的价格
 */
@property (nonatomic, strong, readonly) NSNumber *appPrice;

/**
 是否为应用类广告
 */
@property (nonatomic, readonly) BOOL isAppAd;

/**
 是否为视频广告
 */
@property (nonatomic, readonly) BOOL isVideoAd;

/**
 是否为三小图广告
 */
@property (nonatomic, readonly) BOOL isThreeImgsAd;

/**
 是否为微信原生页广告 (可针对此广告类型来控制按钮展示文案为"去微信看看")
 */
@property (nonatomic, readonly) BOOL isWechatCanvasAd;

/**
 返回广告的eCPM，单位：分
 
 @return 成功返回一个大于等于0的值，-1表示无权限或后台出现异常
 */
@property (nonatomic, readonly) NSInteger eCPM;

/**
 返回广告的eCPM等级
 
 @return 成功返回一个包含数字的string，@""或nil表示无权限或后台异常
 */
@property (nonatomic, readonly) NSString *eCPMLevel;

/**
 广告对应的按钮展示文案
 此字段可能为空
 */
@property (nonatomic, readonly) NSString *buttonText;

/**
 广告对应的CTA文案，自定义CTA视图时建议使用此字段
 广告对应的callToAction文案，比如“立即预约”或“电话咨询”, 自定义callToAction视图时建议使用此字段

 该字段在部分广告类型中可能为空
 */
@property (nonatomic, readonly) NSString *callToAction;

/**
返回广告是否可以跳过，用于做前贴片场景

@return YES 表示可跳过、NO 表示不可跳过
*/
@property (nonatomic, readonly) BOOL skippable;

/**
 视频广告播放配置
 */
@property (nonatomic, strong) GDTVideoConfig *videoConfig;

/**
 * 视频广告时长，单位 ms
 */
@property (nonatomic, readonly) CGFloat duration;

/**
 *  VAST Tag Url，可能为空。
 */
@property (nonatomic, copy, readonly) NSString *vastTagUrl;

/**
 * VAST Content，可能为空。
 */
@property (nonatomic, copy, readonly) NSString *vastContent;

/**
 * 是否为 VAST 广告
 */
@property (nonatomic, assign, readonly) BOOL isVastAd;

/**
 *  广告是否有效，以下情况会返回NO，建议在展示广告之前判断，否则会影响计费或展示失败
 *  a.广告过期
 */
@property (nonatomic, readonly) BOOL isAdValid;

/**
 判断两个自渲染2.0广告数据是否相等

 @param dataObject 需要对比的自渲染2.0广告数据对象
 @return YES or NO
 */
- (BOOL)equalsAdData:(GDTUnifiedNativeAdDataObject *)dataObject;

/**
 * 绑定展示的图片视图
 *
 * @param imageViews     进行渲染的 imageView
 * @param placeholder     图片加载过程中的占位图
 */
- (void)bindImageViews:(NSArray<UIImageView *> *)imageViews placeholder:(UIImage *)placeholder;

@end

@class GDTUnifiedNativeAdView;

//视频广告时长Key
extern NSString* const kGDTUnifiedNativeAdKeyVideoDuration;

@protocol GDTUnifiedNativeAdViewDelegate <GDTAdDelegate>

@optional
/**
 广告曝光回调

 @param unifiedNativeAdView GDTUnifiedNativeAdView 实例
 */
- (void)gdt_unifiedNativeAdViewWillExpose:(GDTUnifiedNativeAdView *)unifiedNativeAdView;


/**
 广告点击回调

 @param unifiedNativeAdView GDTUnifiedNativeAdView 实例
 */
- (void)gdt_unifiedNativeAdViewDidClick:(GDTUnifiedNativeAdView *)unifiedNativeAdView;


/**
 广告详情页关闭回调

 @param unifiedNativeAdView GDTUnifiedNativeAdView 实例
 */
- (void)gdt_unifiedNativeAdDetailViewClosed:(GDTUnifiedNativeAdView *)unifiedNativeAdView;


/**
 当点击应用下载或者广告调用系统程序打开时调用
 
 @param unifiedNativeAdView GDTUnifiedNativeAdView 实例
 */
- (void)gdt_unifiedNativeAdViewApplicationWillEnterBackground:(GDTUnifiedNativeAdView *)unifiedNativeAdView;


/**
 广告详情页面即将展示回调

 @param unifiedNativeAdView GDTUnifiedNativeAdView 实例
 */
- (void)gdt_unifiedNativeAdDetailViewWillPresentScreen:(GDTUnifiedNativeAdView *)unifiedNativeAdView;


/**
 视频广告播放状态更改回调

 @param nativeExpressAdView GDTUnifiedNativeAdView 实例
 @param status 视频广告播放状态
 @param userInfo 视频广告信息
 */
- (void)gdt_unifiedNativeAdView:(GDTUnifiedNativeAdView *)unifiedNativeAdView playerStatusChanged:(GDTMediaPlayerStatus)status userInfo:(NSDictionary *)userInfo;

@end

@interface GDTUnifiedNativeAdView:UIView

/**
 绑定的数据对象
 */
@property (nonatomic, strong, readonly) GDTUnifiedNativeAdDataObject *dataObject;

/**
 视频广告的媒体View，绑定数据对象后自动生成，可自定义布局
 */
@property (nonatomic, strong, readonly) GDTMediaView *mediaView;

/**
 腾讯广告 LogoView，自动生成，可自定义布局
 */
@property (nonatomic, strong, readonly) GDTLogoView *logoView;

/**
 广告 View 时间回调对象
 */
@property (nonatomic, weak) id<GDTUnifiedNativeAdViewDelegate> delegate;

/*
 *  viewControllerForPresentingModalView
 *  详解：开发者需传入用来弹出目标页的ViewController，一般为当前ViewController
 */
@property (nonatomic, weak) UIViewController *viewController;

/**
 自渲染2.0视图注册方法
 
 调用方法之前请先判断[dataObject isAdValid]是否为YES，当为NO时调用不生效
 
 @param dataObject 数据对象，必传字段
 @param clickableViews 可点击的视图数组，此数组内的广告元素才可以响应广告对应的点击事件
 */
- (void)registerDataObject:(GDTUnifiedNativeAdDataObject *)dataObject
            clickableViews:(NSArray<UIView *> *)clickableViews;


/**
 自渲染2.0视图注册方法
 
 调用方法之前请先判断[dataObject isAdValid]是否为YES，当为NO时调用不生效
 
 @param dataObject 数据对象，必传字段
 @param clickableViews 可点击的视图数组，此数组内的广告元素才可以响应广告对应的点击事件
 @param customClickableViews 可点击的视图数组，与clickableViews的区别是：在视频广告中当dataObject中的videoConfig的detailPageEnable为YES时，点击后直接进落地页而非视频详情页，除此条件外点击行为与clickableViews保持一致
 */
- (void)registerDataObject:(GDTUnifiedNativeAdDataObject *)dataObject
            clickableViews:(NSArray<UIView *> *)clickableViews customClickableViews:(NSArray <UIView *> *)customClickableViews;

/**
 注册可点击的callToAction视图的方法
 建议开发者使用GDTUnifiedNativeAdDataObject中的callToAction字段来创建视图，并取代自定义的下载或打开等button,
 调用此方法之前必须先调用registerDataObject:clickableViews
 @param callToActionView CTA视图, 系统自动处理点击事件
 */
- (void)registerClickableCallToActionView:(UIView *)callToActionView;

/**
 注销数据对象，在 tableView、collectionView 等场景需要复用 GDTUnifiedNativeAdView 时，
 需要在合适的时机，例如 cell 的 prepareForReuse 方法内执行 unregisterDataObject 方法，
 将广告对象与 GDTUnifiedNativeAdView 解绑，具体可参考示例 demo 的 UnifiedNativeAdBaseTableViewCell 类
 */
- (void)unregisterDataObject;

//#pragma mark - DEPRECATED
///**
// 此方法已经废弃
// 自渲染2.0视图注册方法
//
// @param dataObject 数据对象，必传字段
// @param logoView logo视图
// @param viewController 所在ViewController，必传字段。支持在register之后对其进行修改
// @param clickableViews 可点击的视图数组，此数组内的广告元素才可以响应广告对应的点击事件
// */
//- (void)registerDataObject:(GDTUnifiedNativeAdDataObject *)dataObject
//                  logoView:(GDTLogoView *)logoView
//            viewController:(UIViewController *)viewController
//            clickableViews:(NSArray<UIView *> *)clickableViews GDT_DEPRECATED_MSG_ATTRIBUTE("use registerDataObject:clickableViews: instead.");
//
//
///**
// 此方法已经废弃
// 自渲染2.0视图注册方法
//
// @param dataObject 数据对象，必传字段
// @param mediaView 媒体对象视图，此处放视频播放器的容器视图
// @param logoView logo视图
// @param viewController 所在ViewController，必传字段。支持在register之后对其进行修改
// @param clickableViews 可点击的视图数组，此数组内的广告元素才可以响应广告对应的点击事件
// */
//- (void)registerDataObject:(GDTUnifiedNativeAdDataObject *)dataObject
//                 mediaView:(GDTMediaView *)mediaView
//                  logoView:(GDTLogoView *)logoView
//            viewController:(UIViewController *)viewController
//            clickableViews:(NSArray<UIView *> *)clickableViews GDT_DEPRECATED_MSG_ATTRIBUTE("use registerDataObject:clickableViews: instead.");
@end


@protocol GDTUnifiedNativeAdDelegate <NSObject>

/**
 广告数据回调

 @param unifiedNativeAdDataObjects 广告数据数组
 @param error 错误信息
 */
- (void)gdt_unifiedNativeAdLoaded:(NSArray<GDTUnifiedNativeAdDataObject *> * _Nullable)unifiedNativeAdDataObjects error:(NSError * _Nullable)error;
@end

@interface GDTUnifiedNativeAd : NSObject
@property (nonatomic, weak) id<GDTUnifiedNativeAdDelegate> delegate;

/**
 请求视频的时长下限。
 以下两种情况会使用 0，1:不设置  2:minVideoDuration大于maxVideoDuration
*/
@property (nonatomic) NSInteger minVideoDuration;

/**
 请求视频的时长上限，视频时长有效值范围为[5,180]。
 */
@property (nonatomic) NSInteger maxVideoDuration;

/**
 可选属性，设置本次拉取的视频广告从用户角度看到的视频播放策略。
 
 “用户角度”特指用户看到的情况，并非SDK是否自动播放，与自动播放策略 GDTVideoAutoPlayPolicy 的取值并非一一对应
 
 例如开发者设置了 GDTVideoAutoPlayPolicyNever 表示 SDK 不自动播放视频，但是开发者通过 GDTMediaView 的 play 方法播放视频，这在用户看来仍然是自动播放的。
 
 准确的设置 GDTVideoPlayPolicy 有助于提高视频广告的eCPM值，如果广告位仅支持图文广告，则无需调用。
 
 需要在 loadAd 前设置此属性。
 */
@property (nonatomic, assign) GDTVideoPlayPolicy videoPlayPolicy GDT_DEPRECATED_MSG_ATTRIBUTE("接口即将废弃");

/**
 可选属性，设置本次拉取的视频广告封面是由SDK渲染还是开发者自行渲染。
 
 SDK 渲染，指视频广告 containerView 直接在 feed 流等场景展示，用户可以直接看到渲染的视频广告。Demo 工程中的 “视频Feed” 就是 SDK 渲染。
 
 开发者自行渲染，指开发者获取到广告对象后，先用封面图字段在 feed 流中先渲染出一个封面图入口，用户点击封面图，再进入一个有 conainterView 的详细页，播放视频。Demo 工程中的 "竖版 Feed 视频" 就是开发者渲染的场景。
 */
@property (nonatomic, assign) GDTVideoRenderType videoRenderType GDT_DEPRECATED_MSG_ATTRIBUTE("接口即将废弃");

/**
 构造方法

 @param placementId 广告位ID
 @return GDTUnifiedNativeAd 实例
 */
- (instancetype)initWithPlacementId:(NSString *)placementId;

/**
 *  构造方法, S2S bidding 后获取到 token 再调用此方法
 *  @param placementId  广告位 ID
 *  @param token  通过 Server Bidding 请求回来的 token
 */
- (instancetype)initWithPlacementId:(NSString *)placementId token:(NSString *)token;

/**
 *  S2S bidding 竞胜之后调用, 需要在调用广告 show 之前调用
 *  @param eCPM - 曝光扣费, 单位分，若优量汇竞胜，在广告曝光时回传，必传
 *  针对本次曝光的媒体期望扣费，常用扣费逻辑包括一价扣费与二价扣费，当采用一价扣费时，胜者出价即为本次扣费价格；当采用二价扣费时，第二名出价为本次扣费价格.
 */
- (void)setBidECPM:(NSInteger)eCPM;

/**
 加载广告
 */
- (void)loadAd;

/**
 加载广告

 @param adCount 加载条数
 */
- (void)loadAdWithAdCount:(NSInteger)adCount;

/**
 *  竞胜之后调用, 需要在调用广告 show 之前调用
 *
 *  @param winInfo 字典类型，支持的key有
 *  GDT_M_W_E_COST_PRICE：竞胜价格 (单位: 分)，值类型为NSNumber *
 *  GDT_M_W_H_LOSS_PRICE：最高失败出价，值类型为NSNumber  *
 *
 */
- (void)sendWinNotificationWithInfo:(NSDictionary *)winInfo;

/**
 *  竞败之后调用
 *
 *  @pararm lossInfo 竞败信息，字典类型，支持的key有
 *  GDT_M_L_WIN_PRICE ：竞胜价格 (单位: 分)，值类型为NSNumber *
 *  GDT_M_L_LOSS_REASON ：优量汇广告竞败原因，竞败原因参考枚举GDTAdBiddingLossReason中的定义，值类型为NSNumber *
 *  GDT_M_ADNID  ：竞胜方渠道ID，值类型为NSString *
 */
- (void)sendLossNotificationWithInfo:(NSDictionary *)lossInfo;


/**
 返回广告平台名称
 
 @return 当使用流量分配功能时，用于区分广告平台；未使用时为空字符串
 */
- (NSString *)adNetworkName;

/**
 * 当需要支持 VAST 广告时，需流量自行配置 adapter 的 vastClassName
 */
- (void)setVastClassName:(NSString *)vastClassName;

@end

@class GDTMediaView;
@protocol GDTMediaViewDelegate <NSObject>

@optional

/**
 用户点击 MediaView 回调，当 GDTVideoConfig userControlEnable 设为 YES，用户点击 mediaView 会回调。
 
 @param mediaView 播放器实例
 */
- (void)gdt_mediaViewDidTapped:(GDTMediaView *)mediaView;

/**
 播放完成回调

 @param mediaView 播放器实例
 */
- (void)gdt_mediaViewDidPlayFinished:(GDTMediaView *)mediaView;

@end

@interface GDTMediaView : UIView

/**
 GDTMediaView 回调对象
 */
@property (nonatomic, weak) id <GDTMediaViewDelegate> delegate;

/**
 * 视频广告时长，单位 ms
 */
- (CGFloat)videoDuration;

/**
 * 视频广告已播放时长，单位 ms
 */
- (CGFloat)videoPlayTime;

/**
 播放视频
 */
- (void)play;

/**
 暂停视频，调用 pause 后，需要被暂停的视频广告对象，不会再自动播放，需要调用 play 才能恢复播放。
 */
- (void)pause;

/**
 停止播放，并展示第一帧
 */
- (void)stop;

/**
 播放静音开关
 @param flag 是否静音
 */
- (void)muteEnable:(BOOL)flag;

/**
 自定义播放按钮

 @param image 自定义播放按钮图片，不设置为默认图
 @param size 自定义播放按钮大小，不设置为默认大小 44 * 44
 */
- (void)setPlayButtonImage:(UIImage *)image size:(CGSize)size;

#pragma mark - DEPRECATED
/**
 是否支持在WWAN下自动播放视频， 默认 NO，已废弃，请使用 GDTVideoConfig 类配置
 */
@property (nonatomic, assign) BOOL videoAutoPlayOnWWAN GDT_DEPRECATED_ATTRIBUTE;

/**
 是否静音播放视频广告， 默认 YES，已废弃，请使用 GDTVideoConfig 类配置
 */
@property (nonatomic, assign) BOOL videoMuted GDT_DEPRECATED_ATTRIBUTE;

@end

NS_ASSUME_NONNULL_END

#endif /* OMTencentAdNativeClass_h */
