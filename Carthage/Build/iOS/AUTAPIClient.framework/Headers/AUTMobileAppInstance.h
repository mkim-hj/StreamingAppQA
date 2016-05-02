//
//  AUTPhoneProfile.h
//  AUTAPIClient
//
//  Created by James Lawton on 11/12/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

@import Mantle;

#import <AUTAPIClient/AUTAPIPage.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, AUTPhoneProfilePushTarget) {
    AUTPhoneProfilePushTargetProduction,
    AUTPhoneProfilePushTargetSandbox
};

typedef NS_ENUM(NSUInteger, AUTPhoneProfilePlatform) {
    AUTPhoneProfilePlatformiOS,
    AUTPhoneProfilePlatformAndroid,
};

@interface AUTMobileAppInstance : MTLModel <MTLJSONSerializing>

/// The unique ID of this profile, from the server.
///
/// This will be `nil` if the profile was generated locally.
@property (readonly, nonatomic, copy, nullable) NSString *objectID;

/// The platform of the phone.
///
/// This will be `AUTPhoneProfilePlatformiOS` if the profile was
/// generated locally.
@property (readonly, nonatomic) AUTPhoneProfilePlatform platform;

/// Phone identifier provided by phone.
///
/// See `-[UIDevice identifierForVendor]`
@property (readonly, nonatomic, copy) NSString *phoneIdentifier;

/// App bundle ID.
///
/// See `-[NSBundle bundleIdentifier]`
@property (readonly, nonatomic, copy) NSString *appIdentifier;

/// The push notification environment for this phone.
///
/// Production or Sandbox
@property (readonly, nonatomic) AUTPhoneProfilePushTarget pushTarget;

/// Push token returned by the OS upon registration with the push service.
///
/// See `-[UIApplication registerForRemoteNotifications]`
@property (readonly, nonatomic, copy, nullable) NSString *pushNotificationToken;

/// Use `-initWithAppIdentifier:phoneIdentifier:pushTarget:`
- (instancetype)init NS_UNAVAILABLE;

/// Use `-initWithAppIdentifier:phoneIdentifier:pushTarget:`
- (nullable instancetype)initWithDictionary:(NSDictionary *)dictionaryValue error:(NSError **)error NS_UNAVAILABLE;

/// Initialize a fresh phone profile.
- (instancetype)initWithAppIdentifier:(NSString *)appIdentifier phoneIdentifier:(NSString *)phoneIdentifier pushTarget:(AUTPhoneProfilePushTarget)pushTarget;

/// Returns a copy of the receiver with the given phone identifier.
- (instancetype)withPhoneIdentifier:(NSString *)phoneIdentifier;

/// Returns a copy of the receiver with the given push notification token.
- (instancetype)withPushNotificationToken:(NSString *)token;

/// Returns a copy of the receiver with the given push notification token.
///
/// Base64 encodes the token datato generate the string.
- (instancetype)withPushNotificationTokenData:(NSData *)token;

/// Returns an app instance with the ID of the receiver, and other values from
/// the argument.
- (instancetype)updateWithMobileAppInstance:(AUTMobileAppInstance *)mobileAppInstance;

@end

AUTPageSubclassInterface(AUTMobileAppInstance)

NS_ASSUME_NONNULL_END
