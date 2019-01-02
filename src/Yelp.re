open Atdgen_codec_runtime;
open Utils;
open Belt.Result;

let headers =
  Fetch.HeadersInit.make({
    "Content-type": "application/json",
    "Authorization": "Bearer " ++ Env.yelpApiKey,
  });

let decode = (decoder, json) =>
  switch (Decode.decode(decoder, json)) {
  | x => Ok(x)
  | exception (Json_decode.DecodeError(msg)) => Error("Decoding Error" ++ msg)
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
  let response =
    Fetch.fetchWithInit(
      endpoint ++ "?" ++ queryString,
      Fetch.RequestInit.make(~headers, ()),
    );

  Js.Promise.then_(Fetch.Response.json, response)
  ->FutureJs.fromPromise(Js.String.make)
  ->Future.map(YelpJson_bs.read_searchResponse |> decode |> Result.onOk);
};
