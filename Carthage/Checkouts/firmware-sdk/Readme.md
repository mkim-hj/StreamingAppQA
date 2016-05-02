
# Automatic Adapter SDK

The Automatic Adapter provides an ELM327 interface over Bluetooth which allows apps to read engine data from the vehicle the Adapter
is connected to.  Access to this data is restricted, though, using a propietary authentication handshake. Only apps the user has 
authorized can access the data.

This SDK enables apps that are registered on the Automatic platform to get access privileges from the user and conduct the authentication
handshake needed to gain access to Adapter data.

This readme explains...

* [What You'll Need](#what-youll-need)
* [How to Add This SDK to Your App](#how-to-add)
* [Using the SDK](#using-the-sdk)
* [Testing Tips](#testing)
* [Further Documentation](#further-documentation)

<br>

## <a name="what-youll-need"></a>What You'll Need

1. **An Automatic account**

	Install the [Automatic mobile app](https://itunes.apple.com/us/app/automatic/id596594365?mt=8), and use it to create an account.

2. **An Automatic client ID**

	Go to [developer.automatic.com](https://developer.automatic.com) -> [My Apps](https://developer.automatic.com/my-apps/), and click on
	"Create A New Application."  Use the client ID listed on the settings page that appears after creating the app.

3. **Your Automatic client secret**

	Use the above app settings page to reveal your app's client secret.

4. **Your authorization URL scheme**

	Enter the following URI into the OAuth Redirect URL field on your app's settings page, replacing *<client_id>* with your app's client
	ID listed on the same page:
	
	```
	automatic-<client_id>:///
	```
	The scheme, which you'll need below, is the part before the colon.

5. **Access Privileges**
	
	Click "Request Changes to Scopes" on your app's setting page and submit the following request:

	* Place a checkmark by `scope:public`
	* Uncheck any other scopes your app doesn't use.  (None of the other scopes listed is needed for this SDK.)
	* Describe your app in the field provided
	* This SDK is not yet available to the public, so one of the access scopes you'll need is not listed.  Therefore, also include the
	following note along with your app description:

	```
	Please add scope:adapter:basic.
	```

	We'll email you when your access configuration is complete, at which point your app will be able to access Automatic adapters
	using this SDK.

<br>

## <a name="how-to-add"></a>How to Add This SDK to Your App

1. Link to the Automatic Adapter framework:
	1. Open your app project and add AutomaticAdapterSDK.xcodeproj as a subproject.
	2. Go to the Build Phases panel of your app target.
	3. Click [+] under Target Dependencies and add the AutomaticAdapter target of AutomaticAdapterSDK.
	4. Click [+] under Link Binary With Libraries and add AutomaticAdapter.framework.

2. Link to the libraries the framework uses:
	* ExternalAccessory.framework
	* Security.framework
	* UIKit.framework.

3. Declare the authorization URL scheme:
	
	In the Info panel, add a new URL Type.  Set its scheme to the authorization URL scheme discussed in the previous section, and set its role to Editor.

4. Declare the Automatic Adapter protocol:
	
	In the custom properties section of the Info panel, add the string `com.automatic.link.protocol.v2` under "Supported external accessory protocols".

<br>

## <a name="using-the-sdk"></a>Using the SDK
The ALAutomaticAdapter class provides an interface for discovering an Automatic Adapter and opening an ELM327 session that has access to the adapter's engine data.  As an alternative, the EAAccessory(AL) category provides a convenience interface that wraps ALAutomaticAdapter and returns low-level EAAccessory and EASession interfaces.  Both options are illustrated below.

* To determine if an EAAccessory is an Automatic Adapter, ask it for an ALAutomaticAdapter interface like this:


    ```
    #import <AutomaticAdapter/AutomaticAdapter.h>
        .
        .
        .
    ALAutomaticAdapter * myAutomaticAdapter = [myEAAccessory asAutomaticAdapter];
    if (myAutomaticAdapter) {
        // The EAAccessory is an Automatic Adapter.  Use this interface to
        // gain access to its engine data as shown below...
    }
    ```


* To open an ELM327 session with access to the adapter's engine data, do this:

    ```
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
    ```


* Alternatively, use the interface provided by EAAccessory(AL) category to get an open EASession with access to the adapter's engine data like this:

    ```
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
    ```


* If the app isn't authorized to access the adapter, the -openAuthorizedSession... method will open an authorization form in Safari to ask the user for access.  If the user grants access, Safari will call the app back with an authorization URL.  The application delegate's openURL handler should call ALAutomaticAdapter to handle any potential authorization URL like this:

    ```
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
    ```


* Alternatively, the EAAccessory(AL) category can process an authorization URL and provide an EAAccessory instead of an ALAutomaticAdapter when it completes:

    ```
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
    ```

<br>   

## <a name="testing"></a>Testing Tips

**Forcing a Safari Interaction**

The Safari interaction normally happens only once per Adapter (technically, once for a given combination of Adapter, app, and mobile device).  To force another Safari interaction, discard the saved authorization before trying to open a session:

```
	if ([myAccessory isAutomaticAdapter]) {
	    ALAuthorization *   authorization = [ALAuthorization authorizationForAdapter:[myAccessory asAutomaticAdapter]];
	    [authorization discard];
	}
```

<br>

## <a name="further-documentation"></a>Further Documentation

ALAutomaticAdapter.h, ALAuthStatus.h, and EAAccessory+AL.h have AppleDoc comments detailing their interfaces.
Xcode shows these comments in the help inspector when you click in any source file on a symbol declared in one of these headers.
