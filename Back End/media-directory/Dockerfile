FROM golang:alpine as BUILD

LABEL maintainer="superDeano"

# Set the Current Working Directory inside the container
WORKDIR /app

# Copy go mod and sum files
COPY go.mod go.sum ./

# Download all the dependencies
RUN go mod download

# Copy binary from build to main folder
COPY . .
RUN go build -o main .

FROM alpine:latest

WORKDIR app/

COPY --from=BUILD /app/main .
#COPY /resources ./resources
RUN chmod +x main
# This container exposes port 8080 to the outside world
EXPOSE 8080

# Run the executable
#CMD ["go-media-directory"]
CMD ["./main"]