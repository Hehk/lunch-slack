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
let firebaseApiKey = getEnv("FIREBASE_API_KEY");
let firebaseAuthDomain = getEnv("FIREBASE_AUTH_DOMAIN");
let firebaseDatabaseUrl = getEnv("FIREBASE_DATABASE_URL");
let firebaseStorageBucket = getEnv("FIREBASE_STORAGE_BUCKET");
