type meth =
  | Get
  | Post
  | Put
  | Patch
  | Delete
  | Head
  | Options
  | Invalid;

type request = {
  path: list(string),
  meth: meth
};

module Response = {
  type t;
  [@bs.module "micro"] external sendString : (t, int, string) => unit = "send"; 
  [@bs.module "micro"] external sendJson : (t, int, Js.Json.t) => unit = "send"; 
};
module Request = {
  [@bs.deriving abstract]
  type t = pri {
    [@bs.optional] url: string,
    [@bs.optional] [@bs.as "method"] meth: string,
  };
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

  /* This is a slight modification of the router work in ReasonReact 
     https://github.com/reasonml/reason-react/blob/a46fcda65f0847246a493bb2743f216010679b86/src/ReasonReact.re#L796
  */
  let getPath = (req) => {
    switch (urlGet(req)) {
    | None
    | Some("")
    | Some("/") => []
    | Some(raw) => {
        /* remove the preceeding /, which every pathname seems to have */
        let raw = Js.String.sliceToEnd(~from=1, raw);
        /* remove the trailing /, which some pathnames might have. Ugh */
        let raw =
          switch (Js.String.get(raw, Js.String.length(raw) - 1)) {
          | "/" => Js.String.slice(~from=0, ~to_=-1, raw)
          | _ => raw
          };
        raw |> Js.String.split("/") |> Array.to_list;
      }
    }
  };

  let getMethod = (req) => {
    switch (req |> methGet) {
      | None => Invalid;
      | Some(meth) => {
        switch meth {
        | "GET" => Get
        | "POST" => Post
        | "Put" => Put
        | "Patch" => Patch
        | "Delete" => Delete
        | "Head" => Head
        | "Options" => Options
        | _ => Invalid
        }
      }
    }
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

let make = (~handler, rawReq: Request.t, res : Response.t) => {
  let request = {
    path: Request.getPath(rawReq),
    meth: Request.getMethod(rawReq),
  };

  request
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
