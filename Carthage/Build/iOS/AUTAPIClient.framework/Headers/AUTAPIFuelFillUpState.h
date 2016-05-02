//
//  AUTAPIFuelFillUpState.h
//  AUTAPIClient
//
//  Created by Engin Kurutepe on 19/02/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

@import Mantle;

NS_ASSUME_NONNULL_BEGIN

@class AUTAPIFuelFillUpRecordPage;

@interface AUTAPIFuelFillUpState : MTLModel <MTLJSONSerializing>

@property (readonly, nonatomic, copy) NSString *title;

@property (readonly, nonatomic, copy) NSString *detail;

@property (readonly, nonatomic, copy) AUTAPIFuelFillUpRecordPage *itemsList;

@end

NS_ASSUME_NONNULL_END
