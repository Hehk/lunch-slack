type meth =
  | Get
  | Post
  | Put
  | Patch
  | Delete
  | Head
  | Options
  | Invalid;

type response =
  | Json(Js.Json.t)
  | String(string)
  | None;

type request = {
  path: list(string),
  meth,
  text: unit => Future.t(Belt.Result.t(string, string)),
};

