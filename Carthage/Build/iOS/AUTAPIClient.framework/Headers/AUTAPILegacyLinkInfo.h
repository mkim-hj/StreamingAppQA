//
//  AUTAPILegacyLinkInfo.h
//  AUTAPIClient
//
//  Created by Sylvain Rebaud on 2015-09-28.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import Mantle;

#import <AUTAPIClient/AUTAPILegacyResponse.h>

NS_ASSUME_NONNULL_BEGIN

/// Represents a link information to be synched with the server.
@interface AUTAPILegacyLinkInfo : MTLModel <MTLJSONSerializing>

/// The unique phone identifier associated
@property (nonatomic, copy, readonly) NSString *phoneID;

/// The list of features supported by adapter as reported by firmware
@property (nonatomic, copy, readonly) NSArray<NSString *> *features;

/// The user ID associated with link as returned by server.
@property (nonatomic, copy, readonly) NSString *userID;

/// The link unique hardware device ID.
@property (nonatomic, copy, readonly) NSString *hardwareDeviceID;

/// The link unique BT Gatt Service ID.
@property (nonatomic, copy, readonly) NSString *autBluetoothID;

/// The link BT Peripheral UUID as advertised by iOS.
@property (nonatomic, copy, readonly) NSString *iosBluetoothID;

/// The Base64 wrapped session key.
@property (nonatomic, copy, readonly) NSData *wrappedKey;

/// The Base64 encoded session key.
@property (nonatomic, copy, readonly) NSData *encryptionKey;

/// The associated vehicle VIN number.
@property (nonatomic, copy, readonly) NSString *VIN;

@end

@interface AUTLegacyLinkInfoResponse : AUTAPILegacyResponse <MTLJSONSerializing>

@property (readonly, nonatomic, copy) NSArray<AUTAPILegacyLinkInfo *> *links;

@end

NS_ASSUME_NONNULL_END
