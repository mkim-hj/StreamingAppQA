/*****************************************************************************
**
**  Automatic Labs - CONFIDENTIAL
**
**  Unpublished Copyright (c) 2009-2016 AUTOMATIC LABS, All Rights Reserved.
**
**  NOTICE:
**
**  All information contained herein is, and remains the property of AUTOMATIC LABS.
**  The intellectual and technical concepts contained herein are proprietary to AUTOMATIC LABS
**  and may be covered by U.S. and Foreign Patents, patents in process, and are protected by trade
**  secret or copyright law.
**
**  Dissemination of this information or reproduction of this material is strictly forbidden unless
**  prior written permission is obtained from AUTOMATIC LABS.  Access to the source code contained
**  herein is hereby forbidden to anyone except current AUTOMATIC LABS employees, managers or
**  contractors who have executed Confidentiality and Non-disclosure agreements explicitly covering
**  such access.
**
**  The copyright notice above does not evidence any actual or intended publication or disclosure of
**  this source code, which includes information that is confidential and/or proprietary, and is a
**  trade secret, of AUTOMATIC LABS. ANY REPRODUCTION, MODIFICATION, DISTRIBUTION, PUBLIC PERFORMANCE,
**  OR PUBLIC DISPLAY OF OR THROUGH USE OF THIS SOURCE CODE WITHOUT THE EXPRESS WRITTEN CONSENT OF
**  AUTOMATIC LABS IS STRICTLY PROHIBITED, AND IN VIOLATION OF APPLICABLE LAWS AND INTERNATIONAL TREATIES.
**  THE RECEIPT OR POSSESSION OF THIS SOURCE CODE AND/OR RELATED INFORMATION DOES NOT CONVEY OR IMPLY
**  ANY RIGHTS TO REPRODUCE, DISCLOSE OR DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE, USE, OR SELL
**  ANYTHING THAT IT MAY DESCRIBE, IN WHOLE OR IN PART.
**
******************************************************************************/

@import Foundation;

#import <AutomaticAdapter/ALAutomaticAdapter.h>
#import <ExternalAccessory/ExternalAccessory.h>

/**  Extends the EAAccessory interface with methods to open an ELM 327 session
	 with an Automatic Adapter with access to the adapter's restricted data,
     which includes any OBD data.  This is a convenience interface that wraps
	 ALAutomaticAdapter to return low-level EAAccessory and EASession
	 interfaces.  Use ALAutomaticAdapter to get the same functionality with
	 higher-level interfaces.

     To open an ELM 327 session with access to the adapter's engine data, do
     this:
     @code
     #import <AutomaticAdapter/AutomaticAdapter.h>
         .
         .
         .
     BOOL    isAutomaticAdapter = [myEAAccessory
                 openAuthorizedSessionForAutomaticClient:myAutomaticClientID
                 allowingUserInteraction:YES
                 onStatus:^(ALAuthStatus inAuthStatus, NSError *inError) {
 
                     // Monitor authentication status and handle any errors here...
 
                 }
                 onAuthorized:^(EASession *inSession, NSString *inGreeting) {

                     // This block is called if authentication succeeds.  The EASession can now
                     // be used to communicate with the adapter and access engine data.  Its
                     // streams will already be open and scheduled in the current run loop.
 
                 }];
     @endcode


     If the app isn't authorized to access the adapter, the
     -openAuthorizedSession... method will open an authorization form in Safari
     to ask the user for access.  If the user grants access, Safari will call
     the app back with an authorization URL.  The application delegate's openURL
     handler should call ALAutomaticAdapter to handle any potential 
     authorization URL like this:
     @code
     #import <AutomaticAdapter/AutomaticAdapter.h>
         .
         .
         .
     - (BOOL) application:(UIApplication *)inApplication
		 openURL:(NSURL *)inUrl
		 sourceApplication:(NSString *)inSourceApplication
		 annotation:(id)inAnnotation
     {
         BOOL    isAdapterAuthorizationURL = [EAAccessory
                     authorizeAutomaticAdapterForURL:inUrl
                     withSecret:myAutomaticClientSecret
                     onStatus:^(ALAuthStatus inAuthStatus, NSError *inError) {
 
                         // Monitor authorization status and handle any errors here...
 
                     }
                     onAuthorized:^(EAAccessory *inAdapter, NSString *inClientID, ALAutomaticAdapterOnAuthorizationStatus inStatusCallback) {

                         // This block is called if authorization succeeds.  The app now has
                         // keychain credentials to access engine data on the adapter.  Pass
                         // inAdapter to whatever code handles a discovered adapter.

                     }];

         return isAdapterAuthorizationURL;
     }
     @endcode
*/
@interface EAAccessory (AL)

/// YES if the accessory is an Automatic Adapter.  Otherwise, NO
@property (nonatomic, readonly) BOOL isAutomaticAdapter;

