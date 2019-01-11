open Utils;

let handleResponseStatus = res =>
  Fetch.Response.ok(res) ?
    Result.Ok(res) :
    Result.Error("Request failed: " ++ Fetch.Response.statusText(res));

let query = query => {
  open Fetch;

  let body =
    [
      ("query", Js.Json.string(query##query)),
      ("variables", query##variables),
    ]
    |> Js.Dict.fromList
    |> Js.Json.object_
    |> Js.Json.stringify
    |> Fetch.BodyInit.make;

  let headers =
    [("Content-Type", "application/json")]
    |> Js.Dict.fromList
    |> Fetch.HeadersInit.makeWithDict;

  let response =
    Fetch.fetchWithInit(
      "http://localhost:4466",
      Fetch.RequestInit.make(~method_=Fetch.Post, ~body, ~headers, ()),
    );

  response
  ->FutureJs.fromPromise(Js.String.make)
  ->Future.map(Result.onOk(handleResponseStatus))
  ->Future.flatMapOk(res =>
      res->Response.json->FutureJs.fromPromise(Js.String.make)
    )
  ->Future.map(
      Result.onOk(res => {
        Js.log(res);
        switch (Js.Json.decodeObject(res)) {
        | Some(obj) =>
          switch (Js.Dict.get(obj, "data")) {
          | Some(x) => Ok(x |> query##parse)
          | None =>
            Error(
              "Graphql request failed, response is malformed"
              ++ Js.Json.stringify(res),
            )
          }
        | None =>
          Error(
            "Graphql request failed: response is not an object\n"
            ++ Js.Json.stringify(res),
          )
        };
      }),
    );
};
