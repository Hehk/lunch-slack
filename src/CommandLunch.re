open Micro;

let getRestaurant = (~query) => {
  let query = Js.String.trim(query);
  if (query === "mexican") {
    "Taco Bell"
  } else {
    "Mixt"
  }
};

let handler = (~req, ~res) => {
  req.raw
  |> Request.text
  |> Js.Promise.then_(x => {
    let command = x |> Querystring.parse |> Slack.parseCommand;
    let restaurant = getRestaurant(~query=command##text);

    Slack.Response.message(~response_type="in_channel", ~text=restaurant, ())
    |> Slack.Response.sendMessage(~res)
    |> Js.Promise.resolve;
  })
  |> Js.Promise.catch(e => {
    Js.log(e)
    |> Js.Promise.resolve;
  })
  |> ignore;
};
