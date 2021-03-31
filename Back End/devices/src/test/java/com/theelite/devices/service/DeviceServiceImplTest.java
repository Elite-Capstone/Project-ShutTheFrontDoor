package com.theelite.devices.service;

import com.theelite.devices.model.Device;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import static org.junit.jupiter.api.Assertions.*;

//@RunWith(SpringRunner.class)
//@SpringBootTest
public class DeviceServiceImplTest {
    List<Device> devices = new ArrayList<>();

    @Test
    public void TestRetrievingAllIds() {
        List<String> ids = devices
                .stream()
                .map(d -> d.getDeviceId())
                .filter(id -> id != null)
                .collect(Collectors.toList());

        assertNotNull(ids);
    }

}