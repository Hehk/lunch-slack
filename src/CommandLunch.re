open Utils;

module SaveCommandMutation = [%graphql
  {|
  mutation createCommand(
    $teamId: String!,
    $teamDomain: String!,
    $channelId: String!,
    $channelName: String!,
    $command: String!,
    $text: String!,
    $userId: ID!,
  ) {
    createCommand(data:{
      teamId: $teamId,
      teamDomain: $teamDomain,
      channelId: $channelId,
      channelName: $channelName,
      command: $command,
      text: $text,
      user: {
        connect: {
          id: $userId
        }
      }
    }) {
      id
    }
  }
|}
];

module SaveUser = [%graphql
  {|
  mutation upsertUser($slackId: ID!, $name: String!) {
    upsertUser(
      where: {
        slackId: $slackId
      },
      create:{
        name: $name,
        slackId: $slackId
      },
      update:{
        name:$name
      }
    ) {
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

let saveUser = (command: SlackJson_bs.command) => {
  let response =
    SaveUser.make(~slackId=command.user_id, ~name=command.user_name, ())
    ->Graphql.query;

  response->Future.mapOk(x => x##upsertUser##id);
};

let saveCommand = (command: SlackJson_bs.command, userId) =>
  SaveCommandMutation.make(
    ~teamId=command.team_id,
    ~teamDomain=command.team_domain,
    ~channelId=command.channel_id,
    ~channelName=command.channel_name,
    ~command=command.command,
    ~text=command.text,
    ~userId,
    (),
  )
  ->Graphql.query;

let handleRequest = queryString => {
  open Atdgen_codec_runtime;

  let command =
    queryString
    |> Querystring.parse
    |> Decode.decode(SlackJson_bs.read_command);

  command
  ->saveUser
  ->Future.flatMapOk(saveCommand(command))
  ->ignore;

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
