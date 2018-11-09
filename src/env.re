[%bs.raw {| require('dotenv').config() |}];

[@bs.val] external getSentryUrl : Js.Nullable.t(string) = "process.env.SENTRY_URL";

let sentryUrl = {
  switch (getSentryUrl |> Js.Nullable.toOption) {
  | None =>
    Js.log("No sentry url set");
    ""
  | Some(x) => x
  }
};
