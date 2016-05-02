//
//  AUTAPIFuelFillUpRecord.h
//  AUTAPIClient
//
//  Created by Engin Kurutepe on 19/02/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

@import Mantle;

#import <AUTAPIClient/AUTAPIPage.h>
#import <AUTAPIClient/AUTAPIObject.h>
#import <AUTAPIClient/AUTAPIViewDestination.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTAPIFuelFillUpRecord : AUTAPIObject

@property (readonly, nonatomic, copy) NSDate *date;

@property (readonly, nonatomic, copy) NSString *fuelPrice;

@property (nullable, readonly, nonatomic, copy) NSURL *stationIcon;

@property (readonly, nonatomic, copy) NSString *title;

@property (readonly, nonatomic, copy) NSString *totalCost;

@property (nullable, readonly, nonatomic) AUTAPIViewDestination *destination;

@end

AUTPageSubclassInterface(AUTAPIFuelFillUpRecord)

NS_ASSUME_NONNULL_END
