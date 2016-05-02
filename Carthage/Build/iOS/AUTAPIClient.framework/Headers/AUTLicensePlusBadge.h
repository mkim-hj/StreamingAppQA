//
//  AUTLicensePlusBadge.h
//  AUTAPIClient
//
//  Created by James Lawton on 4/7/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <Mantle/Mantle.h>
#import <AUTAPIClient/AUTAPIPage.h>

NS_ASSUME_NONNULL_BEGIN

typedef enum {
    AUTBadgeSizeSmall,
    AUTBadgeSizeLarge,
} AUTBadgeSize;

typedef enum {
    AUTBadgeStateInProgress,
    AUTBadgeStateGranted,
} AUTBadgeState;

@interface AUTLicensePlusBadge : MTLModel <MTLJSONSerializing>

@property (nonatomic, copy, readonly) NSString *identifier;
@property (nonatomic, strong, readonly) NSURL *URL;

// The localised name of the badge
@property (nonatomic, copy, readonly) NSString *displayName;

// Further localised messages about the badge
@property (nonatomic, copy, readonly) NSString *requirements;
@property (nonatomic, copy, readonly) NSString *completedMessage;
@property (nonatomic, copy, readonly) NSString *sharingSubjectMessage;
@property (nonatomic, copy, readonly) NSString *sharingTextMessage;
@property (nonatomic, copy, readonly) NSString *sharingImageMessage;

// The name or URL of the icon to use in a given circumstance
- (nullable NSString *)iconResourceNameForState:(AUTBadgeState)state size:(AUTBadgeSize)size;

@end

AUTPageSubclassInterface(AUTLicensePlusBadge)

NS_ASSUME_NONNULL_END
