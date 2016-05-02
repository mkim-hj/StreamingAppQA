//
//  AUTAPISettings.h
//  AUTAPIClient
//
//  Created by James Lawton on 9/29/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTAPISettingsObject.h>

NS_ASSUME_NONNULL_BEGIN

@class AUTAPISettingsGroup;

/// Represents a (branch on the) tree of settings.
@interface AUTAPISettingsPage : AUTAPISettingsObject

/// Array of groups of items in this branch.
@property (nonatomic, readonly, copy) NSArray<AUTAPISettingsGroup *> *groups;

/// A unique identifier of this branch of the settings.
@property (nonatomic, readonly, copy) NSString *viewPath;

/// The current value of the setting (redeclared).
@property (nonatomic, readonly, copy, nullable) NSString *currentValue;

/// Text to appear at the top of the settings page.
@property (nonatomic, readonly, copy, nullable) NSString *header;

@end

NS_ASSUME_NONNULL_END
