package com.theelite.portal.ui.register

import android.os.Bundle
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import com.theelite.portal.R

class RegisterActivity : AppCompatActivity() {
    lateinit var registerButton: Button
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_register)
        registerButton = findViewById(R.id.registerButton)
        registerButton.setOnClickListener {
            println("Clicked on Button!")
        }
    }
}