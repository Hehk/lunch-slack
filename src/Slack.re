type command = Js.t({
  .
  token: string,
  team_id: string,
  team_domain: string,
  channel_id: string,
  channel_name: string,
  user_id: string,
  user_name: string,
  command: string,
  text: string,
  response_url: string,
  trigger_id: string,
});

external parseCommand : Js.Json.t => command = "%identity";

module Request = {
};

module Response = {
  [@bs.deriving abstract]
  type message = {
    text: string,
    [@bs.optional] response_type: string,
    [@bs.optional] attachments: array(message)
  };
  external messageToJson : message => Js.Json.t = "%identity";

  let sendMessage = (~res, message) => Micro.Response.sendJson(~res, message |> messageToJson);
};
