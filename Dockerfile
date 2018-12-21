FROM ocaml/opam2 as atd
RUN sudo apt-get install -y m4
RUN opam pin add atd --dev-repo
RUN opam pin add atdgen --dev-repo

WORKDIR /app
COPY . .
RUN sudo chmod -R 777 /app
RUN scripts/gen-atd


FROM node:latest as build
# bs-platform will reference this global installation when installed
# to the application
RUN npm install -g bs-platform --unsafe-perm

WORKDIR /app
COPY package.json .
COPY package-lock.json .
RUN npm install

COPY . .
COPY --from=atd /app/json /app/json
RUN ls json
RUN npm run build


FROM gcr.io/distroless/nodejs
COPY --from=build /app/dist/slack.js /app/slack.js
WORKDIR /app
EXPOSE 3000
CMD ["slack.js"]
