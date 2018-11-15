import micro from "micro";
import { make } from "../lib/es6_global/src/Nom.bs.js";

const app = micro(make);
app.listen(3000, () => console.log("Listening on port 3000"));

export default app;
