open Utils;

let handleResponseStatus = res =>
  Fetch.Response.ok(res) ?
    Result.Ok(res) :
    Result.Error("Request failed: " ++ Fetch.Response.statusText(res));

let sendQuery = query => {
  open Fetch;

  let body =
    Js.Dict.fromList([
      ("query", Js.Json.string(query##query)),
      ("variables", query##variables),
    ])
    |> Js.Json.object_
    |> Js.Json.stringify
    |> Fetch.BodyInit.make;
  let headers =
    Fetch.HeadersInit.makeWithArray([|
      ("content-type", "application/json"),
    |]);

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
  ->Future.mapOk(query##parse);
};
