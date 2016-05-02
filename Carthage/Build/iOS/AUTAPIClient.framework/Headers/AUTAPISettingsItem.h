//
//  AUTAPISettingsItem.h
//  AUTAPIClient
//
//  Created by James Lawton on 9/30/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTAPISettingsObject.h>

NS_ASSUME_NONNULL_BEGIN

/// Represents a single setting.
@interface AUTAPISettingsItem : AUTAPISettingsObject

/// Unique identifier of the setting.
@property (nonatomic, readonly, copy) NSString *objectID;

/// The URL to set the value of this setting.
@property (readonly, nonatomic, strong) NSURL *URL;

/// Some notion of a default value.
@property (nonatomic, readonly, copy) id<NSCopying> defaultValue;

/// The current value of the setting (redeclared).
@property (nonatomic, readonly, copy, nonnull) id<NSCopying> currentValue;

/// True if the vehicle list needs to be refreshed when this setting is updated.
@property (nonatomic, readonly) BOOL mutatesVehicle;

@end

NS_ASSUME_NONNULL_END
