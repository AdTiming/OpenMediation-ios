// Copyright 2020 ADTIMING TECHNOLOGY COMPANY LIMITED
// Licensed under the GNU Lesser General Public License Version 3

#import "AppDelegate.h"
#import "WelcomeViewController.h"

@import OpenMediation;
@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    if (@available(iOS 14, *)) {
        ATTrackingManagerAuthorizationStatus status = ATTrackingManager.trackingAuthorizationStatus;
        Class fbSettingCls = NSClassFromString(@"FBAdSettings");
        if (fbSettingCls && [fbSettingCls respondsToSelector:@selector(setAdvertiserTrackingEnabled:)]) {
            [fbSettingCls setAdvertiserTrackingEnabled:(status == ATTrackingManagerAuthorizationStatusAuthorized)];
        }
    }
    self.window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    self.window.backgroundColor = [UIColor whiteColor];
    self.window.rootViewController = [[UINavigationController alloc] initWithRootViewController:[[WelcomeViewController alloc] init]];
    [self.window makeKeyAndVisible];
    [OpenMediation setUseCacheAdFormat:OpenMediationAdFormatSplash];
    [OpenMediation initWithAppKey:[self getAppKey] baseHost:[self getBaseHost] adFormat:(OpenMediationAdFormatInterstitial|OpenMediationAdFormatRewardedVideo|OpenMediationAdFormatCrossPromotion)];    
    return YES;
}

- (NSString *)getAppKey {
    NSString *appKey = [[NSUserDefaults standardUserDefaults] valueForKey:@"OpenMediationAppKey"];
    if (!appKey) {
        appKey = @"2wXT9C0MPoDIAXEDeH04O86PeHQrsko4";
        [[NSUserDefaults standardUserDefaults] setValue:appKey forKey:@"OpenMediationAppKey"];
    }
    return appKey;
}

- (NSString*)getBaseHost {
    NSString *baseHost = [[NSUserDefaults standardUserDefaults] valueForKey:@"OpenMediationBaseHost"];
    if (!baseHost) {
        baseHost = @"https://ads.test.mises.site";
        [[NSUserDefaults standardUserDefaults] setValue:baseHost forKey:@"OpenMediationBaseHost"];
    }
    return baseHost;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and invalidate graphics rendering callbacks. Games should use this method to pause the game.
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the active state; here you can undo many of the changes made on entering the background.
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}


- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}


@end
