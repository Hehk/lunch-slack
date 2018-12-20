Dotenv.config();

let getEnv = (name) => {
  switch (Node.Process.process##env |> Js.Dict.get(_, name)) {
  | None =>
    Js.log("Environment variable " ++ name ++ " not provided!");
    ""
  | Some(x) => x
  }
}

let sentryUrl = getEnv("SENTRY_URL");
let yelpApiKey = getEnv("YELP_API_KEY");
