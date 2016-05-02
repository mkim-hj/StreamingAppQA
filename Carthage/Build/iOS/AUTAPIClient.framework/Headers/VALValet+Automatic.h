//
//  VALValet+Automatic.h
//  AUTAPIClient
//
//  Created by Sylvain Rebaud on 10/20/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

@import Foundation;
@import Valet;

NS_ASSUME_NONNULL_BEGIN

@class AFOAuthCredential;
@class AUTLegacyCredential;

/// The keychain identifier used to generate a service name in keychain where
/// entries are to be saved.
extern NSString * const AUTClientKeychainIdentifer;

/// The keychain identifier to store the AFOAuthCredential object.
extern NSString * const AUTAFOAuthCredentialKey;

/// The keychain identifier to store the AUTLegacyCredential object.
extern NSString * const AUTLegacyCredentialKey;

/// Helper class to store OAuth Credentials and abstract how keychain is being
/// accessed.
@interface VALValet (Automatic)

/// An instance of a Valet configured with AUTClientKeychainIdentifer identifier
/// and the keychain accessibilty of kSecAttrAccessibleAfterFirstUnlock.
+ (VALValet *) aut_valet;

/// Stores the specified AFOAuthCredential in the keychain.
///
/// @param credential The AFOAuthCredential to be stored.
///
/// @return YES if credential was stored in the keychain, NO if keychain was not
///         accessible.
- (BOOL)aut_storeCredential:(AFOAuthCredential *)credential;

/// Retrieves the OAuth credential stored from the keychain or nil if the
/// keychain was not accessible.
///
/// @return The retrieved AFOAuthCredential.
- (nullable AFOAuthCredential *)aut_retrieveCredential;

/// Deletes the AFOAuthCredential stored from the Keychain.
///
/// @return YES if credential was stored in the keychain, NO if keychain was not
///         accessible.
- (BOOL)aut_deleteCredential;

/// Stores the specified AUTLegacyCredential in the keychain with the default
/// keychain Accessibilty of kSecAttrAccessibleAfterFirstUnlock.
///
/// @param credential The AUTLegacyCredential to be stored.
///
/// @return YES if credential was stored in the keychain, NO if keychain was not
///         accessible.
- (BOOL)aut_storeLegacyCredential:(AUTLegacyCredential *)credential;

/// Retrieves the OAuth credential stored from the keychain or nil if the
/// keychain was not accessible.
///
/// @return The retrieved AFOAuthCredential.
- (nullable AUTLegacyCredential *)aut_retrieveLegacyCredential;

/// Deletes the AUTLegacyCredential stored from the Keychain.
///
/// @return YES if credential was stored in the keychain, NO if keychain was not
///         accessible.
- (BOOL)aut_deleteLegacyCredential;

@end

NS_ASSUME_NONNULL_END
