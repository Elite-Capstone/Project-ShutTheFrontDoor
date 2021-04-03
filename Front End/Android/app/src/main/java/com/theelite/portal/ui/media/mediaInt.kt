package com.theelite.portal.ui.media

import okhttp3.ResponseBody
import retrofit2.Call
import retrofit2.Callback
import retrofit2.http.GET
import java.io.File

interface mediaInt {

    @GET("file/download/1614125014709_abstractVenom.jpg")
    fun getMovies() : Call < ResponseBody >
}