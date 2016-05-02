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

typedef enum {
    kALAuthStatus_None = 0,

    //** kALAuthStatus_UserInteractionRequired
    ///
    ///  An attempt to authenticate with an Automatic Adapter has encountered
    ///  one of these conditions:
    ///
    ///      - No key to access the adapter has been saved on the keychain;
    ///
    ///      - The adapter rejected login with the saved key; or,
    ///
    ///      - The adapter needs to sync and cannot accept connections until the
    ///        Automatic app connects and syncs.
    ///
    ///  In the case of an access key issue, if user interaction is allowed,
    ///  then the SDK will ask the user to grant the present app access to (at
    ///  least) the target adapter.  This might (currently will) launch Safari
    ///  to present an authorization form, which could cause the present app to
    ///  quit.  If the user grants authorization, then the present app will be
    ///  called back with an URL to complete the authorization process.
    ///
    ///  As of this version of the SDK, the syncing mechanism is a future
    ///  feature, so the app won't encounter that condition for the time being.
    ///  The adapter firmware does not yet signal the need to sync, and a
    ///  mechanism for this SDK to trigger a sync operation does not yet exist.
    ///  However, if this version of the SDK does eventually encounter an
    ///  adapter that signals that it needs to sync, this SDK will report this
    ///  status with error code @c kALError_Unsynced, indicating it can't
    ///  resolve that condition.  If that happens, the app should assume that
    ///  the SDK is out of date and a newer version of the app incorporating the
    ///  latest adapter SDK is required.  In the meantime, the user should be
    ///  able to clear the condition manually by launching the Automatic app and
    ///  connecting to the adapter.
    ///
    ///  An error is reported with this status if user interaction is required
    ///  but not allowed.  The SDK will report this status with no error if it
    ///  will undertake the necessary user interaction to gain access to the
    ///  adapter.
    ///
    kALAuthStatus_UserInteractionRequired,

    //** kALAuthStatus_AccountAccessPrivileges_Acquiring
    ///
    ///  The user granted some account access privileges to the present app and
    ///  the SDK will exchange the resulting grant token for credentials
    ///
    ///  An error with this status can mean:
    ///
    ///     - the HTTPS connection to automatic.com dropped or could not be
    ///       established
    ///
    ///     - this SDK is out of date
    ///
    ///     - the authorization system is broken
    ///
    kALAuthStatus_AccountAccessPrivileges_Acquiring,

    //** kALAuthStatus_AdapterAccessPrivileges_Acquiring
    ///
    ///  The SDK is retrieving an authorization token that will grant the
    ///  present app access to the target adapter
    ///
    ///  An error with this status can mean:
    ///
    ///     - the HTTPS connection to automatic.com dropped or could not be
    ///       established
    ///
    ///     - this SDK is out of date
    ///
    ///     - the authorization system is broken
    ///
    kALAuthStatus_AdapterAccessPrivileges_Acquiring,

    //** kALAuthStatus_AdapterAccessPrivileges_Installing
    ///
    ///  The SDK is installing an authorization token on the adapter that gives
    ///  the present app access to adapter data
    ///
    ///  An error with this status can mean:
    ///
    ///     - the bluetooth connection dropped or could not be established
    ///
    ///     - this SDK or the adapter firmware is out of date
    ///
    ///     - the authorization system is broken
    ///
    ///     - the adapter is improperly provisioned
    ///
    kALAuthStatus_AdapterAccessPrivileges_Installing,

    //** kALAuthStatus_AdapterAccessPrivileges_Installed
    ///
    ///  The adapter is now provisioned to give the present app access to
    ///  adapter data
    ///
    kALAuthStatus_AdapterAccessPrivileges_Installed,

    //** kALAuthStatus_AdapterSession_Authenticating
    ///
    ///  The SDK is negotiating access to privileged adapter data
    ///
    ///  An error with this status can mean:
    ///
    ///     - the bluetooth connection dropped or could not be established
    ///
    ///  If the adapter rejects the authentication attemp, the error is not
    ///  reported with this condition.  Instead, the status progresses to
    ///  @c kALAuthStatus_UserInteractionRequired.
    ///
    kALAuthStatus_AdapterSession_Authenticating,

    //** kALAuthStatus_AdapterSession_Authenticated
    ///
    ///  The present app can now use the current open data session to access
    ///  privileged adapter data.  The session object may be delivered in
    ///  another callback.
    ///
    kALAuthStatus_AdapterSession_Authenticated,
} ALAuthStatus;
