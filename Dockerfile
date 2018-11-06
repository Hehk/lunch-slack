FROM node:latest
# bs-platform will reference this global installation when installed
# to the application
RUN npm install -g bs-platform --unsafe-perm

WORKDIR /app/
COPY package.json .
COPY package-lock.json .
RUN npm install

COPY . .
RUN npm run build

EXPOSE 3000
CMD ["npm", "start"]
