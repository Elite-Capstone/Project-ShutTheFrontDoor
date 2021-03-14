package com.theelite.s3.service;

import com.amazonaws.services.s3.AmazonS3;
import com.amazonaws.services.s3.model.PutObjectRequest;
import com.amazonaws.services.s3.model.S3Object;
import com.amazonaws.services.s3.model.S3ObjectInputStream;
import com.amazonaws.util.IOUtils;
import com.theelite.s3.communication.MediaDirectoryService;
import com.theelite.s3.communication.UsersService;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.web.multipart.MultipartFile;
import retrofit2.Retrofit;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.List;

@Service
@Slf4j
public class StorageService {

    @Value("${application.bucket.name}")
    private String bucketName;

    @Value("${media-directory.url}")
    private String mediaDirectoryUrl;
    @Value("${users.url}")
    private String usersUrl;


    private AmazonS3 s3Client;
    private MediaDirectoryService mediaDirectoryService;
    private UsersService usersService;


    public StorageService(AmazonS3 s3Client) {
        this.s3Client = s3Client;
        mediaDirectoryService = buildRetrofitObjects(mediaDirectoryUrl, MediaDirectoryService.class);
        usersService = buildRetrofitObjects(usersUrl, UsersService.class);
    }


    public String uploadFile(MultipartFile file) {
        File fileObj = convertMultiPartFileToFile(file);
        String fileName = System.currentTimeMillis() + "_" + file.getOriginalFilename();
        s3Client.putObject(new PutObjectRequest(bucketName, fileName, fileObj));
        fileObj.delete();
        return "File uploaded : " + fileName;
    }


    public byte[] downloadFile(String fileName) {
        S3Object s3Object = s3Client.getObject(bucketName, fileName);
        S3ObjectInputStream inputStream = s3Object.getObjectContent();
        try {
            byte[] content = IOUtils.toByteArray(inputStream);
            return content;
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }


    public String deleteFile(String fileName) {
        s3Client.deleteObject(bucketName, fileName);
        mediaDirectoryService.deleteFilesnamesForAccount(fileName);
        return fileName + " removed ...";
    }


    private File convertMultiPartFileToFile(MultipartFile file) {
        File convertedFile = new File(file.getOriginalFilename());
        try (FileOutputStream fos = new FileOutputStream(convertedFile)) {
            fos.write(file.getBytes());
        } catch (IOException e) {
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
            mediaDirectoryService.deleteFilesnamesForAccount(accountId).execute();
        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
    }

    public boolean saveFileNameToUserAccount(String filename, String familyAccount) {
        Boolean nameExists = false;
        try {
            nameExists = mediaDirectoryService.checkFileNameExists(filename).execute().body();
            // Name already saved in db
            if (nameExists == null || nameExists) return false;
            nameExists = mediaDirectoryService.saveFilesnameForAccount(familyAccount, filename).execute().body();
        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
        return nameExists;
    }

    private <T> T buildRetrofitObjects(String url, Class<T> service) {
        if (url == null || url.isBlank() || url.isBlank()) return null;
        Retrofit retrofit = new Retrofit.Builder().baseUrl(url).build();
        return retrofit.create(service);
    }
}