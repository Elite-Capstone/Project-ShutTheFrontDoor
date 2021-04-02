package com.theelite.s3.controller;

import com.theelite.s3.service.StorageService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.io.ByteArrayResource;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

import java.util.List;

@RestController
@RequestMapping("/file")
public class StorageController {

    @Autowired
    private StorageService service;

    @GetMapping("/")
    public ResponseEntity<String> pong() {
        return service.getHealth();
    }

    @PostMapping("/upload/{id}")
    public ResponseEntity<String> uploadFile(@RequestParam(value = "file") MultipartFile file, @PathVariable String id) {
        return new ResponseEntity<>(service.uploadFile(file, id), HttpStatus.OK);
    }

    @GetMapping("/download/{fileName}")
    public ResponseEntity<ByteArrayResource> downloadFile(@PathVariable String fileName) {
        byte[] data = service.downloadFile(fileName);
        if (data != null) {
            ByteArrayResource resource = new ByteArrayResource(data);
            return ResponseEntity
                    .ok()
                    .contentLength(data.length)
                    .header("Content-type", "application/octet-stream")
                    .header("Content-disposition", "attachment; filename=\"" + fileName + "\"")
                    .body(resource);
        }
        return ResponseEntity
                .ok()
                .contentLength(0)
                .header("Content-type", "application/octet-stream")
                .header("Content-disposition", "No files found").body(null);
    }

    @DeleteMapping("/delete/{fileName}")
    public ResponseEntity<String> deleteFile(@PathVariable String fileName) {
        return new ResponseEntity<>(service.deleteFile(fileName), HttpStatus.OK);
    }

    @DeleteMapping("/familyAccountDeleted/{acc}")
    public void deleteAllFilesRelatedToAccount(@PathVariable String acc) {
        service.familyAccountDeleted(acc);
    }

    @GetMapping("/list/{email}/{token}")
    public List<String> getListForUser(@PathVariable("email") String email, @PathVariable("token") String token) {
        return service.getFilenamesForUser(email, token);
    }
}