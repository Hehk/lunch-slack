module Test = {
  type t;
  [@bs.send.pipe : t] external pass : unit = "";
};

[@bs.module "ava"] external test : (string, Test.t => unit) => unit = "";


