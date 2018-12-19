open Atdgen_codec_runtime;

let search = query => {
  let url = "https://api.yelp.com/v3/businesses/search";
  let queryString = Encode.encode(YelpJson_bs.write_searchParams, {
    term: query,
    location: Some("San Francisco, CA"),
    latitude: None,
    longitude: None,
    radius: None,
    categories: Some("Restaurant"),
    limit: 10,
    open_now: Some(true)
  }) |> Querystring.stringify;

  Js.log(url ++ queryString);
};
