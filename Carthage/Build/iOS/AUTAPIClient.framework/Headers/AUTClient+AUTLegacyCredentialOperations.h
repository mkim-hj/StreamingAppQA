//
//  AUTClient+AUTLegacyCredentialOperations.h
//  AUTAPIClient
//
//  Created by Eric Horacek on 6/18/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import ReactiveCocoa;

#import <AUTAPIClient/AUTClient.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTClient (AUTLegacyCredentialOperations)

/// A cold signal that creates a client upon subscription and then attempts to
/// restore its legacyCredential property from a credential that was previously
/// persisted with the storeLegacyCredential method.
///
/// Does not guarantee that the credential will be restored successfully. Use
/// the authenticatedWithLegacy method to check whether a client is
/// authenticated.
+ (RACSignal *)restoredLegacyClientWithClientID:(NSString *)clientID clientSecret:(NSString *)clientSecret;

/// A cold signal that upon subscription signs out the client from the legacy
/// API by performing the following steps:
///
/// - Delete the legacy credential from persistence (if it was persisted)
/// - Set the legacyCredential property to nil
///
/// and then completes.
///
/// If this signal is unable to delete the persisted credential, it will error
/// with the AUTClientErrorUnableToDeleteStoredCredential code.
- (RACSignal *)signOutLegacy;

/// A signal that upon subscription attempts to store the legacyCredential
/// property for later restoration, typically as part of
/// -[AUTClient restoredLegacyClientWithClientID:clientSecret:];
///
/// If the save is unsuccessful, it will error with the
/// AUTClientErrorUnableToStoreCredential code.
- (RACSignal *)storeLegacyCredential;

/// A signal that upon subscription attempts to restore the legacyCredential
/// property persisted by an invocation to storeLegacyCredential.
///
/// If restoration fails, errors with a AUTClientErrorUnableToRestoreCredential
/// code
- (RACSignal *)restoreLegacyCredential;

/// A cold signal that upon subscription creates a restored client and then
/// attempts to restore its credential and legacyCredential property from
/// credentials that were previously persisted with the storeCredential
/// and storeLegacyCredential methods.
///
/// Does not guarantee that the credentials will be restored successfully. Use
/// the authenticated/authenticatedWithLegacy methods to check whether a
/// client is authenticated.
+ (RACSignal *)restoredLegacyAndNewtonClientWithClientID:(NSString *)clientID clientSecret:(NSString *)clientSecret;

/// A cold signal identical to one returned by
/// -[AUTClient restoredLegacyClientWithClientID:clientSecret:]
/// with an option to specify a certain server environment.
+ (RACSignal *)restoredLegacyAndNewtonClientWithClientID:(NSString *)clientID clientSecret:(NSString *)clientSecret server:(AUTServerEnvironment)server;

/// Signs out of the legacy and newton APIs simultaneously by merging
/// signOutLegacy with signOut.
- (RACSignal *)signOutLegacyAndNewton;

/// Stores the legacy and newton credentials simultaneously by merging
/// storeCredential and storeLegacyCredential.
- (RACSignal *)storeLegacyAndNewtonCredentials;

/// Restores the legacy and newton credentials simultaneously by merging
/// restoreCredential and restoreLegacyCredential.
- (RACSignal *)restoreLegacyAndNewtonCredential;

@end

NS_ASSUME_NONNULL_END
