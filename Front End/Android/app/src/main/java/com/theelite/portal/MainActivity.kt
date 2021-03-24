package com.theelite.portal

import android.os.Bundle
import androidx.activity.viewModels
import androidx.appcompat.app.AppCompatActivity
import androidx.navigation.findNavController
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.setupWithNavController
import com.google.android.material.bottomnavigation.BottomNavigationView
import com.theelite.portal.ui.home.HomeViewModel


class MainActivity : AppCompatActivity() {

    private val homeViewModel by viewModels<HomeViewModel>()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val navView: BottomNavigationView = findViewById(R.id.nav_view)

        val home = supportFragmentManager.findFragmentById(R.id.nav_host_fragment)
        println("intent argument is : ${intent.extras?.get("Home_Greeting")}")
        home?.arguments = intent.extras

        val navController = findNavController(R.id.nav_host_fragment)
        // Passing each menu ID as a set of Ids because each
        // menu should be considered as top level destinations.
        val appBarConfiguration = AppBarConfiguration(
            setOf(
                R.id.navigation_home,
                R.id.navigation_media,
                R.id.navigation_notifications,
                R.id.navigation_settings
            )
        )
//        setupActionBarWithNavController(navController, appBarConfiguration)
        navView.setupWithNavController(navController)
    }

    override fun onResume() {
        super.onResume()
        homeViewModel.setGreeting(intent.extras?.get("Home_Greeting").toString())
    }

    /*fun sendGet(view: View) {
       // val retrofit = RetroFit.get(getString(R.string.url))

        val gson = GsonBuilder()
            .setLenient()
            .create()

        val retrofit: Retrofit = Retrofit.Builder()
            .baseUrl("http://34.122.235.66/")
            //.addConverterFactory(BitmapFactory.create())
            .build()

        val mediaService = retrofit.create(mediaInt::class.java)

        val call = mediaService.getMovies()
        call.enqueue(object : Callback<ResponseBody> {
            override fun onResponse(call: Call<ResponseBody>, response: Response<ResponseBody>) {
                if (response.isSuccessful) {
                    println("File Downloaded!")
                    if (response.body() != null) {
                        // display the image data in a ImageView or save it
                        val bm = BitmapFactory.decodeStream(response.body()!!.byteStream())
                       findViewById<ImageView>(R.id.image1).setImageBitmap(bm)
                    }

                }
            }

            override fun onFailure(call: Call<ResponseBody>, t: Throwable) {
                println("Failure" + t.message)
            }
        })

    }*/


}