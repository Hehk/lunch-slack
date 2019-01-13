let compose = (f, g, x) => f(g(x));

let (>>) = compose;

/* ignore but for promises :D */
let ignoreResolve = _ => Js.Promise.resolve();

let some = x => Some(x);

module Option = {
  let fmap = (f: 'a => 'b) =>
    fun
    | None => None
    | Some(x) => x->f->Some;

  let flatten =
    fun
    | None => None
    | Some(None) => None
    | Some(Some(x)) => Some(x);
};

module Result = {
  include Belt.Result;

  let onOk = f =>
    fun
    | Error(x) => Error(x)
    | Ok(x) => f(x);

  let onError = f =>
    fun
    | Error(x) => f(x)
    | Ok(x) => Ok(x);
};

