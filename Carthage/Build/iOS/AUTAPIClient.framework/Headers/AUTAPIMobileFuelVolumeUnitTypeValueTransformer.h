//
//  AUTAPIMobileFuelVolumeUnitTypeValueTransformer.h
//  AUTAPIClient
//
//  Created by Sylvain Rebaud on 3/1/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

@import Mantle;

NS_ASSUME_NONNULL_BEGIN

extern NSString * const AUTAPIMobileFuelVolumeUnitTypeStringGallon;
extern NSString * const AUTAPIMobileFuelVolumeUnitTypeStringImperialGallon;
extern NSString * const AUTAPIMobileFuelVolumeUnitTypeStringLiter;

@interface AUTAPIMobileFuelVolumeUnitTypeValueTransformer : NSValueTransformer

+ (NSValueTransformer *)transformer;

@end

NS_ASSUME_NONNULL_END
