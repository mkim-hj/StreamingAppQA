//
//  AUTAPISettingsGroup.h
//  AUTAPIClient
//
//  Created by James Lawton on 9/30/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

@import Mantle;

NS_ASSUME_NONNULL_BEGIN

@class AUTAPISettingsObject;

/// Represents a group of related settings, expected to be displayed
/// as a group to the user.
@interface AUTAPISettingsGroup : MTLModel <MTLJSONSerializing>

/// User-visible title of the group.
@property (nonatomic, readonly, copy) NSString *title;

/// User-visible footer of the group.
@property (nonatomic, readonly, copy) NSString *footer;

/// Items in this settings group.
@property (nonatomic, readonly, copy) NSArray<AUTAPISettingsObject *> *items;

@end

NS_ASSUME_NONNULL_END
