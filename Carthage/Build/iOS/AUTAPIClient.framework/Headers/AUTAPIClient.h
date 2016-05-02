//
//  AUTAPIClient.h
//  AUTAPIClient
//
//  Created by Robert Böhnke on 25/03/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <Foundation/Foundation.h>

//! Project version number for AUTAPIClient.
FOUNDATION_EXPORT double AUTAPIClientVersionNumber;

//! Project version string for AUTAPIClient.
FOUNDATION_EXPORT const unsigned char AUTAPIClientVersionString[];

// OAuth 2
#import <AUTAPIClient/AFHTTPRequestSerializer+OAuth2.h>
#import <AUTAPIClient/AFOAuth2SessionManager.h>
#import <AUTAPIClient/RACSignal+AUTAuthenticationOperators.h>

// AFNetworking Extensions
#import <AUTAPIClient/AFHTTPSessionManager+AUTReactiveCocoaAdditions.h>
#import <AUTAPIClient/AFHTTPSessionManager+AUTHTTPRequest.h>
#import <AUTAPIClient/AFHTTPSessionManager+AUTModel.h>
#import <AUTAPIClient/AFNetworkReachabilityManager+AUTReactiveAdditions.h>

// ReactiveCococa Extensions
#import <AUTAPIClient/RACSignal+AUTRetryRequest.h>

