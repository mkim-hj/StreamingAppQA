//
//  AUTAPIMobileFuelVolumeUnitType.h
//  AUTAPIClient
//
//  Created by Sylvain Rebaud on 3/1/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

@import Foundation;

/// The volume units for fuel.
typedef NS_ENUM(NSInteger, AUTAPIMobileFuelVolumeUnitType) {
    AUTAPIMobileFuelVolumeUnitTypeUndefined = 0,
    AUTAPIMobileFuelVolumeUnitTypeGallon,
    AUTAPIMobileFuelVolumeUnitTypeImperialGallon,
    AUTAPIMobileFuelVolumeUnitTypeLiter
};
