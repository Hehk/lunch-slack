let compose = (f, g, x) => f(g(x));

let (>>) = compose;

/* ignore but for promises :D */
let ignoreResolve = (_) => {
  Js.Promise.resolve(());
};
