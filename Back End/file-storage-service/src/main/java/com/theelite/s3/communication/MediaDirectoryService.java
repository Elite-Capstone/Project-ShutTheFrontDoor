package com.theelite.s3.communication;

import retrofit2.Call;
import retrofit2.http.GET;
import retrofit2.http.Path;

import java.util.List;

public interface MediaDirectoryService {

    @GET("getMedias/{accountId}")
    Call<List<String>> getFilesnamesForAccount(@Path("accountId") String accountId);

    @GET("deleteMedia/{accountId}")
    Call<Boolean> deleteFilename(@Path("accountId") String accountId);

    @GET("deleteMedias/{accountId}")
    Call<Boolean> deleteFilesnamesForAccount(@Path("accountId") String accountId);

    @GET("insertMedia/{accountId}/{file}")
    Call<Boolean> saveFilesnameForAccount(@Path("accountId") String accountId, @Path("file") String file);

    @GET("nameExists/{accountId}")
    Call<Boolean> checkFileNameExists(@Path("accountId") String accountId);


}
