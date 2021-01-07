package com.theelite.portal.ui.register

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import com.theelite.portal.MainActivity
import com.theelite.portal.R

class RegisterActivity : AppCompatActivity() {
    lateinit var registerButton: Button
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_register)
        registerButton = findViewById(R.id.registerButton)
        registerButton.setOnClickListener {
            //TODO Implement
//            println("Clicked on Button!")
            val mainActivity = Intent(this, MainActivity::class.java)
            mainActivity.putExtra("Home_Greeting", getString(R.string.registration_message))
            startActivity(mainActivity)
            this.finish()
        }
    }
}