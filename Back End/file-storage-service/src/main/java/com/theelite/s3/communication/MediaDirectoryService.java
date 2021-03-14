package com.theelite.s3.communication;

import retrofit2.Call;
import retrofit2.http.GET;
import retrofit2.http.Path;

import java.util.List;

public interface MediaDirectoryService {

    @GET("/getMedias/{accountId}")
    Call<List<String>> getFilesnamesForAccount(@Path("accountId") String accountId);

    @GET("/deleteMedia/{accountId}")
    Call<List<String>> deleteFilename(@Path("accountId") String accountId);

    @GET("/deleteMedias/{accountId}")
    Call<List<String>> deleteFilesnamesForAccount(@Path("accountId") String accountId);

    @GET("/insertMedia/{accountId}")
    Call<List<String>> saveFilesnameForAccount(@Path("accountId") String accountId);

    @GET("/nameExists/{accountId}")
    Call<List<String>> checkFileNameExists(@Path("accountId") String accountId);


}
