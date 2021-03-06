package com.theelite.mqttKafkaBridge;

import com.google.gson.Gson;
import com.theelite.mqttKafkaBridge.model.Notification;

import org.apache.kafka.clients.admin.AdminClient;
import org.apache.kafka.clients.admin.KafkaAdminClient;
import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.Producer;
import org.apache.kafka.clients.producer.ProducerRecord;
import org.apache.log4j.ConsoleAppender;
import org.apache.log4j.Level;
import org.apache.log4j.Logger;
import org.apache.log4j.PatternLayout;
import org.eclipse.paho.client.mqttv3.*;
import org.springframework.core.env.Environment;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.scheduling.annotation.Async;
import org.springframework.scheduling.annotation.EnableAsync;
import org.springframework.stereotype.Component;

import java.util.*;
import java.util.stream.Collectors;

@EnableAsync
@Component
public class Bridge implements MqttCallback {
    public final int fiveMinutes = 300000;
    private Logger logger;
    private final String mqttServerUri;
    private final String brokerUri;
    public static MqttAsyncClient mqtt;
    private static AdminClient kafkaAdmin;
    private final String NOTIFICATION = "notification/";
    public static Producer<String, String> kafkaProducer;


    public Bridge(Environment environment) {
        this.brokerUri = environment.getProperty("spring.kafka.producer.bootstrap-servers");
        this.mqttServerUri = environment.getProperty("mqtt.servers");
        setUpLogger();
        connect();
        Thread periodicThread = new Thread(periodicallyGetKafkaTopics);
        periodicThread.start();
    }

    private void setUpLogger() {
        this.logger = Logger.getLogger(this.getClass().getName());
        ConsoleAppender consoleAppender = new ConsoleAppender();
        consoleAppender.setTarget(ConsoleAppender.SYSTEM_OUT);
        consoleAppender.activateOptions();
        consoleAppender.setLayout(new PatternLayout(PatternLayout.TTCC_CONVERSION_PATTERN));
        this.logger.addAppender(consoleAppender);
    }

    Runnable periodicallyGetKafkaTopics = new Runnable() {

        @Override
        public void run() {
            logger.info("Started Thread to periodically check and subscribe to kafka topics");
            while (true) {
                try {
                    String[] topics = kafkaAdmin
                            .listTopics()
                            .names()
                            .get()
                            .stream().map(t -> NOTIFICATION + t)
                            .collect(Collectors.toList())
                            .toArray(String[]::new);

                    logger.log(Level.INFO, "Got topics");
                    if (topics != null && topics.length > 0) {
                        logger.log(Level.INFO, "Topics not null and more than one");
                        subcribeToTopics(topics);
                    }
                    logger.trace("Thread to check new Topics going to sleep");
                    Thread.sleep(fiveMinutes);
                } catch (Exception e) {
                    logger.error(e.getMessage());
                }
            }
        }
    };


    public void connect() {
        try {
            mqtt = new MqttAsyncClient(mqttServerUri, "MqttKafkaBridge");
            mqtt.setCallback(this);
            MqttConnectOptions connOpts = new MqttConnectOptions();
            connOpts.setCleanSession(true);
            IMqttToken token = mqtt.connect(connOpts);
            Properties props = new Properties();
            props.put("bootstrap.servers", brokerUri);
            kafkaAdmin = KafkaAdminClient.create(props);
            props.put("key.serializer", "org.apache.kafka.common.serialization.StringSerializer");
            props.put("value.serializer", "org.apache.kafka.common.serialization.StringSerializer");
            kafkaProducer = new KafkaProducer<>(props);
            token.waitForCompletion();
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    private void reconnect() throws MqttException {
        IMqttToken token = mqtt.connect();
        token.waitForCompletion();
    }


    @Async
    @Override
    public void connectionLost(Throwable cause) {
        logger.warn("Lost connection to MQTT server", cause);
        while (true) {
            try {
                logger.info("Attempting to reconnect to MQTT server");
                reconnect();
                logger.info("Reconnected to MQTT server, resuming");
                return;
            } catch (MqttException e) {
                logger.warn("Reconnect failed, retrying in 10 seconds", e);
            }
            try {
                Thread.sleep(10000);
            } catch (InterruptedException e) {
                logger.error(e.getMessage());
            }
        }
    }

    @Override
    public void messageArrived(String topic, MqttMessage message) throws Exception {
        byte[] payload = message.getPayload();
        logger.log(Level.INFO, "Message Arrived " + topic);
        if (topic.contains(NOTIFICATION)) {
            topic = getDeviceIdFromTopic(topic);
            Notification notification = new Notification(new String(payload), topic);
            kafkaProducer.send((new ProducerRecord<>(topic, new Gson().toJson(notification))));
        }
    }


    private String getDeviceIdFromTopic(String topic) {
        return topic.split("/")[1];
    }

    private void subcribeToTopics(String[] topics) {
        int[] qos = new int[topics.length];
        try {
            IMqttToken token = mqtt.subscribe(topics, qos);
            token.waitForCompletion();
        } catch (MqttException e) {
            logger.error(e.getMessage());
        }
    }

    @Override
    public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {
        try {
            logger.info("DELIVERY COMPLETE " + iMqttDeliveryToken.getMessage().toString());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static ResponseEntity<String> testForHealth() {
        if (mqtt == null || kafkaProducer == null || kafkaAdmin == null)
            return new ResponseEntity<>("Kafka or Mqtt is null", HttpStatus.INTERNAL_SERVER_ERROR);
        try {
            mqtt.checkPing(null, null);
            kafkaProducer.metrics();
            kafkaAdmin.describeCluster();
        } catch (MqttException e) {
            return new ResponseEntity<>("Mqtt Server Error", HttpStatus.INTERNAL_SERVER_ERROR);
        } catch (Exception e) {
            return new ResponseEntity<>("Kafka Server Error", HttpStatus.INTERNAL_SERVER_ERROR);
        }
        return new ResponseEntity<>("Everything seems fine!\nMqtt Connection: " + mqtt.isConnected(), HttpStatus.OK);
    }
}
