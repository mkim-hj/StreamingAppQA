//
//  AUTHTTPResponseCodes.h
//  AUTAPIClient
//
//  Created by James Lawton on 2/4/14.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, AUTHTTPResponseCode) {
    // Successful
    HTTP_200_OK = 200,
    HTTP_201_Created = 201,
    HTTP_202_Accepted = 202,
    HTTP_203_Non_Authoritative = 203,
    HTTP_204_No_Content = 204,
    HTTP_205_Reset_Content = 205,
    HTTP_206_Partial_Content = 206,

    // Redirection
    HTTP_300_Multiple_Choices = 300,
    HTTP_301_Moved_Permanently = 301,
    HTTP_302_Found = 302,
    HTTP_303_See_Other = 303,
    HTTP_304_Not_Modified = 304,
    HTTP_305_Use_Proxy = 305,
    HTTP_307_Temporary_Redirect = 307,

    // Client error
    HTTP_400_Bad_Request = 400,
    HTTP_401_Unauthorized = 401, // Actually unauthenticated
    HTTP_402_Payment_Required = 402,
    HTTP_403_Forbidden = 403, // Actually unauthorized
    HTTP_404_Not_Found = 404,
    HTTP_405_Method_Not_Found = 405,
    HTTP_406_Not_Acceptable = 406,
    HTTP_407_Proxy_Authentication_Required = 407,
    HTTP_408_Request_Timeout = 408,
    HTTP_409_Conflict = 409,
    HTTP_410_Gone = 410,
    HTTP_411_Length_Required = 411,
    HTTP_412_Precondition_Failed = 412,
    HTTP_413_Request_Entity_Too_Large = 413,
    HTTP_414_URI_Too_Long = 414,
    HTTP_415_Unsupported_Media_Type = 415,
    HTTP_416_Requested_Range_Not_Satisfiable = 416,
    HTTP_417_Expectation_Failed = 417,

    HTTP_422_Unprocessable_Entity = 422, // Non standard

    // Server error
    HTTP_500_Internal_Server_Error = 500,
    HTTP_501_Not_Implemented = 501,
    HTTP_502_Bad_Gateway = 502,
    HTTP_503_Service_Unavailable = 503,
    HTTP_504_Gateway_Timeout = 504,
    HTTP_505_HTTP_Version_Not_Supported = 505,
};

static inline BOOL AUTHTTPStatusCodeIsSuccess(NSInteger statusCode) {
    return 200 <= statusCode && statusCode < 300;
}
static inline BOOL AUTHTTPStatusCodeIsRedirect(NSInteger statusCode) {
    return 300 <= statusCode && statusCode < 400;
}
static inline BOOL AUTHTTPStatusCodeIsClientError(NSInteger statusCode) {
    return 400 <= statusCode && statusCode < 500;
}
static inline BOOL AUTHTTPStatusCodeIsServerError(NSInteger statusCode) {
    return 500 <= statusCode && statusCode < 600;
}

FOUNDATION_EXPORT NSString * AUTStringFromHTTPStatusCode(NSInteger statusCode);
