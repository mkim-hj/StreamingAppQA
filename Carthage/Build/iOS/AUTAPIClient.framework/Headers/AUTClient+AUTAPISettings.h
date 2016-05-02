//
//  AUTClient+AUTAPISettings.h
//  AUTAPIClient
//
//  Created by James Lawton on 10/1/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTClient.h>

NS_ASSUME_NONNULL_BEGIN

/// Represents the types of Facets that have settings
typedef NS_ENUM(NSInteger, AUTAPIFacetType) {
    /// Park facet
    AUTAPIFacetTypePark
};

@interface AUTClient (AUTAPISettings)

/// Fetch global settings. Sends `AUTAPISettingsPage` and completes, or errors.
- (RACSignal *)fetchSettings;

/// Fetch settings for a particular facet and vehicle. Sends
/// `AUTAPISettingsPage` and completes, or errors.
- (RACSignal *)fetchSettingsForFacet:(AUTAPIFacetType)facet vehicleID:(NSString *)vehicleID;

/// Update a setting. `value` must be encodable as JSON.
- (RACSignal *)updateSetting:(NSURL *)settingURL value:(id)value;

@end

NS_ASSUME_NONNULL_END
