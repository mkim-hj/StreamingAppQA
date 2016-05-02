//
//  AppDelegate.h
//  Oregon
//
//  Created by Maruchi Kim on 10/22/15.
//  Copyright © 2015 Maruchi Kim. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreData/CoreData.h>
#import "AutomaticAdapter/AutomaticAdapter.h"

@import AUTAPIClient;

@interface AppDelegate : UIResponder <UIApplicationDelegate>


@property (strong, nonatomic) UIWindow *window;

@property (readonly, strong, nonatomic) NSManagedObjectContext *managedObjectContext;
@property (readonly, strong, nonatomic) NSManagedObjectModel *managedObjectModel;
@property (readonly, strong, nonatomic) NSPersistentStoreCoordinator *persistentStoreCoordinator;
@property (weak,   nonatomic) UIViewController *dashboardController;
@property (weak,   nonatomic) UIViewController *loginController;
@property (strong, nonatomic) AUTClient *client;

- (void)saveContext;
- (NSURL *)applicationDocumentsDirectory;
- (void) goToDashboard;
- (void) goToLogin;

@end

