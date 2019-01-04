open Utils;

let getRestaurant = (businesses: list(YelpJson_bs.business)) => {
  let errorMsg = Result.Error("No restaurant found");
  switch (businesses) {
  | [] => errorMsg
  | _ =>
    businesses
    |> List.length
    |> Js.Math.random_int(0)
    |> Belt.List.get(businesses)
    |> (
      fun
      | None => errorMsg
      | Some(x) => Ok(x)
    )
  };
};

let createMessage = ({name}: YelpJson_bs.business): SlackJson_bs.message => {
  text: name,
  response_type: Some("in_channel"),
  attachments: [],
};

let handleRequest = queryString => {
  open Atdgen_codec_runtime;

  let command =
    queryString
    |> Querystring.parse
    |> Decode.decode(SlackJson_bs.read_command);

  Yelp.search(command.text)
  ->Future.mapOk(x => x.businesses)
  ->Future.map(getRestaurant |> Result.onOk)
  ->Future.mapOk(createMessage)
  ->Future.mapOk(Encode.encode(SlackJson_bs.write_message))
  ->Future.mapOk(x => Http.Json(x));
};

let handler = (request: Http.request) => {
  let text = request.text();

  text
  ->Future.flatMap(
      fun
      | Ok(x) => handleRequest(x)
      | Error(msg) => Future.value(Result.Error(msg)),
    );
};
