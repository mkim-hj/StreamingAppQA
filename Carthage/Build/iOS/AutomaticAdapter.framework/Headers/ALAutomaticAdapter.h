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

#import <AutomaticAdapter/ALAuthStatus.h>
#import <ExternalAccessory/ExternalAccessory.h>
#import <Foundation/Foundation.h>

@class AUTClient;
@class ALELM327Session;
@class ALAdapterDetector;

/**  An interface through which the app can discover an Automatic Adapter and
     open an ELM 327 session that has access to the adapter's restricted data,
     which includes any OBD data.  As an alternative, the EAAccessory(AL)
	 category provides a convenience interface that wraps this one to return
	 low-level EAAccessory and EASession interfaces.  Both options are
	 illustrated below.

     To determine if an EAAccessory is an Automatic Adapter, ask it for an
	 ALAutomaticAdapter interface like this:
     @code
     #import <AutomaticAdapter/AutomaticAdapter.h>
         .
         .
         .
     ALAutomaticAdapter * myAutomaticAdapter = [myEAAccessory asAutomaticAdapter];
     if (myAutomaticAdapter) {
         // The EAAccessory is an Automatic Adapter.  Use this interface to
         // gain access to its engine data as shown below...
     }
     @endcode


     To open an ELM 327 session with access to the adapter's engine data, do
     this:
     @code
     #import <AutomaticAdapter/AutomaticAdapter.h>
         .
         .
         .
     [myAutomaticAdapter
         openAuthorizedSessionForAutomaticClient:myAutomaticClientID
         allowingUserInteraction:YES
         onStatus:^(ALAuthStatus inAuthStatus, NSError *inError) {
 
             // Monitor authentication status and handle any errors here...
 
         }
         onAuthorized:^(ALELM327Session *inSession, NSString *inGreeting) {

             // This block is called if authentication succeeds.  The ALELM327Session can now
             // be used to communicate with the adapter and access engine data.  For example:
		 
             [inSession sendLine:@"01 00" onResponse:^(NSString *inCommand, NSString *inResponse, NSError *inError) {
                 // Check inError and use inResponse here...
             }];
 
             // Retain inSession to continue using it after this block returns.
        }
         onClosed:^(NSError *inError) {
 
             // This block is called when the session ends (unless it was detached as
             // shown below)
 
         }];
     @endcode


     Alternatively, use the interface provided by EAAccessory(AL) category to 
	 get an open EASession with access to the adapter's engine data like this:
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
                     // Retain inSession to continue using it after this block returns.
 
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
     - (BOOL)
     application:(UIApplication *)inApplication
     openURL:(NSURL *)inUrl
     sourceApplication:(NSString *)inSourceApplication
     annotation:(id)inAnnotation
     {
         BOOL    isAdapterAuthorizationURL = [ALAutomaticAdapter
                     authorizeAdapterForURL:inUrl
                     withSecret:myAutomaticClientSecret
                     onStatus:^(ALAuthStatus inAuthStatus, NSError *inError) {
 
                         // Monitor authorization status and handle any errors here...
 
                     }
                     onAuthorized:^(ALAutomaticAdapter *inAdapter, NSString *inClientID, ALAutomaticAdapterOnAuthorizationStatus inStatusCallback) {

                         // This block is called if authorization succeeds.  The app now has
                         // keychain credentials to access engine data on the adapter.  Pass
                         // inAdapter to whatever code handles a discovered Automatic Adapter.
                         // Retain inAdapter to continue using it after this block returns.

                     }];

         return isAdapterAuthorizationURL;
     }
     @endcode


     Alternatively, the EAAccessory(AL) category can process an authorization
	 URL and provide an EAAccessory instead of an ALAutomaticAdapter when it
	 completes:
     @code
     #import <AutomaticAdapter/AutomaticAdapter.h>
         .
         .
         .
     - (BOOL)
     application:(UIApplication *)inApplication
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
                         // inAdapter to whatever code handles a discovered adapter.  Retain
                         // inAdapter to continue using it after this block returns.

                     }];

         return isAdapterAuthorizationURL;
     }
     @endcode
 
     @attention  This SDK recognizes URLs with the scheme

                     automatic-@a<client_id>

                 where @a<client_id> is the app's Automatic client ID, as
                 assigned by Automatic.  Log in to
                 https://developer.automatic.com/ to find this ID.  <b>The app
                 developer must declare this scheme in two places for
                 authorization to work:</b>

                     1. The app's settings at developer.automatic.com must
                        define a redirect URL that uses this scheme.  The
                        authority and path components of the URL are ignored.

                     2. The Info panel for the app's target settings in Xcode
                        must declare this scheme as one of its URL Types.
                        (Or, equivalently, the scheme can be declared in the
                        app's Info plist.)
   
*/
@interface ALAutomaticAdapter : NSObject