//** ALAccessoryOnAuthorizationStatus
///
///  Called with status updates during adapter authorization or ELM 327 session
///  authentication
///
///  @param      inAuthStatus
///              an ALAuthStatus code indicating the stage of authorization or
///              authentication progress
///
///  @param      inError
///              an error, if any, or nil.  Authorization has stopped if this is
///              not nil.
///
typedef void (^ALAccessoryOnAuthorizationStatus)(ALAuthStatus inAuthStatus, NSError *inError);

//** ALAccessoryOnAdapterAuthorized
///
///  Called when authorization completes if inAdapter was successfully
///  authorized
///
///  @param      inAdapter
///              an EAAccessory interface to the authorized adapter.
///              Authorization is a one-time process per adapter.  Future
///              attempts to open an authorized session with this adapter should
///              succeed without requiring user interaction.
///
///  @param      inClientID
///              the Automatic client ID of the present app, which was used to
///              obtain the authorization.  If the app opens a new session
///              immediately upon adapter authorization, it can pass this client
///              ID to the open method.
///
///  @param      inStatusCallback
///              the block that was used to track authorization status.  If the
///              app opens a new session immediately upon adapter authorization,
///              it can pass this block to the open method if it wants to
///              continue using the same status callback that was used for
///              authorization.
///
typedef void (^ALAccessoryOnAdapterAuthorized)(EAAccessory *inAdapter, NSString *inClientID, ALAccessoryOnAuthorizationStatus inStatusCallback);

//** ALAccessoryOnAppAuthenticated
///
///  Called with an open EASession when authentication with an Automatic
///  Adapter succeeds
///
///  @param      inSession
///              an EASession that the app can use to communicate with the
///              adapter and access privileged engine data.  Its streams will
///              already be open and scheduled in the current run loop.
///
///  @param      inGreeting
///              the string received from the ELM 327 interface upon connection
///
typedef void (^ALAccessoryOnAppAuthenticated)(EASession *inSession, NSString *inGreeting);

///  @returns    an ALAutomaticAdapter interface to the accessory if it is an
///              Automatic Adapter, or nil if it isn't
///
- (ALAutomaticAdapter *)asAutomaticAdapter;

///  If this accessory is an Automatic Adapter then open an ELM 327 session with
///  it and conduct an authentication handshake to gain access to its engine
///  data.  Otherwise, do nothing and return NO.  If the app doesn't have
///  credentials to access the data, and if interaction is allowed, this method
///  will ask the user to authorize access.  This interaction happens once per
///  adapter and might (currently DOES) require switching to Safari to present
///  the authorization form.  This switch may cause the current app to quit.
///  The current app will be launched again (if it isn't already running) to
///  open an authorization URL if the user okays access to the adapter.  The
///  application delegate's openURL hander should call
///  +authorizeAutomaticAdapterForURL:withSecret:onStatus:onAuthorized: to
///  process this URL.  Once authorization completes, the credentials needed to
///  access the adapter are saved on the app's keychain and further calls to
///  this method should succeed without user interaction.
///
///  @note       This method provides a raw EASession interface to the adapter.
///              when it completes.  To get a high-level ALELM327Session, use
///              -asAutomaticAdapter and ALAutomaticAdapter
///              -openAuthorizedSessionForAutomaticClient:allowingUserInteraction:onStatus:onAuthorized:onClosed:
///              instead.
///
///  @param      inClientID
///              the app's Automatic client ID, as assigned by Automatic.  Log
///              in to https://developer.automatic.com/ to find this ID.
///
///  @param      inInteractionAllowed
///              YES to ask the user for access to the adapter if the app is not
///              authorized to use it.  The interaction will be initiated
///              immediately if the app's keychain has no credentials to
///              access the adapter (though the interaction will proceed
///              asynchronously).  However, interaction can occur later if the
///              app has credentials but the adapter rejects them.  (This
///              wouldn't normally happen.)
///
///  @param      inOnStatus
///              Called with status updates during adapter authorization or ELM
///              327 session authentication.  See the description of
///              ALAccessoryOnAuthorizationStatus and ALAuthStatus for
///              more details.  May be nil.
///
///  @param      inOnAuthorized
///              Called if and only if authentication succeeds.  Must not be
///              nil.  The app can use the EASession passed to this block to
///              communicate with the adapter and access engine data.  Access
///              to engine data will persist until the EASession is closed.
///              Once closed, -openAuthorizedSessionForAutomaticClient... must
///              be called again to access engine data.  See the description of
///              ALAccessoryOnAppAuthenticated for more details.
///
///  @returns    YES if this accessory is an Automatic Adapter.  Otherwise, this
///              method does nothing and returns NO.
///
///  @see        ALAccessoryOnAuthorizationStatus,
///              ALAccessoryOnAppAuthenticated,
///              ALAuthStatus,
///              +authorizeAutomaticAdapterForURL:withSecret:onStatus:onAuthorized:,
///              ALAutomaticAdapter -openAuthorizedSessionForAutomaticClient:allowingUserInteraction:onStatus:onAuthorized:onClosed:
///
- (BOOL)openAuthorizedSessionForAutomaticClient:(NSString *)inClientID
                        allowingUserInteraction:(BOOL)inInteractionAllowed
                                       onStatus:(ALAccessoryOnAuthorizationStatus)inOnStatus
                                   onAuthorized:(ALAccessoryOnAppAuthenticated)inOnAuthorized;

