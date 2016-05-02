//
//  AUTAPISettingsSelectItem.h
//  AUTAPIClient
//
//  Created by James Lawton on 12/11/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTAPISettingsItem.h>

NS_ASSUME_NONNULL_BEGIN

@class AUTAPISettingsSelectOption;

/// Represents a setting with a value from several options.
@interface AUTAPISettingsSelectItem : AUTAPISettingsItem

@property (nonatomic, readonly, copy) NSArray<AUTAPISettingsSelectOption *> *options;

@end


@interface AUTAPISettingsSelectOption : MTLModel <MTLJSONSerializing>

@property (nonatomic, readonly, copy) id<NSCopying> value;

@property (nonatomic, readonly, copy) NSString *title;

@property (nonatomic, readonly, copy) NSString *subtitle;

@end

NS_ASSUME_NONNULL_END