/// The underlying EAAccessory
@property (nonatomic, readonly) EAAccessory *accessory;

//** ALAutomaticAdapterOnDetected
///
///  Called when one or more Automatic Adapters are connected that match the
///  filter criteria, if any
///
///  @param      inAdapterAccessories
///              an array of (usually one) ALAutomaticAdapters
///
typedef void (^ALAutomaticAdapterOnDetected)(NSArray *inAdapters);

//** ALAutomaticAdapterOnAuthorizationStatus
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
typedef void (^ALAutomaticAdapterOnAuthorizationStatus)(ALAuthStatus inAuthStatus, NSError *inError);

//** ALAutomaticAdapterOnAdapterAuthorized
///
///  Called when authorization completes if inAdapter was successfully
///  authorized
///
///  @param      inAdapter
///              the authorized adapter.  Authorization is a one-time process
///              per adapter.  Future attempts to open an authorized session
///              with this adapter should succeed without requiring user
///              interaction.  Retain this object to continue using it after the
///              block returns.
///
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
typedef void (^ALAutomaticAdapterOnAdapterAuthorized)(ALAutomaticAdapter *inAdapter, NSString *inClientID, ALAutomaticAdapterOnAuthorizationStatus inStatusCallback);

//** ALAutomaticAdapterOnAppAuthenticated
///
///  Called with an open ALELM327Session when authentication with an Automatic
///  Adapter succeeds
///
///  @param      inSession
///              an open ALELM327Session that the app can use to communicate
///              with the adapter and access privileged engine data.  Its
///              streams will already be open and scheduled in the current run
///              loop.
///
///  @param      inGreeting
///              the string received from the ELM 327 interface upon connection
///
typedef void (^ALAutomaticAdapterOnAppAuthenticated)(ALELM327Session *inSession, NSString *inGreeting);

//** ALAutomaticAdapterOnSessionClosed
///
///  Called when an ALELM327Session ends
///
///  @param      inError	the error, if any, that caused the session to end
///
typedef void (^ALAutomaticAdapterOnSessionClosed)(NSError *inError);

///  Wrap an EAAccessory in an ALAutomaticAdapter interface if the accessory
///  is indeed an Automatic Adapter
///
///  @param      inAccessory
///              an EAAccessory
///
///  @returns	an ALAutomaticAdapter that accesses the Automatic Adapter bound
///             to inAccessory, or nil if inAccessory is bound to something
///             other than an Automatic Adapter.
///
///  @see       EAAccessory(AL) -asAutomaticAdapter,
///             EAAccessory(AL).isAutomaticAdapter
///
+ (instancetype)adapterWithAccessory:(EAAccessory *)inAccessory;

///  Create an ALAdapterDetector that watches for Automatic Adapters matching
///  a specified PIN.  This method presents the accessory picker when it is
///  first called if no matching adapters are connected.
///
///  @param      inAdapterPIN
///              the PIN to watch for.  PIN matching is not exact, but the
///              detector is able to exclude most adapters that could not
///              have this PIN.
///
///  @param      inOnDetected
///              Called whenever a matching adapter becomes available (until the
///              ALAdapterDetector is released).  This block will never be
///              called before the ALAdapterDetector is returned.  The app
///              should release the ALAdapterDetector during this block if it
///              does not want to receive more adapters.
///
///  @returns    an ALAdapterDetector, which the app must retain until it no
///              longer wants inOnDetected to receive callbacks.
///
+ (ALAdapterDetector *)watchForAdapterWithPIN:(NSString *)inAdapterPIN
                                   onDetected:(ALAutomaticAdapterOnDetected)inOnDetected;

