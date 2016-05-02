//
//  AUTAPIViewDestination.h
//  AUTAPIClient
//
//  Created by Engin Kurutepe on 26/02/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

@import Mantle;

NS_ASSUME_NONNULL_BEGIN

/// An empty base-class which acts as an entry point to initialize the
/// specific subclass corresponding to the JSON object from the view server.
@interface AUTAPIViewDestination : MTLModel <MTLJSONSerializing>

@end

NS_ASSUME_NONNULL_END
