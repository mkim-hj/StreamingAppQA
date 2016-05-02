//
//  AUTAPICarHealthSummaryItem.h
//  AUTAPIClient
//
//  Created by Engin Kurutepe on 19/02/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTAPIPage.h>

@import Mantle;

#import <AUTAPIClient/AUTAPIViewDestination.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTAPICarHealthSummaryItem : MTLModel <MTLJSONSerializing>

@property (readonly, nonatomic, copy) NSString *title;

@property (readonly, nonatomic, copy) NSString *detail;

@property (nullable, readonly, nonatomic) AUTAPIViewDestination *destination;

@end

AUTPageSubclassInterface(AUTAPICarHealthSummaryItem)

NS_ASSUME_NONNULL_END
