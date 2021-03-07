FROM golang:alpine

#ENV SERVER_PORT=8080
#    MYSQLDB=test


# Set the Current Working Directory inside the container
WORKDIR /app

# Copy go mod and sum files
COPY go.mod ./
COPY go.sum ./
RUN ls -al
# Download all the dependencies
RUN go mod download

# Copy binary from build to main folder
COPY . .
RUN go build -o main .

# This container exposes port 8080 to the outside world
EXPOSE 8080

# Run the executable
#CMD ["go-media-directory"]
CMD ["./main"]