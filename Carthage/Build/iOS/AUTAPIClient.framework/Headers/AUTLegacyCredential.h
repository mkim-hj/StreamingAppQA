//
//  AUTLegacyCredential.h
//  AUTAPIClient
//
//  Created by Sylvain Rebaud on 5/8/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTAPILegacyResponse.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTLegacyCredential : AUTAPILegacyResponse <MTLJSONSerializing>

/// The userID string.
@property (readonly, nonatomic, copy) NSString *userID;

/// The session key string.
@property (readonly, nonatomic, copy) NSString *sessionKey;

- (instancetype)init NS_UNAVAILABLE;

/// Create a Legacy credential from a user id and a session key.
///
/// @param userID The userID string.
/// @param sessionKey The sessionKey string.
///
+ (instancetype)credentialWithUserID:(NSString *)userID sessionKey:(NSString *)sessionKey;

/// Initialize a Legacy credential from a user id and a session key.
///
/// @param userID The userID string.
/// @param sessionKey The sessionKey string.
///
- (instancetype)initWithUserID:(NSString *)userID sessionKey:(NSString *)sessionKey;

@end

NS_ASSUME_NONNULL_END
