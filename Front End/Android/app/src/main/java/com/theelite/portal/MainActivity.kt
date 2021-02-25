package com.theelite.portal

import android.os.Bundle
import android.os.StrictMode
import android.util.Log
import android.view.View
import androidx.activity.viewModels
import com.google.android.material.bottomnavigation.BottomNavigationView
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentManager.findFragment
import androidx.fragment.app.findFragment
import androidx.navigation.findNavController
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.setupActionBarWithNavController
import androidx.navigation.ui.setupWithNavController
import com.theelite.portal.ui.home.HomeFragment
import com.theelite.portal.ui.home.HomeViewModel
import kotlinx.android.synthetic.main.activity_main.*
import java.io.BufferedReader
import java.io.InputStreamReader
import java.net.URL

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

    fun sendGet(view: View) {
        try {
            //disable the strict mode otherwise perform this operation on netWork Thread
            val policy = StrictMode.ThreadPolicy.Builder().permitAll().build()
            StrictMode.setThreadPolicy(policy)

            // GET Request
            val request = "http://34.122.235.66/file/download/1614216243554_venom.jpg"
            val url = URL(request)
            val conn = url.openConnection()
            conn.doOutput = true
            // Get the response
            val rd = BufferedReader(InputStreamReader(conn.getInputStream()))
            var line: String
            var sResult = ""
            while (rd.readLine().also { line = it } != null) {
                // Process line...
                sResult = "$sResult$line "
            }
            rd.close()
            Log.e("RESULT", sResult)
        } catch (e: Exception) {
            println("Error $e")
        }
    }


}