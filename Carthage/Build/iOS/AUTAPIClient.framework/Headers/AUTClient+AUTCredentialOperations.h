//
//  AUTClient+AUTCredentialOperations.h
//  AUTAPIClient
//
//  Created by Eric Horacek on 6/18/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import ReactiveCocoa;

#import <AUTAPIClient/AUTClient.h>

@interface AUTClient (AUTCredentialOperations)

/// A cold signal that creates a client upon subscription and then attempts to
/// restore its credential property from a credential that was previously
/// persisted with the storeCredential method.
///
/// Does not guarantee that the credential will be restored successfully. Use
/// the authenticated method to check whether a client is authenticated.
+ (RACSignal *)restoredClientWithClientID:(NSString *)clientID clientSecret:(NSString *)clientSecret;

/// A cold signal that upon subscription signs out the client performing the
/// following steps:
///
/// - Delete the credential from persistence (if it was persisted)
/// - Set the credential property to nil
///
/// and then completes.
///
/// If this signal is unable to delete the persisted credential, it will error
/// with the AUTClientErrorUnableToDeleteStoredCredential code.
- (RACSignal *)signOut;

/// A signal that upon subscription attempts to store the credential property
/// for later restoration, typically as part of
/// -[AUTClient restoredClientWithClientID:clientSecret:];
///
/// If the save is unsuccessful, it will error with the
/// AUTClientErrorUnableToStoreCredential code.
- (RACSignal *)storeCredential;

/// A signal that upon subscription attempts to restore the credential property
/// persisted by an invocation to storeCredential.
///
/// If restoration fails, errors with a AUTClientErrorUnableToRestoreCredential
/// code
- (RACSignal *)restoreCredential;

@end
