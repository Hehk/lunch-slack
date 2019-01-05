open Micro;

Sentry.init(~dsn=Env.sentryUrl);

Firebase.config(
  ~apiKey=Env.firebaseApiKey,
  ~databaseURL=Env.firebaseDatabaseUrl,
  (),
)
|> Firebase.initializeApp;

let stringResponse = s => s->Http.String->Belt.Result.Ok->Future.value;

let make =
  make(~handler=(~req, ~res) => {
    let request: Http.request = {
      path: Micro.Request.getPath(req),
      meth: Micro.Request.getMethod(req),
      text: () => Micro.Request.text(req),
    };

    let response =
      switch (request.path) {
      | ["command", "lunch"] => CommandLunch.handler(request)
      | ["ping", ..._] => "pong" |> stringResponse
      | _ => "Hello World" |> stringResponse
      };

    response
    ->Future.tapOk(
        fun
        | Json(json) => Micro.Response.sendJson(~res, json)
        | String(str) => Micro.Response.sendString(~res, str)
        | None => (),
      )
    ->Future.tapError(msg => {
        Js.log("Error: " ++ msg);
        Micro.Response.sendString(
          ~res,
          ~statusCode=500,
          "Internal server error",
        );
      })
    ->ignore;
  });
