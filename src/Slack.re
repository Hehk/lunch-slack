open Micro;

Sentry.init(~dsn=Env.sentryUrl);

let make = make(~handler=(~req, ~res) => {
  switch (req.path) {
  | ["slack", ..._] => Response.sendString(~res, "Hello World")
  | ["ping", ...] => Response.sendString(~res, "pong")
  | _ => 
    Sentry.captureEvent(Sentry.event(~message="Invalid Route 2", ()));
    Response.sendString(~res, "Invalid Route");
  }
});

