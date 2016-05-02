//
//  AUTAPIEncryptionData.h
//  AUTAPIClient
//
//  Created by Robert Böhnke on 02/04/15.
//  Copyright (c) 2015 Automatic. All rights reserved.
//

#import <AUTAPIClient/AUTAPILegacyResponse.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTAPIEncryptionData : AUTAPILegacyResponse <MTLJSONSerializing>

@property (readonly, nonatomic, strong) NSData *encryptionKey;

/// This should not be persisted, like, ever.
@property (readonly, nonatomic, strong) NSData *wrappedEncryptionKey;

@end

NS_ASSUME_NONNULL_END
