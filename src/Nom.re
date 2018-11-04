open Micro;

let make = make(~handler=(req) => {
  Request.text(req)
  |> Js.Promise.then_(_value => {
    Js.Promise.resolve(String("Hello World"));
  });
});

