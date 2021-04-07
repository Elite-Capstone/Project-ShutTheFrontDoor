package com.theelite.portal.request

import okhttp3.ResponseBody
import retrofit2.Call
import retrofit2.http.DELETE
import retrofit2.http.GET
import retrofit2.http.Path

interface MediaService {

    @GET("file/list/{email}/{token}")
    fun getFileNames(@Path("email") email: String, @Path("token") token: String): Call<List<String>>

    @GET("file/download/{filename}")
    fun downloadFile(@Path("filename") filename: String): Call<ResponseBody>

    @DELETE("file/delete/{filename}")
    fun deleteFile(@Path("filename") filename: String): Call<Void>

}