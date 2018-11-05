open Micro;

let make = make(~handler=(req) => {
  Js.Promise.make((~resolve, ~reject as _) => {
    resolve(. String("Hello World"))
  });
});

