FROM node:latest as build
# bs-platform will reference this global installation when installed
# to the application
RUN npm install -g bs-platform --unsafe-perm

WORKDIR /app/
COPY package.json .
COPY package-lock.json .
RUN npm install

COPY . .
RUN npm run build


FROM gcr.io/distroless/nodejs
COPY --from=build /app/dist/nom.js /app/nom.js
WORKDIR /app
EXPOSE 3000
CMD ["nom.js"]
