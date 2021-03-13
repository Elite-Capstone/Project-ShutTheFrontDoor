FROM maven:3.6.0-jdk-11-slim AS build
WORKDIR /app
COPY src src
COPY pom.xml .
RUN ls
#RUN mvn -f pom.xml clean package
#RUN mvn clean install
RUN mvn clean package

#
# Package stage
#
FROM openjdk:11-jre-slim
#COPY --from=build /app/build/sizzle-0.0.1.jar /usr/local/lib/app.jar
COPY --from=build /app/target/email-0.0.1-SNAPSHOT.jar /usr/local/lib/app.jar
EXPOSE 8080
ENTRYPOINT ["java","-jar","/usr/local/lib/app.jar"]
