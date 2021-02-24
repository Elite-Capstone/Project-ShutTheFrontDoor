package com.theelite.email.controller;
import com.theelite.email.model.Email;
import com.theelite.email.model.RequestBody;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.mail.SimpleMailMessage;
import org.springframework.mail.javamail.JavaMailSender;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/email")
public class emailController {

    @Autowired
    private JavaMailSender javaMailSender;

    @GetMapping("/")
    public String getMicroServiceHealth() {
        return "Service is running!";
    }

    @PostMapping("/send")
    public boolean sendEmail(Email body) {

           SimpleMailMessage msg = new SimpleMailMessage();

           msg.setTo(body.getEmail());
           msg.setSubject(body.getSubject());
           msg.setText(body.getBody());

           javaMailSender.send(msg);

       return true;
    }
}




