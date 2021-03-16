package com.theelite.s3.service;

import com.amazonaws.services.s3.AmazonS3;
import com.amazonaws.services.s3.model.PutObjectRequest;
import com.amazonaws.services.s3.model.S3Object;
import com.amazonaws.services.s3.model.S3ObjectInputStream;
import com.amazonaws.util.IOUtils;
import com.theelite.s3.communication.DeviceService;
import com.theelite.s3.communication.MediaDirectoryService;
import com.theelite.s3.communication.UsersService;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.core.env.Environment;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Service;
import org.springframework.web.multipart.MultipartFile;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.List;
import java.util.Objects;

@Service
@Slf4j
public class StorageService {

    @Value("${application.bucket.name}")
    private String bucketName;

    private final AmazonS3 s3Client;
    private final MediaDirectoryService mediaDirectoryService;
    private final UsersService usersService;
    private final DeviceService deviceService;

    public StorageService(AmazonS3 s3Client, Environment environment) {
        this.s3Client = s3Client;
        this.mediaDirectoryService = buildRetrofitObjects(environment.getProperty("media.directory.url"), MediaDirectoryService.class);
        this.usersService = buildRetrofitObjects(environment.getProperty("user.url"), UsersService.class);
        this.deviceService = buildRetrofitObjects(environment.getProperty("device.url"), DeviceService.class);
    }


    public String uploadFile(MultipartFile file, String deviceId) {
        File fileObj = convertMultiPartFileToFile(file);
        String fileName = System.currentTimeMillis() + "_" + file.getOriginalFilename();
        s3Client.putObject(new PutObjectRequest(bucketName, fileName, fileObj));
        fileObj.delete();
        saveFileNameToUserAccount(fileName, deviceId);
        return "File uploaded : " + fileName;
    }


    public byte[] downloadFile(String fileName) {
        S3Object s3Object = s3Client.getObject(bucketName, fileName);
        S3ObjectInputStream inputStream = s3Object.getObjectContent();
        try {
            return IOUtils.toByteArray(inputStream);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }


    public String deleteFile(String fileName) {
        s3Client.deleteObject(bucketName, fileName);
        mediaDirectoryService.deleteFilename(fileName);
        return fileName + " removed ...";
    }


    private File convertMultiPartFileToFile(MultipartFile file) {
        File convertedFile = new File(Objects.requireNonNull(file.getOriginalFilename()));
        try (FileOutputStream fos = new FileOutputStream(convertedFile)) {
            fos.write(file.getBytes());
        } catch (IOException | NullPointerException e) {
            log.error("Error converting multipartFile to file", e);
        }
        return convertedFile;
    }

    public List<String> getFilenamesForUser(String email, String token) {
        List<String> filenames = null;
        try {
            Boolean userLegit = usersService.validateUser(email, token).execute().body();
            if (userLegit == null || !userLegit) return null;
            String accountId = usersService.getFamilyAccountFor(email).execute().body();
            if (accountId == null || accountId.isBlank()) return null;
            filenames = mediaDirectoryService.getFilesnamesForAccount(accountId).execute().body();
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
        return filenames;
    }

    public void familyAccountDeleted(String accountId) {
        try {
            List<String> fileNames = mediaDirectoryService.getFilesnamesForAccount(accountId).execute().body();
            mediaDirectoryService.deleteFilesnamesForAccount(accountId).execute();
            if (fileNames == null && fileNames.size() == 0) return;
            if (fileNames.size() > 10)
                fileNames.parallelStream().forEach(fileName -> s3Client.deleteObject(bucketName, fileName));
            else fileNames.forEach(fileName -> s3Client.deleteObject(bucketName, fileName));

        } catch (IOException | NullPointerException e) {
            System.out.println(e.getMessage());
        }
    }

    public boolean saveFileNameToUserAccount(String filename, String deviceId) {
        Boolean nameExists = false;
        String familyAccount = getFamilyName(deviceId);
        if (familyAccount == null || familyAccount.isEmpty() || familyAccount.isBlank()) return false;
        try {
            nameExists = mediaDirectoryService.checkFileNameExists(filename).execute().body();
            // Name already saved in db
            if (nameExists == null || nameExists) return false;
            nameExists = mediaDirectoryService.saveFilesnameForAccount(familyAccount, filename).execute().body();
        } catch (IOException | NullPointerException e) {
            System.out.println(e.getMessage());
        }
        return nameExists;
    }

    private String getFamilyName(String device) {
        String familyAccount = null;
        try {
            familyAccount = deviceService.getFamilyAccountForDeviceWithId(device).execute().body();
        } catch (IOException | NullPointerException e) {
            System.out.println(e.getMessage());
        }
        return familyAccount;
    }

    private <T> T buildRetrofitObjects(String url, Class<T> service) {
        if (url == null || url.isBlank() || url.isBlank()) {
            System.out.println("Url for " + service.getName() + " is null.");
            return null;
        }
        Retrofit retrofit = new Retrofit.Builder().baseUrl(url).addConverterFactory(GsonConverterFactory.create()).build();
        return retrofit.create(service);
    }

    public ResponseEntity<String> getHealth() {
        try {
            s3Client.getBucketPolicy(bucketName);
            if (usersService == null)
                return new ResponseEntity<>("userService is null", HttpStatus.INTERNAL_SERVER_ERROR);
            else if (deviceService == null)
                return new ResponseEntity<>("deviceService is null", HttpStatus.INTERNAL_SERVER_ERROR);
            else if (mediaDirectoryService == null)
                return new ResponseEntity<>("mediaDirectoryService is null", HttpStatus.INTERNAL_SERVER_ERROR);
        } catch (Exception e) {
            return new ResponseEntity<>("Error with s3 Connection\n" + e.getMessage(), HttpStatus.INTERNAL_SERVER_ERROR);
        }
        return new ResponseEntity<>("Everything seems to be fine", HttpStatus.OK);
    }
}