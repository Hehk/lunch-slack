open Atdgen_codec_runtime;
open Utils;

let headers =
  Fetch.HeadersInit.make({
    "Content-type": "application/json",
    "Authorization": "Bearer " ++ Env.yelpApiKey,
  });

let decode = (decoder, json) =>
  switch (Decode.decode(decoder, json)) {
  | x => Some(x)
  | exception (Json_decode.DecodeError(msg)) =>
    Sentry.captureMessage(msg);
    Js.log("Decoding Error: \n" ++ msg);
    None;
  };

let search = query => {
  let endpoint = "https://api.yelp.com/v3/businesses/search";
  let queryString =
    Encode.encode(
      YelpJson_bs.write_searchParams,
      {
        term: query,
        location: Some("San Francisco, CA"),
        latitude: None,
        longitude: None,
        radius: None,
        categories: Some("Restaurant"),
        limit: 10,
        open_now: Some(true),
      },
    )
    |> Querystring.stringify;

  Js.Promise.(
    Fetch.fetchWithInit(
      endpoint ++ "?" ++ queryString,
      Fetch.RequestInit.make(~headers, ()),
    )
    |> then_(Fetch.Response.json)
    |> then_(resolve >> decode(YelpJson_bs.read_searchResponse))
  );
};
