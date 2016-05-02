//
//  AUTClient+AUTAPIEncryptionData.h
//  AUTAPIClient
//
//  Created by Robert Böhnke on 02/04/15.
//  Copyright (c) 2015 Automatic. All rights reserved.
//

#import <AUTAPIClient/AUTClient.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTClient (AUTAPIEncryptionData)

/// Returns a signal that will send an instance of `AUTAPIEncryptionData` for the
/// given setup data and PIN, or else error.
- (RACSignal *)fetchEncryptionKeysForSetupData:(NSDictionary *)setupData PIN:(NSString *)PIN;

@end

NS_ASSUME_NONNULL_END
