//
//  AUTAPIVehicle.h
//  AUTAPIClient
//
//  Created by Eric Horacek on 6/29/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTAPIObject.h>
#import <AUTAPIClient/AUTAPIPage.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTAPIVehicle : AUTAPIObject

@property (readonly, nonatomic, copy) NSString *vin;

@property (readonly, nonatomic, copy, nullable) NSString *make;

@property (readonly, nonatomic, copy, nullable) NSString *model;

@property (readonly, nonatomic, copy, nullable) NSNumber *year;

@property (readonly, nonatomic, copy, nullable) NSString *submodel;

@property (readonly, nonatomic, copy, nullable) NSString *displayName;

@end

AUTPageSubclassInterface(AUTAPIVehicle)

NS_ASSUME_NONNULL_END

