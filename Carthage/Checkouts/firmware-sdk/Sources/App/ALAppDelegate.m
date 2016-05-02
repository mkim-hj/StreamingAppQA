/*****************************************************************************
**
**  Automatic Labs - CONFIDENTIAL
**
**  Unpublished Copyright (c) 2009-2016 AUTOMATIC LABS, All Rights Reserved.
**
**  NOTICE:
**
**  All information contained herein is, and remains the property of AUTOMATIC LABS.
**  The intellectual and technical concepts contained herein are proprietary to AUTOMATIC LABS
**  and may be covered by U.S. and Foreign Patents, patents in process, and are protected by trade
**  secret or copyright law.
**
**  Dissemination of this information or reproduction of this material is strictly forbidden unless
**  prior written permission is obtained from AUTOMATIC LABS.  Access to the source code contained
**  herein is hereby forbidden to anyone except current AUTOMATIC LABS employees, managers or
**  contractors who have executed Confidentiality and Non-disclosure agreements explicitly covering
**  such access.
**
**  The copyright notice above does not evidence any actual or intended publication or disclosure of
**  this source code, which includes information that is confidential and/or proprietary, and is a
**  trade secret, of AUTOMATIC LABS. ANY REPRODUCTION, MODIFICATION, DISTRIBUTION, PUBLIC PERFORMANCE,
**  OR PUBLIC DISPLAY OF OR THROUGH USE OF THIS SOURCE CODE WITHOUT THE EXPRESS WRITTEN CONSENT OF
**  AUTOMATIC LABS IS STRICTLY PROHIBITED, AND IN VIOLATION OF APPLICABLE LAWS AND INTERNATIONAL TREATIES.
**  THE RECEIPT OR POSSESSION OF THIS SOURCE CODE AND/OR RELATED INFORMATION DOES NOT CONVEY OR IMPLY
**  ANY RIGHTS TO REPRODUCE, DISCLOSE OR DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE, USE, OR SELL
**  ANYTHING THAT IT MAY DESCRIBE, IN WHOLE OR IN PART.
**
******************************************************************************/

@import AutomaticAdapter;

#import "ALAppDelegate.h"
#import "ALRunViewController.h"

#define USE_ALAUTOMATICADAPTER_INTERFACE (1)

static NSDictionary *_authStatusNames;

@implementation ALAppDelegate

+ (void)initialize {
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        _authStatusNames = @{
            @(kALAuthStatus_AdapterAccessPrivileges_Acquiring) : @"kALAuthStatus_AdapterAccessPrivileges_Acquiring",
            @(kALAuthStatus_AdapterAccessPrivileges_Installed) : @"kALAuthStatus_AdapterAccessPrivileges_Installed",
            @(kALAuthStatus_AdapterAccessPrivileges_Installing) : @"kALAuthStatus_AdapterAccessPrivileges_Installing",
            @(kALAuthStatus_AdapterSession_Authenticated) : @"kALAuthStatus_AdapterSession_Authenticated",
            @(kALAuthStatus_AdapterSession_Authenticating) : @"kALAuthStatus_AdapterSession_Authenticating",
            @(kALAuthStatus_AccountAccessPrivileges_Acquiring) : @"kALAuthStatus_AccountAccessPrivileges_Acquiring",
            @(kALAuthStatus_None) : @"kALAuthStatus_None",
            @(kALAuthStatus_UserInteractionRequired) : @"kALAuthStatus_UserInteractionRequired",
        };
    });
}

+ (NSString *)automaticClientID {
    return @"bd28d964c234e9df04eb";
}

+ (NSString *)automaticClientSecret {
    return @"8c36388524fa91e787f25fbb17054524db1923eb";
}

- (BOOL)application:(UIApplication *)inApplication didFinishLaunchingWithOptions:(NSDictionary *)inOptions {
    [[EAAccessoryManager sharedAccessoryManager] registerForLocalNotifications];

    return YES;
}

- (BOOL)application:(UIApplication *)inApplication openURL:(NSURL *)inUrl sourceApplication:(NSString *)inSourceApplication annotation:(id)inAnnotation {
    ALAppDelegate *__weak weakSelf = self;
    BOOL isAdapterAuthorizationURL;

#if USE_ALAUTOMATICADAPTER_INTERFACE

    // This example shows how to use the high-level ALAutomaticAdapter interface
    // to handle an authorization URL.  It provides an ALAutomaticAdapter
    // instance if authorization succeeds.

    isAdapterAuthorizationURL = [ALAutomaticAdapter
        authorizeAdapterForURL:inUrl
        withSecret:[ALAppDelegate automaticClientSecret]
        onStatus:^(ALAuthStatus inAuthStatus, NSError *inError) {
            NSLog(@"%@ %@", _authStatusNames[@(inAuthStatus)], inError);
        }
        onAuthorized:^(ALAutomaticAdapter *inAdapter, NSString *inClientID, ALAutomaticAdapterOnAuthorizationStatus inStatusCallback) {

            // Authorized.  The current app now has keychain credentials to access
            // engine data on the adapter.  Pass inAdapter to whatever code handles
            // a discovered Automatic Adapter.  See ViewController -setAutomaticAdapter
            // for an example of how call an ALAutomaticAdapter to open an
            // ALELM327Session that has access to the adapter's privileged data.
            //
            // Authorization is a one-time process per adapter.  Once completed, the
            // credentials needed to access the adapter are saved on the app's keychain
            // and no more user interaction is needed for the current app to access
            // that adapter.

            NSLog(@"AUTHORIZED %@", inAdapter);
            UINavigationController *navController = (UINavigationController *)[[weakSelf window] rootViewController];
            ALRunViewController *adapterVC = navController.viewControllers[0];
            [adapterVC setAutomaticAdapter:inAdapter];
        }];

#else

    // This example shows how to use the EAAccessory(AL) category to handle an
    // authorization URL.  It provides an EAAccessory instance if authorization
    // succeeds.

    isAdapterAuthorizationURL = [EAAccessory
        authorizeAutomaticAdapterForURL:inUrl
        withSecret:[ALAppDelegate automaticClientSecret]
        onStatus:^(ALAuthStatus inAuthStatus, NSError *inError) {
            AL_LOG(@"%@ %@", _authStatusNames[@(inAuthStatus)], inError);
        }
        onAuthorized:^(EAAccessory *inAdapter, NSString *inClientID, ALAccessoryOnAuthorizationStatus inStatusCallback) {

            // Authorized.  The current app now has keychain credentials to access
            // engine data on the adapter.  Pass inAdapter to whatever code handles
            // a discovered adapter.  See ViewController -setAccessory for an example
            // of how to call an EAAccessory to open an EASession that has access to
            // the adapter's privileged data.
            //
            // Authorization is a one-time process per adapter.  Once completed, the
            // credentials needed to access the adapter are saved on the app's keychain
            // and no more user interaction is needed for the current app to access
            // that adapter.

            NSLog(@"AUTHORIZED %@", inAdapter);
            UINavigationController *navController = (UINavigationController *)[[weakSelf window] rootViewController];
            ViewController *adapterVC = [navController viewControllers][0];
            [adapterVC setAccessory:inAdapter];
        }];

#endif // #if USE_ALAUTOMATICADAPTER_INTERFACE

    return isAdapterAuthorizationURL;
}

- (void)applicationWillTerminate:(UIApplication *)inApplication {
    [[EAAccessoryManager sharedAccessoryManager] unregisterForLocalNotifications];
}

@end