///  DEPRECATED.
///  Use -openAuthorizedSessionForAutomaticClient:allowingUserInteraction:onStatus:onAuthorized:onClosed:
- (void)getAuthorizedSessionForAutomaticClient:(NSString *)inClientID
                       allowingUserInteraction:(BOOL)inInteractionAllowed
                                      onStatus:(ALAccessoryOnAuthorizationStatus)inOnStatus
                                  onAuthorized:(ALAccessoryOnAppAuthenticated)inOnAuthorized
    __attribute__((deprecated));

///  Obtain access credentials for an Automatic Adapter if the given URL
///  is an authorization token, or do nothing and return NO if it isn't.
///  The application delegate's openURL handler should call this method to
///  handle URLs it receives.
///
///  @attention  This method recognizes URLs with the scheme
///
///                  automatic-@a<client_id>
///
///              where @a<client_id> is the app's Automatic client ID, as
///              assigned by Automatic.  Log in to
///              https://developer.automatic.com/ to find this ID.  <b>The app
///              developer must declare this scheme in two places for
///              authorization to work:</b>
///
///                  1. The app's settings at developer.automatic.com must
///                     define a redirect URL that uses this scheme.  The
///                     authority and path components of the URL are ignored.
///
///                  2. The Info panel for the app's target settings in Xcode
///                     must declare this scheme as one of its URL Types.
///                     (Or, equivalently, the scheme can be declared in the
///                     app's Info plist.)
///
///  @note       An authorization URL is generated and delivered as follows:
///
///              - The app first calls
///                -openAuthorizedSessionForAutomaticClient:allowingUserInteraction:onStatus:onAuthorized:onClosed:.
///                If that method can't authenticate with the adapter but user
///                interaction is allowed, it calls the UIApplication instance
///                to open an URL that presents an authorization form.  This
///                normally switches to Safari.
///
///              - If the user okays access to the adapter, then the Automatic
///                server redirects Safari to the app's redirect URL as set at
///                developer.automatic.com.
///
///              - iOS dispatches the redirect URL to the appropriate app, which
///                should be this app if the redirect URL and the app's URL
///                Types are both configured as described above.
///
///              - The application delegate's openURL handler receives the URL
///                and calls this method.
///
///  @note       This method provides an EAAccessory interface to the adapter
///              when it completes.  To get a high-level ALAutomaticAdapter
///              interface, call ALAutomaticAdapter
///              +authorizeAdapterForURL:withSecret:onStatus:onAuthorized:
///              instead.
///
///  @param      inURL
///              any URL received by the application delegate's openURL handler.
///              If the URL is an authorization token, this method starts a
///              background task to obtain access credentials and then returns
///              YES.  Otherwise, it does nothing and returns NO.
///
///  @param      inClientSecret
///              a secret code supplied by Automatic for use only by the
///              assigned app.  Log in to https://developer.automatic.com/ to
///              get this code.
///
///  @param      inOnStatus
///              Called with status updates as the authorization sequence
///              progresses.  See the description of
///              ALAccessoryOnAuthorizationStatus and ALAuthStatus for more
///              details.  May be nil.
///
///  @param      inOnAuthorized
///              Called if and only if this method was able to obtain access
///              credentials, install the corresponding authorization token on
///              the adapter, and save the credentials on the app's keychain.
///              This is a one-time process per adapter.  Once the app is
///              authorized to access an Automatic Adapter, further attempts to
///              access that adapter should succeed without user interaction.
///              See the description of ALAccessoryOnAdapterAuthorized for more
///              details.
///
///  @returns    YES if this method accepts inURL, otherwise NO.  If YES, the
///              method will attempt to obtain credentials in the background.
///              Otherwise it does nothing.
///
///  @see        ALAutomaticAdapter +authorizeAdapterForURL:withSecret:onStatus:onAuthorized:
///
+ (BOOL)authorizeAutomaticAdapterForURL:(NSURL *)inURL
                             withSecret:(NSString *)inClientSecret
                               onStatus:(ALAccessoryOnAuthorizationStatus)inOnStatus
                           onAuthorized:(ALAccessoryOnAdapterAuthorized)inOnAuthorized;

@end
