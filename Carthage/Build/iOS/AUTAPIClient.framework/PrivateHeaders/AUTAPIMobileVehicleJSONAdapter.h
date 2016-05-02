//
//  AUTAPIMobileVehicleJSONAdapter.h
//  AUTAPIClient
//
//  Created by Eric Horacek on 11/12/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

@import Mantle;

NS_ASSUME_NONNULL_BEGIN

/// A JSON Adapter that omits certain properties that should not be included in
/// updates of mobile vehicles depending on context.
@interface AUTAPIMobileVehicleJSONAdapter : MTLJSONAdapter

@end

NS_ASSUME_NONNULL_END
