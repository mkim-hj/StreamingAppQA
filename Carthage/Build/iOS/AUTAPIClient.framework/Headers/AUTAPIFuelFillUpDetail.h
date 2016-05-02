//
//  AUTAPIFuelFillUpDetail.h
//  AUTAPIClient
//
//  Created by Engin Kurutepe on 25/02/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

@import Mantle;

#import <AUTAPIClient/AUTAPIObject.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTAPIFuelFillUpDetail : AUTAPIObject

@property (readonly, nonatomic, copy) NSDate *date;
@property (readonly, nonatomic, copy) NSString *fuelGrade;
@property (readonly, nonatomic, copy) NSString *fuelTotalCost;
@property (readonly, nonatomic, copy) NSString *fuelUnitPrice;
@property (readonly, nonatomic, copy) NSString *fuelVolume;
@property (readonly, nonatomic, copy) NSString *stationAddress;
@property (readonly, nonatomic, copy) NSString *stationName;
@property (readonly, nonatomic, copy) NSString *title;
@property (nullable, readonly, nonatomic, copy) NSURL *stationIcon;

@end

NS_ASSUME_NONNULL_END
