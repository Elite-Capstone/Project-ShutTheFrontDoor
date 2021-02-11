package com.theelite.notifications.configuration;

import com.theelite.notifications.model.Notification;
import org.apache.kafka.clients.admin.AdminClient;
import org.apache.kafka.clients.admin.KafkaAdminClient;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.autoconfigure.kafka.KafkaProperties;
import org.springframework.context.EnvironmentAware;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.env.Environment;
import org.springframework.kafka.core.ConsumerFactory;
import org.springframework.kafka.core.DefaultKafkaConsumerFactory;
import org.springframework.kafka.core.KafkaAdmin;

import java.util.Properties;
import java.util.UUID;

@Configuration
public class NotificationConfigurations {
    @Value("${spring.kafka.bootstrap-servers}")
    public String bootstrapServer;

    @Bean
    public ConsumerFactory<UUID, Notification> consumerFactory(KafkaProperties kafkaProperties) {
        return new DefaultKafkaConsumerFactory<>(kafkaProperties.buildConsumerProperties());
    }

    @Bean
    public KafkaAdmin kafkaAdmin(KafkaProperties kafkaProperties) {
        return new KafkaAdmin(kafkaProperties.buildAdminProperties());
    }

    @Bean
    public AdminClient kafkaAdminClient(KafkaProperties kafkaProperties) {

//        Properties properties = new Properties();
//        properties.put("bootstrap.servers", kafkaProperties.);
//        properties.put("connections.max.idle.ms", 10000);
//        properties.put("request.timeout.ms", 5000);
//        try (
//                AdminClient client =
        return KafkaAdminClient.create(kafkaProperties.buildAdminProperties());

//        ) {
//            CreateTopicsResult result = client.createTopics(Arrays.asList(
//                    new NewTopic("storage-topic", 1, (short) 1),
//                    new NewTopic("global-id-topic", 1, (short) 1),
//                    new NewTopic("snapshot-topic", 1, (short) 1)
//            ));
//            try {
//                result.all().get();
//            } catch (InterruptedException | ExecutionException e) {
//                throw new IllegalStateException(e);
//            }
//        }
    }


    public static Properties getConsumerProps(String clientId, String consumerGroup, String bootstrapServer) {
        // TODO deserializer for java object
        Properties props = new Properties();
        props.setProperty("bootstrap.servers", bootstrapServer);
        props.setProperty("group.id", consumerGroup);
        props.setProperty("client.it", clientId);
        props.setProperty("enable.auto.commit", "true");
        props.setProperty("key.deserializer", "org.apache.kafka.common.serialization.StringDeserializer");
        props.setProperty("value.deserializer", "org.apache.kafka.common.serialization.StringDeserializer");

        return props;
    }

    public static Properties getProducerProps(String bootstrapServer){
        //TODO serializer for java object
        Properties props = new Properties();
        props.setProperty("bootstrap.servers", bootstrapServer);
        props.put("key.serializer", "org.apache.kafka.common.serialization.StringSerializer");
        props.put("value.serializer", "org.apache.kafka.common.serialization.StringSerializer");

        return props;
    }
}
