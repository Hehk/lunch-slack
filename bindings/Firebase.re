[@bs.deriving abstract]
type config = {
  apiKey: string,
  [@bs.optional] authDomain: string,
  databaseURL: string,
  [@bs.optional] storageBucket: string,
};

[@bs.module "firebase"] external initializeApp : config => unit = "";
