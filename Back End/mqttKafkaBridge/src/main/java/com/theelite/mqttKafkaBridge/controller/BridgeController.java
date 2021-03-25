package com.theelite.mqttKafkaBridge.controller;

import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class BridgeController {
    @GetMapping("/bridge")
    public ResponseEntity<String> getHealth() {
        return new ResponseEntity<>("Seems to be working", HttpStatus.OK);
    }
}
