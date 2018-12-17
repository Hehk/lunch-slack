open Micro;
open Utils;

let getRestaurant = (~query) => {
  let query = Js.String.trim(query);
  if (query === "mexican") {
    "Taco Bell"
  } else {
    "Mixt"
  }
};

let handleRequest = (queryString) => {
  open Atdgen_codec_runtime;

  let command = queryString
  |> Querystring.parse
  |> Decode.decode(SlackJson_bs.read_command);

  let restaurant =
      getRestaurant(~query=command.text);
  let message = Encode.encode(SlackJson_bs.write_message, {
    text: restaurant,
    response_type: Some("in_channel"),
    attachments: [],
  });

  Response.sendJson(~message);
};

let handler = (~req, ~res) => {
  req.raw
  |> Request.text
  |> Js.Promise.then_(ignoreResolve >> handleRequest(~res))
  |> Js.Promise.catch(ignoreResolve >> Js.log)
  |> ignore;
};
