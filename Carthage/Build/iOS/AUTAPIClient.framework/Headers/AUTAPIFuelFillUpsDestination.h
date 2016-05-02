//
//  AUTAPIFuelFillUpsDestination.h
//  AUTAPIClient
//
//  Created by Engin Kurutepe on 26/02/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTAPIViewDestination.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTAPIFuelFillUpsDestination : AUTAPIViewDestination

@property (readonly, nonatomic, copy) NSString *settingsPath;

@property (readonly, nonatomic, copy) NSString *vehicleID;

@end

NS_ASSUME_NONNULL_END
