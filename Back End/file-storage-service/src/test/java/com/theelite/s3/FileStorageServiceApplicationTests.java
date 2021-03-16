package com.theelite.s3;

import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.test.context.SpringBootTest;


@SpringBootTest
//@TestPropertySource("/resources/application.properties")
class FileStorageServiceApplicationTests {


    @Value("${media.directory.url}")
    private String mediaDirectoryUrl;
    @Value("${user.url}")
    private String usersUrl;

    @Test
    public void whenFilePropertyProvided_thenProperlyInjected() {
        assert mediaDirectoryUrl.equals("http://test/");
        assert usersUrl.equals("http://test/");
    }

    @Test
    void contextLoads() {
    }

}
