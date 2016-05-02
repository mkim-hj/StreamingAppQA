//
//  AppDelegate.m
//  Oregon
//
//  Created by Maruchi Kim on 10/22/15.
//  Copyright © 2015 Maruchi Kim. All rights reserved.
//

#import "AppDelegate.h"
#import "AutomaticAdapter/AutomaticAdapter.h"
#import "TestViewController.h"
#import "DashboardViewController.h"

@import AFNetworking;
@import AUTAPIClient;
@import HockeySDK;

@interface AppDelegate ()

@end

@implementation AppDelegate

- (void) goToDashboard {
    [[_dashboardController navigationController] popToViewController:_dashboardController animated:YES];
}

- (void) goToLogin {
    [[_dashboardController navigationController] popToViewController:_loginController animated:YES];
}

- (void)commonFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        [NSUserDefaults standardUserDefaults];
    });
}

- (BOOL)application:(UIApplication *)application willFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
//    [[BITHockeyManager sharedHockeyManager] configureWithIdentifier:@"9f81d665ba0745688800a8e69477525d"];
//    [[BITHockeyManager sharedHockeyManager] startManager];
//    [[BITHockeyManager sharedHockeyManager].authenticator authenticateInstallation];

    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
    UIViewController *vc1 = [storyboard instantiateViewControllerWithIdentifier:@"LoginViewController"];
    UIViewController *vc2 = [storyboard instantiateViewControllerWithIdentifier:@"DashboardViewController"];
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSString *emailAddress = [defaults stringForKey:@"emailAddress"];
    
    //If _user is nil that means no internet, default to login screen.
    NSArray *controllers = emailAddress ? @[vc1, vc2] : @[vc1];
    
    UINavigationController *nc = (UINavigationController *) self.window.rootViewController;
    _loginController = vc1;
    _dashboardController = vc2;
    [nc setViewControllers:controllers];
    
    [self commonFinishLaunchingWithOptions:launchOptions];
    return YES;
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{    
    [self commonFinishLaunchingWithOptions:launchOptions];
    return YES;
}

- (BOOL) application:(UIApplication *)application shouldRestoreApplicationState:(NSCoder *)coder {
    return YES;
}

- (BOOL) application:(UIApplication *)application shouldSaveApplicationState:(NSCoder *)coder {
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
    // Saves changes in the application's managed object context before the application terminates.
    [self saveContext];
}

#pragma mark - Core Data stack

@synthesize managedObjectContext = _managedObjectContext;
@synthesize managedObjectModel = _managedObjectModel;
@synthesize persistentStoreCoordinator = _persistentStoreCoordinator;

- (NSURL *)applicationDocumentsDirectory {
    // The directory the application uses to store the Core Data store file. This code uses a directory named "Automatic.Oregon" in the application's documents directory.
    return [[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] lastObject];
}

- (NSManagedObjectModel *)managedObjectModel {
    // The managed object model for the application. It is a fatal error for the application not to be able to find and load its model.
    if (_managedObjectModel != nil) {
        return _managedObjectModel;
    }
    NSURL *modelURL = [[NSBundle mainBundle] URLForResource:@"Oregon" withExtension:@"momd"];
    _managedObjectModel = [[NSManagedObjectModel alloc] initWithContentsOfURL:modelURL];
    return _managedObjectModel;
}

- (NSPersistentStoreCoordinator *)persistentStoreCoordinator {
    // The persistent store coordinator for the application. This implementation creates and returns a coordinator, having added the store for the application to it.
    if (_persistentStoreCoordinator != nil) {
        return _persistentStoreCoordinator;
    }
    
    // Create the coordinator and store
    
    _persistentStoreCoordinator = [[NSPersistentStoreCoordinator alloc] initWithManagedObjectModel:[self managedObjectModel]];
    NSURL *storeURL = [[self applicationDocumentsDirectory] URLByAppendingPathComponent:@"Oregon.sqlite"];
    NSError *error = nil;
    NSString *failureReason = @"There was an error creating or loading the application's saved data.";
    if (![_persistentStoreCoordinator addPersistentStoreWithType:NSSQLiteStoreType configuration:nil URL:storeURL options:nil error:&error]) {
        // Report any error we got.
        NSMutableDictionary *dict = [NSMutableDictionary dictionary];
        dict[NSLocalizedDescriptionKey] = @"Failed to initialize the application's saved data";
        dict[NSLocalizedFailureReasonErrorKey] = failureReason;
        dict[NSUnderlyingErrorKey] = error;
        error = [NSError errorWithDomain:@"YOUR_ERROR_DOMAIN" code:9999 userInfo:dict];
        // Replace this with code to handle the error appropriately.
        // abort() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
        NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
        abort();
    }
    
    return _persistentStoreCoordinator;
}


- (NSManagedObjectContext *)managedObjectContext {
    // Returns the managed object context for the application (which is already bound to the persistent store coordinator for the application.)
    if (_managedObjectContext != nil) {
        return _managedObjectContext;
    }
    
    NSPersistentStoreCoordinator *coordinator = [self persistentStoreCoordinator];
    if (!coordinator) {
        return nil;
    }
    _managedObjectContext = [[NSManagedObjectContext alloc] initWithConcurrencyType:NSMainQueueConcurrencyType];
    [_managedObjectContext setPersistentStoreCoordinator:coordinator];
    return _managedObjectContext;
}

#pragma mark - Core Data Saving support

- (void)saveContext {
    NSManagedObjectContext *managedObjectContext = self.managedObjectContext;
    if (managedObjectContext != nil) {
        NSError *error = nil;
        if ([managedObjectContext hasChanges] && ![managedObjectContext save:&error]) {
            // Replace this implementation with code to handle the error appropriately.
            // abort() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
            NSLog(@"Unresolved error %@, %@", error, [error userInfo]);
            abort();
        }
    }
}

- (BOOL)
application:(UIApplication *)inApplication
openURL:(NSURL *)inUrl
sourceApplication:(NSString *)inSourceApplication
annotation:(id)inAnnotation
{
    NSLog(@"After Safari");
    BOOL    isAdapterAuthorizationURL = [ALAutomaticAdapter
                                         authorizeAdapterForURL:inUrl
                                         withSecret:@"9a1da50fa231b8fb077ba575dee0f1b6ac50ec20"
                                         onStatus:^(ALAuthStatus inAuthStatus, NSError *inError) {
                                             
                                             // Monitor authorization status and handle any errors here...
                                             NSLog(@"APP DELEGATE: On Status, %d, %@", inAuthStatus, inError);
                                             
                                         }
                                         onAuthorized:^(ALAutomaticAdapter *inAdapter, NSString *inClientID, ALAutomaticAdapterOnAuthorizationStatus inStatusCallback) {
                                             
                                             // This block is called if authorization succeeds.  The app now has
                                             // keychain credentials to access engine data on the adapter.  Pass
                                             // inAdapter to whatever code handles a discovered Automatic Adapter.
                                             // Retain inAdapter to continue using it after this block returns.
                                             
                                             NSLog(@"APP DELEGATE: On Authorized");
                                             NSNotificationCenter * defaultCenter = [NSNotificationCenter defaultCenter];
                                             [defaultCenter postNotificationName:@"authorizationReceived" object:inAdapter];
                                             
                                             
                                             
                                         }];
    
    return isAdapterAuthorizationURL;
}

@end
