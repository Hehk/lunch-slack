open Utils;

module SaveUserMutation = [%graphql
  {|
  mutation createNewUser($name: String!, $slackId: String!) {
    createUser(data: {
    name: $name,
    slackId: $slackId,
    orders: []
    }) {
      id
    }
  }
|}
];

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

let createMessage =
    ({name, location, url}: YelpJson_bs.business): SlackJson_bs.message => {
  let address =
    location.display_address
    |> (
      fun
      | [] => "None found"
      | [hd, ...tl] => Belt.List.reduce(tl, hd, (acc, s) => acc ++ ", " ++ s)
    );

  {
    text: "Restaurant",
    response_type: Some("in_channel"),
    attachments: [
      {title: Some(name), title_link: url, text: "Address: " ++ address},
    ],
  };
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