///  Open an ELM 327 session with this adapter and conduct an authentication
///  handshake to gain access to its engine data.  If the app doesn't have
///  credentials to access the data, and if interaction is allowed, this
///  method will ask the user to authorize access.  This interaction happens
///  once per adapter and might (currently DOES) require switching to Safari to
///  present the authorization form.  This switch may cause the current app to
///  quit.  The current app will be launched again (if it isn't already running)
///  to open an authorization URL if the user okays access to the adapter.  The
///  application delegate's openURL hander should call
///  +authorizeAdapterForURL:withSecret:onStatus:onAuthorized: to process this
///  URL.  Once authorization completes, the credentials needed to access the
///  adapter are saved on the app's keychain and further calls to this method
///  should succeed without user interaction.
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
///              ALAutomaticAdapterOnAuthorizationStatus and ALAuthStatus for
///              more details.  May be nil.
///
///  @param      inOnAuthorized
///              Called if and only if authentication succeeds.  Must not be
///              nil.  The app can use the ALELM327Session passed to this block
///              to communicate with the adapter and access engine data.  Access
///              to engine data will persist until the ALELM327Session (or its
///              underlying EASession) is closed.  Once closed,
///              -openAuthorizedSessionForAutomaticClient... must be called
///              again to access engine data.  See the description of
///              ALAutomaticAdapterOnAppAuthenticated for more details.
///
///  @param      inOnClosed
///              If inOnAuthorized is called, then this block will eventually be
///              called when the session ends.  See the description of
///              ALAutomaticAdapterOnSessionClosed for more details.  May be
///              nil.
///
///  @see        ALAutomaticAdapterOnAuthorizationStatus,
///              ALAutomaticAdapterOnAppAuthenticated,
///              ALAutomaticAdapterOnSessionClosed,
///              ALAuthStatus,
///              +authorizeAdapterForURL:withSecret:onStatus:onAuthorized:
///              EAAccessory(AL) -openAuthorizedSessionForAutomaticClient:allowingUserInteraction:onStatus:onAuthorized:onClosed:
///
- (void)openAuthorizedSessionForAutomaticClient:(NSString *)inClientID
                        allowingUserInteraction:(BOOL)inInteractionAllowed
                                       onStatus:(ALAutomaticAdapterOnAuthorizationStatus)inOnStatus
                                   onAuthorized:(ALAutomaticAdapterOnAppAuthenticated)inOnAuthorized
                                       onClosed:(ALAutomaticAdapterOnSessionClosed)inOnClosed;

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
///              ALAutomaticAdapterOnAuthorizationStatus and ALAuthStatus for
///              more details.
///
///  @param      inOnAuthorized
///              Called if and only if this method was able to obtain access
///              credentials, install the corresponding authorization token on
///              the adapter, and save the credentials on the app's keychain.
///              This is a one-time process per adapter.  Once the app is
///              authorized to access an Automatic Adapter, further attempts to
///              access that adapter should succeed without user interaction.
///              See the description of ALAutomaticAdapterOnAdapterAuthorized
///              for more details.  This parameter must not be nil.
///
///  @returns    YES if this method accepts inURL, otherwise NO.  If YES, the
///              method will attempt to obtain credentials in the background.
///              Otherwise it does nothing.
///
+ (BOOL)authorizeAdapterForURL:(NSURL *)inURL
                    withSecret:(NSString *)inClientSecret
                      onStatus:(ALAutomaticAdapterOnAuthorizationStatus)inOnStatus
                  onAuthorized:(ALAutomaticAdapterOnAdapterAuthorized)inOnAuthorized;

///  Obtain access credentials for an Automatic Adapter using an OAuth token.
///
///  @param      inToken
///              An OAuth token that grants the current app at least
///              scope:public and scope:adapter:basic access to the account that
///              owns this adapter.
///
///  @param      inTokenType
///              The OAuth token type.
///
///  @param      inOnStatus
///              Called with status updates as the authorization sequence
///              progresses.  See the description of
///              ALAutomaticAdapterOnAuthorizationStatus and ALAuthStatus for
///              more details.
///
///  @param      inOnAuthorized
///              Called if and only if this method was able to obtain access
///              credentials, install the corresponding authorization token on
///              the adapter, and save the credentials on the app's keychain.
///              This is a one-time process per adapter.  Once the app is
///              authorized to access an Automatic Adapter, further attempts to
///              access that adapter should succeed without user interaction.
///              See the description of ALAutomaticAdapterOnAdapterAuthorized
///              for more details.  This parameter must not be nil.
///
///  @returns    NO if an authorization attempt for this adapter is already in
///              progress, otherwise YES.  If YES, this method will attempt to
///              obtain credentials in the background.  Otherwise it does nothing.
///
- (BOOL)authorizeWithOAuthToken:(NSString *)inToken
                      tokenType:(NSString *)inTokenType
                       onStatus:(ALAutomaticAdapterOnAuthorizationStatus)inOnStatus
                   onAuthorized:(ALAutomaticAdapterOnAdapterAuthorized)inOnAuthorized;

///  Get an ALAutomaticAdapter interface to a currently connected adapter by ID,
///  if that adapter is available
///
///  @param      inAdapterID
///              the target adapter's unique ID
///
///  @returns	 an ALAutomaticAdapter that accesses the specified adapter, or
///              nil if no such adapter is connected
///
+ (ALAutomaticAdapter *)connectedAdapterWithID:(NSString *)inAdapterID;

///  @returns	 the protocol string used to open a data session with an
///              Automatic Adapter
///
+ (NSString *)protocolID;

@end
