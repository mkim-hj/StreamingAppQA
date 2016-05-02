//
//  AUTAPISettingsToggleItem.h
//  AUTAPIClient
//
//  Created by James Lawton on 9/30/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTAPISettingsItem.h>

NS_ASSUME_NONNULL_BEGIN

/// Represents a single toggleable setting.
@interface AUTAPISettingsToggleItem : AUTAPISettingsItem

/// True iff the currentValue equals the trueValue
@property (nonatomic, readonly, getter=isOn) BOOL on;

/// The value corresponding to the toggle being ON.
@property (nonatomic, readonly, copy) id<NSCopying> trueValue;

/// The value corresponding to the toggle being OFF.
@property (nonatomic, readonly, copy) id<NSCopying> falseValue;

@end

NS_ASSUME_NONNULL_END
