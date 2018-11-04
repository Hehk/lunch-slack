module Response = {
  type t;
  [@bs.module "micro"] external sendString : (t, int, string) => unit = "send"; 
  [@bs.module "micro"] external sendJson : (t, int, Js.Json.t) => unit = "send"; 
};
module Request = {
  type t;
  [@bs.deriving abstract]
  type options = {
    limit: Js.Nullable.t(string),
    encoding: Js.Nullable.t(string),
  };
  [@bs.module "micro"] external _text : (t, options) => Js.Promise.t(string) = "text";
  let text = (~limit=?, ~encoding=?, req) => {
    let options = options(~limit=Js.Nullable.fromOption(limit), ~encoding=Js.Nullable.fromOption(encoding));
    _text(req, options);
  };
};

type response =
| String(string)
| Json(Js.Json.t);

let handleResponse = res => fun
| String(str) => Response.sendString(res, 200, str)
| Json(json) => Response.sendJson(res, 200, json);

let makeSync = (~handler, req : Request.t, res : Response.t) => {
  req
  |> handler
  |> handleResponse(res);
};

let make = (~handler, req: Request.t, res : Response.t) => {
  req
  |> handler
  |> Js.Promise.then_(value => {
    handleResponse(res, value);
    Js.Promise.resolve();
  })
  |> ignore;

  /* The returns of this function will be handled by micro unless
     it returns undefined */
  Js.Null_undefined.undefined;
};
