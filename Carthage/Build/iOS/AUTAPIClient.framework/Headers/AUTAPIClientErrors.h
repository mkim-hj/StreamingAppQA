//
//  AUTAPIClientErrors.h
//  AUTAPIClient
//
//  Created by Eric Horacek on 6/18/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import Foundation;

NS_ASSUME_NONNULL_BEGIN

/// The error domain for errors originating with AUTClient.
extern NSString * const AUTClientErrorDomain;

/// Error codes in AUTClientErrorDomain.
typedef NS_ENUM(NSInteger, AUTAPIClientErrorCodes) {
    /// Parsing of an object failed.
    AUTClientErrorParsingFailed,

    /// The provided password was too short.
    AUTClientErrorPasswordTooShort,

    /// The provided password was to long.
    AUTClientErrorPasswordTooLong,

    /// The provided email was too long.
    AUTClientErrorInvalidEmail,

    /// The client was unable to delete the credential during sign out.
    AUTClientErrorUnableToDeleteStoredCredential,

    /// The client was unable to persist the credential.
    AUTClientErrorUnableToStoreCredential,

    /// The client was unable to restore its persisted credential.
    AUTClientErrorUnableToRestoreCredential,

    /// The client was unable to refresh its credential with its refresh token.
    AUTClientErrorUnableToRefreshCredential,

    /// Validation of a model object failed.
    AUTClientErrorValidationFailed,

    /// The client was unable to locate a newton vehicle from a legacy vehicle.
    AUTClientErrorUnableToLocateEquivalentVehicle,
    
    /// The client was unable to serialize an object to JSON
    AUTClientErrorJSONSerializationFailed,

    /// The client was unable to fetch a next page because the host of the next
    /// page URL was unrecognized.
    AUTClientErrorUnrecognizedNextPageURL,

    /// The client was unable to find an uploaded file in the results of a media
    /// creation response.
    AUTClientErrorFileMissingFromMediaResults,

    /// The client was unable to find a nearest station to the provided
    /// coordinate.
    ///
    /// The userInfo dictionary for this code will include
    /// AUTClientErrorNoNearestStationCoordinateKey.
    AUTClientErrorNoNearestStation,
};

/// The coordinate that a nearest station could not be found for.
extern NSString * const AUTClientErrorNoNearestStationCoordinateKey;

NS_ASSUME_NONNULL_END
