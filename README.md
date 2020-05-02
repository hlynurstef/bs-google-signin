<p align="center">
  <img alt="reason-ml" src="img/reason.png" width="300">
  <img alt="google-signin-button" src="img/google-signin-button.png" width="300">
</p>

<h1 align="center">
  @hlynurstef/bs-google-signin
</h1>

<p align="center">
  <a href="https://reasonml.github.io/">ReasonML</a> bindings for <a href="https://github.com/react-native-community/google-signin">@react-native-community/google-signin</a>
</p>

---

<div align="center">

![npm](https://img.shields.io/npm/v/@hlynurstef/bs-google-signin)
![GitHub last commit](https://img.shields.io/github/last-commit/hlynurstef/bs-google-signin)
![GitHub](https://img.shields.io/github/license/hlynurstef/bs-google-signin)
![PRs Welcome](https://img.shields.io/badge/PRs-Welcome-brightgreen)

</div>

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

## Google Signin Button

```
<GoogleSignin.GoogleSignInButton
  size=`Wide
  color=`Light
  disabled=false
  onPress=(_ => ())
/>
```

# Contributing

1. Fork this repo and clone it onto your machine.
2. Run `yarn install` in the root directory.
3. Start the compiler with `yarn watch` in the root directory.
4. Create a PR to this repo with your changes.
