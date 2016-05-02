//
//  RACSequence+NSDictionary.h
//  AUTDeviceManagement
//
//  Created by Blossom Woo on 8/10/15.
//  Copyright (c) 2015 Automatic. All rights reserved.
//

#import <ReactiveCocoa/ReactiveCocoa.h>

@interface RACSequence (NSDictionary)

/// Evaluates the full sequence to produce an equivalently-sized dictionary
/// from the RACTuples
///
/// RACTuple[0] is the key in the dictionary;
/// RACTuple[1] is the value in the dictionary for that key.
@property (nonatomic, copy, readonly) NSDictionary *dictionary;

@end
