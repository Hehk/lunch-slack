open Micro;

Sentry.init(~dsn=Env.sentryUrl);

let make = make(~handler=(~req, ~res) => {
  switch (req.path) {
  | ["command", "lunch"] => CommandLunch.handler(~req, ~res) -> ignore;
  | ["ping", ..._] => Response.sendString(~res, "pong")
  | _ => Response.sendString(~res, "Hello World")
  }
});

