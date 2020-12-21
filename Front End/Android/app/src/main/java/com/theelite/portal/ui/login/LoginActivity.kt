package com.theelite.portal.ui.login

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import com.theelite.portal.MainActivity
import com.theelite.portal.R
import com.theelite.portal.ui.register.RegisterActivity

class LoginActivity : AppCompatActivity() {
    lateinit var toRegisterButton: Button
    lateinit var loginButton: Button

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_login)

        toRegisterButton = findViewById(R.id.loginToRegisterButton)
        loginButton = findViewById(R.id.loginButton)

        toRegisterButton.setOnClickListener {
            val registerActivityIntent = Intent(this, RegisterActivity::class.java)
            startActivity(registerActivityIntent)
        }

        loginButton.setOnClickListener {
            val mainActivityIntent = Intent(this, MainActivity::class.java)
            startActivity(mainActivityIntent)
            this.finish()
        }

    }
}