open Micro;

let make = make(~handler=(~req, ~res) => {
  switch (req.path) {
  | ["slack", ..._] => Response.sendString(~res, "Hello World")
  | _ => ();
  }
});

