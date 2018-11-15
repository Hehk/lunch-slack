// This file exists to add micro into the webpack build
const micro = require("micro");
const app = require("./app");

const server = micro(app);
server.listen(3000, () => console.log("Listening on port 3000"));

module.exports = server;
