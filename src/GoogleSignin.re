[@bs.deriving abstract]
type config = {
  [@bs.optional]
  scopes: array(string), /* what API you want to access on behalf of the user, default is email and profile */
  [@bs.optional]
  webClientId: string, /* client ID of type WEB for your server (needed to verify user ID and offline access) */
  [@bs.optional]
  offlineAccess: bool, /* if you want to access Google API on behalf of the user FROM YOUR SERVER */
  [@bs.optional]
  hostedDomain: string, /* specifies a hosted domain restriction */
  [@bs.optional]
  loginHint: string, /* [iOS] The user's ID, or email address, to be prefilled in the authentication UI if possible. [See docs here](https://developers.google.com/identity/sign-in/ios/api/interface_g_i_d_sign_in.html#a0a68c7504c31ab0b728432565f6e33fd) */
  [@bs.optional]
  forceConsentPrompt: bool, /* [Android] if you want to show the authorization prompt at each login */
  [@bs.optional]
  accountName: string, /* [Android] specifies an account name on the device that should be used */
  [@bs.optional]
  iosClientId: string /* [iOS] optional, if you want to specify the client ID of type iOS (otherwise, it is taken from GoogleService-Info.plist) */
};

module User = {
  type t;

  [@bs.get] external email: t => string = "email";
  [@bs.get] external id: t => string = "id";
  [@bs.get] external givenName: t => string = "givenName";
  [@bs.get] external familyName: t => string = "familyName";
  [@bs.get] external photo: t => string = "photo"; /* url */
  [@bs.get] external name: t => string = "name"; /* full name */
};

module UserInfo = {
  type t;

  [@bs.get] external idToken: t => string = "idToken";
  [@bs.get] external serverAuthCode: t => string = "serverAuthCode";
  [@bs.get] external user: t => User.t = "user";

  /* on iOS this is an empty array if no additional scopes are defined */
  [@bs.get] external scopes: t => array(string) = "scopes";
};

module Tokens = {
  type t;

  [@bs.get] external idToken: t => string = "idToken";

  /* Note that using accessToken is discouraged. https://developers.google.com/identity/sign-in/android/migration-guide */
  [@bs.get] external accessToken: t => string = "accessToken";
};

[@bs.deriving abstract]
type playServicesConfig = {showPlayServicesUpdateDialog: bool};

[@bs.module "@react-native-community/google-signin"]
[@bs.scope "GoogleSignin"]
[@bs.val]
external configure: config => unit = "configure";

[@bs.module "@react-native-community/google-signin"]
[@bs.scope "GoogleSignin"]
[@bs.val]
external signIn: unit => Js.Promise.t(UserInfo.t) = "signIn";

[@bs.module "@react-native-community/google-signin"]
[@bs.scope "GoogleSignin"]
[@bs.val]
external signOut: unit => Js.Promise.t(unit) = "signOut";

[@bs.module "@react-native-community/google-signin"]
[@bs.scope "GoogleSignin"]
[@bs.val]
external signInSilently: unit => Js.Promise.t(UserInfo.t) = "signInSilently";

[@bs.module "@react-native-community/google-signin"]
[@bs.scope "GoogleSignin"]
[@bs.val]
external isSignedIn: unit => Js.Promise.t(bool) = "isSignedIn";

[@bs.module "@react-native-community/google-signin"]
[@bs.scope "GoogleSignin"]
[@bs.val]
external getCurrentUser: unit => Js.Promise.t(Js.nullable(UserInfo.t)) =
  "getCurrentUser";
let getCurrentUser: unit => Js.Promise.t(option(UserInfo.t)) =
  () =>
    getCurrentUser()
    |> Js.Promise.then_(x => Js.Promise.resolve(Js.Nullable.toOption(x)));

[@bs.module "@react-native-community/google-signin"]
[@bs.scope "GoogleSignin"]
[@bs.val]
external clearCachedToken: string => Js.Promise.t(bool) = "clearCachedToken";

[@bs.module "@react-native-community/google-signin"]
[@bs.scope "GoogleSignin"]
[@bs.val]
external getTokens: unit => Js.Promise.t(Tokens.t) = "getTokens";

[@bs.module "@react-native-community/google-signin"]
[@bs.scope "GoogleSignin"]
[@bs.val]
external revokeAccess: unit => Js.Promise.t(unit) = "revokeAccess";

[@bs.module "@react-native-community/google-signin"]
[@bs.scope "GoogleSignin"]
[@bs.val]
external hasPlayServices: playServicesConfig => Js.Promise.t(unit) =
  "hasPlayServices";

module GoogleSignInError = {
  type t = {
    .
    "code": string,
    "message": string,
  };
};

[@bs.deriving abstract]
type _statusCodes = {
  [@bs.as "SIGN_IN_CANCELLED"]
  signInCancelled: string,
  [@bs.as "IN_PROGRESS"]
  inProgress: string,
  [@bs.as "PLAY_SERVICES_NOT_AVAILABLE"]
  playServicesNotAvailable: string,
  [@bs.as "SIGN_IN_REQUIRED"]
  signInRequired: string,
};
[@bs.module "@react-native-community/google-signin"]
external _statusCodes: _statusCodes = "statusCodes";

type statusCode =
  | SignInCancelled(string)
  | PlayServicesNotAvailable(string)
  | InProgress(string)
  | SignInRequired(string)
  | Error(string);

external _exnToGoogleSigninError: Js.Promise.error => GoogleSignInError.t =
  "%identity";

let exnToGoogleSigninError = exn => {
  let googleError = _exnToGoogleSigninError(exn);
  let code = googleError##code;
  let message = googleError##message;
  switch (code) {
  | code when code == signInCancelledGet(_statusCodes) =>
    SignInCancelled(message)
  | code when code == inProgressGet(_statusCodes) => InProgress(message)
  | code when code == playServicesNotAvailableGet(_statusCodes) =>
    PlayServicesNotAvailable(message)
  | code when code == signInRequiredGet(_statusCodes) =>
    SignInRequired(message)
  | _ => Error(message)
  };
};

module GoogleSignInButton = {
  type size = [ | `Standard | `Wide | `Icon];
  type color = [ | `Dark | `Light];
  [@bs.module "@react-native-community/google-signin"] [@react.component]
  external make:
    (~size: int=?, ~color: int=?, ~disabled: bool=?, ~onPress: unit => unit) =>
    React.element =
    "GoogleSigninButton";

  let makeProps = (~size=`Standard, ~color=`Light) =>
    makeProps(
      ~size=
        switch (size) {
        | `Standard => 0
        | `Wide => 1
        | `Icon => 2
        },
      ~color=
        switch (color) {
        | `Dark => 0
        | `Light => 1
        },
    );
};