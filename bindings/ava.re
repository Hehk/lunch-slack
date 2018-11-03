type t;
[@bs.send.pipe : t] external pass : unit = "";

[@bs.module "ava"] external test : (string, t => unit) => unit = "";