// Core
#import <AUTAPIClient/VALValet+Automatic.h>
#import <AUTAPIClient/AUTAPIAddress.h>
#import <AUTAPIClient/AUTBase64Transformer.h>
#import <AUTAPIClient/AUTHexadecimalTransformer.h>
#import <AUTAPIClient/AUTClient+AUTAPIDevice.h>
#import <AUTAPIClient/AUTClient+AUTAPITrip.h>
#import <AUTAPIClient/AUTClient+AUTAPIUser.h>
#import <AUTAPIClient/AUTClient+AUTAPIVehicle.h>
#import <AUTAPIClient/AUTClient+AUTCredentialOperations.h>
#import <AUTAPIClient/AUTClient.h>
#import <AUTAPIClient/AUTAPIDevice.h>
#import <AUTAPIClient/AUTAPIFirmware.h>
#import <AUTAPIClient/AUTAPIFirmwareVersion.h>
#import <AUTAPIClient/AUTAPIVehicle.h>
#import <AUTAPIClient/AUTAPIVehicleSettings.h>
#import <AUTAPIClient/AUTHTTPResponseCodes.h>
#import <AUTAPIClient/AUTLicensePlusBadge.h>
#import <AUTAPIClient/AUTLicensePlusBadgeState.h>
#import <AUTAPIClient/AUTLicensePlusCoachInvitation.h>
#import <AUTAPIClient/AUTLicensePlusPerson.h>
#import <AUTAPIClient/AUTLicensePlusProgram.h>
#import <AUTAPIClient/AUTLicensePlusProgramState.h>
#import <AUTAPIClient/AUTAPILocation.h>
#import <AUTAPIClient/AUTAPIObject.h>
#import <AUTAPIClient/AUTAPIDeletable.h>
#import <AUTAPIClient/AUTAPICreatableObject.h>
#import <AUTAPIClient/AUTAPIPage.h>
#import <AUTAPIClient/AUTAPIPage+AUTPaginationDirection.h>
#import <AUTAPIClient/AUTAPIPark.h>
#import <AUTAPIClient/AUTAPIParkTimer.h>
#import <AUTAPIClient/AUTAPIParkPhoto.h>
#import <AUTAPIClient/AUTHTTPRequest.h>
#import <AUTAPIClient/AUTHTTPMultipartRequest.h>
#import <AUTAPIClient/AUTAPITrip.h>
#import <AUTAPIClient/AUTAPILogChunk.h>
#import <AUTAPIClient/AUTAPIUser.h>
#import <AUTAPIClient/AUTAPIVehicleIgnitionStatus.h>
#import <AUTAPIClient/AUTAPIMobileVehicle.h>
#import <AUTAPIClient/AUTLog.h>
#import <AUTAPIClient/AUTAPIRealTimeEvent.h>
#import <AUTAPIClient/AUTAPISettingsObject.h>
#import <AUTAPIClient/AUTAPISettingsPage.h>
#import <AUTAPIClient/AUTAPISettingsGroup.h>
#import <AUTAPIClient/AUTAPISettingsItem.h>
#import <AUTAPIClient/AUTAPISettingsSelectItem.h>
#import <AUTAPIClient/AUTAPISettingsToggleItem.h>
#import <AUTAPIClient/AUTClient+AUTAPISettings.h>
#import <AUTAPIClient/AUTMediaCreationResults.h>
#import <AUTAPIClient/AUTClient+AUTMedia.h>
#import <AUTAPIClient/AUTClient+AUTAPIMobileVehicle.h>
#import <AUTAPIClient/AUTMobileAppInstance.h>
#import <AUTAPIClient/AUTClient+AUTMobileAppInstance.h>
#import <AUTAPIClient/AUTAPITimelineDayGroup.h>
#import <AUTAPIClient/AUTAPITimelineDayGroupItem.h>
#import <AUTAPIClient/AUTAPITimelineDayGroupTripsItem.h>
#import <AUTAPIClient/AUTClient+AUTAPITimeline.h>
#import <AUTAPIClient/AUTAPITripsDayGroup.h>
#import <AUTAPIClient/AUTAPITripsDayGroupTrip.h>
#import <AUTAPIClient/AUTClient+AUTAPITripsDayGroup.h>
#import <AUTAPIClient/AUTClient+AUTHTTPRequestOperators.h>
#import <AUTAPIClient/AUTAPITripDetails.h>
#import <AUTAPIClient/AUTClient+AUTAPITripDetails.h>
#import <AUTAPIClient/AUTClient+AUTAPIFuelFillUpDetail.h>
#import <AUTAPIClient/AUTClient+AUTAPIFuelFillUps.h>
#import <AUTAPIClient/AUTClient+AUTCarHealthSummary.h>
#import <AUTAPIClient/AUTAPIFuelFillUpDetail.h>
#import <AUTAPIClient/AUTAPIFuelFillUpState.h>
#import <AUTAPIClient/AUTAPIFuelFillUpRecord.h>
#import <AUTAPIClient/AUTAPICarHealthSummaryItem.h>
#import <AUTAPIClient/AUTAPIViewDestination.h>
#import <AUTAPIClient/AUTAPIFuelFillUpsDestination.h>
#import <AUTAPIClient/AUTAPIFuelFillUpDetailDestination.h>
#import <AUTAPIClient/AUTAPIMobileFuelStation.h>
#import <AUTAPIClient/AUTAPIMobileFuelGrade.h>
#import <AUTAPIClient/AUTAPIMobileFuelVolumeUnitType.h>
#import <AUTAPIClient/AUTClient+AUTAPIMobileFuelStation.h>
#import <AUTAPIClient/AUTAPIMobileFuelFillUp.h>
#import <AUTAPIClient/AUTClient+AUTAPIMobileFuelFillUp.h>
#import <AUTAPIClient/AUTAPIMobileFuelVolumeUnitTypeValueTransformer.h>

// License Plus
#import <AUTAPIClient/AUTLicensePlusClient.h>
#import <AUTAPIClient/AUTLicensePlusClient+AUTSession.h>
#import <AUTAPIClient/AUTLicensePlusClient+AUTLicensePlus.h>

// Legacy
#import <AUTAPIClient/AUTClient+Legacy.h>
#import <AUTAPIClient/AUTAPILegacyResponse.h>
#import <AUTAPIClient/AUTClient+AUTLegacyCredentialOperations.h>
#import <AUTAPIClient/AUTLegacyCredential.h>
#import <AUTAPIClient/AUTAPIEncryptionData.h>
#import <AUTAPIClient/AUTClient+AUTAPIEncryptionData.h>
#import <AUTAPIClient/AUTAPILegacyVehicle.h>
#import <AUTAPIClient/AUTClient+AUTAPILegacyVehicle.h>
#import <AUTAPIClient/AUTAPIClientErrors.h>
#import <AUTAPIClient/AUTClient+AUTLogChunkUpload.h>
#import <AUTAPIClient/AUTClient+AUTAPIRealTimeEvent.h>
#import <AUTAPIClient/AUTAPILegacyLinkInfo.h>
#import <AUTAPIClient/AUTClient+AUTAPILegacyLinkInfo.h>
