# bs-google-signin

ReasonML bindings for [@react-native-community/google-signin](https://github.com/react-native-community/google-signin)

# Installation

Use yarn or npm

`yarn add @hlynurstef/bs-google-signin`

Then add `@hlynurstef/bs-google-signin` to `bs-dependencies` in your `bsconfig.json`:

```
{
  ...
  "bs-dependencies": ["@hlynurstef/bs-google-signin"]
}
```

# Example Usage

## Configure

```
GoogleSignin.configure(
  GoogleSignin.config(
    ~webClientId="your-id-here",
    (),
  ),
);
```

## Sign In

```
GoogleSignin.(
  hasPlayServices(playServicesConfig(~showPlayServicesUpdateDialog=true))
  |> Js.Promise.then_(_ => signIn())
  |> Js.Promise.then_((userInfo: UserInfo.t) => {
       Js.Console.log2("idToken:", userInfo->UserInfo.idToken);
       Js.Console.log2("serverAuthCode:", userInfo->UserInfo.serverAuthCode);

       let user = userInfo->UserInfo.user;
       Js.Console.log2("user.id:", user->User.id);
       Js.Console.log2("user.name:", user->User.name);
       Js.Console.log2("user.email:", user->User.email);
       Js.Console.log2("user.photo:", user->User.photo);
       Js.Console.log2("user.givenName:", user->User.givenName);
       Js.Console.log2("user.familyName:", user->User.familyName);
       Js.Promise.resolve();
     })
  |> Js.Promise.catch(exn =>
       switch (exnToGoogleSigninError(exn)) {
       | SignInCancelled(message)
       | PlayServicesNotAvailable(message)
       | InProgress(message)
       | SignInRequired(message)
       | GoogleSignin.Error(message) =>
         Js.Promise.resolve(Js.Console.log2("GoogleSignin failed:", message))
       }
     )
);
```
