open Utils;
open Result;

/* TODO add smarter selection so the same does not come up every time */
let getRestaurant = (businesses: list(YelpJson_bs.business)) =>
  switch (businesses) {
  | [] => Error("No restaurant found")
  | [hd, ..._] => Ok(hd)
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
  ->Future.map(getRestaurant |> onOk)
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
      | Error(msg) => Future.value(Error(msg)),
    );
};
