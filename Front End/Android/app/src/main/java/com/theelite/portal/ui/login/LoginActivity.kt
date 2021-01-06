package com.theelite.portal.ui.login

import android.content.Intent
import android.os.Build
import android.os.Bundle
import android.widget.Button
import android.widget.Toast
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AppCompatActivity
import com.android.volley.Request
import com.android.volley.Response
import com.android.volley.toolbox.Volley
import com.google.android.material.textfield.TextInputEditText
import com.theelite.portal.MainActivity
import com.theelite.portal.R
import com.theelite.portal.request.BooleanRequest
import com.theelite.portal.ui.register.RegisterActivity
import org.json.JSONObject
import kotlin.random.Random

class LoginActivity : AppCompatActivity() {
    private lateinit var toRegisterButton: Button
    private lateinit var loginButton: Button
    private lateinit var loginEmailTextField: TextInputEditText
    private lateinit var loginPasswordTextField: TextInputEditText


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_login)
        setUpLoginView()
    }

    private fun setUpLoginView() {

        toRegisterButton = findViewById(R.id.loginToRegisterButton)
        loginButton = findViewById(R.id.loginButton)
        loginEmailTextField = findViewById(R.id.loginEmailTextField)
        loginPasswordTextField = findViewById(R.id.loginPasswordTextField)

        toRegisterButton.setOnClickListener {
            val registerActivityIntent = Intent(this, RegisterActivity::class.java)
            startActivity(registerActivityIntent)
        }

        loginButton.setOnClickListener {
            login()
        }
    }


    private fun login() {

//        if (::loginEmailTextField.isInitialized) print("They null"); return
//        println("Email is ${loginEmailTextField.text}")
//        println("Password is ${loginPasswordTextField.text}")
        val queue = Volley.newRequestQueue(this)
        //TODO refactor all urls in one single place
        val url = R.string.url
        val params = JSONObject()
        params.put("email", loginEmailTextField.text.toString())
        params.put("password", loginPasswordTextField.text.toString())
        params.put("username", loginEmailTextField.text.toString())


//        println("params in strings is ${params.toString()}")
//        val jSonObjectRequest = BooleanRequest(
//            Request.Method.POST,
//            url, params.toString(),
//            Response.Listener { authenticated -> if (authenticated) moveToHomePage() else displayError() },
//            Response.ErrorListener { error -> println("error happened during get ${error.message}") }
//        )
//        queue.add(jSonObjectRequest)
        moveToHomePage()
    }

    private fun moveToHomePage() {
        val mainActivityIntent = Intent(this, MainActivity::class.java)
        var greeting: String
        when(Random(System.currentTimeMillis()).nextInt(3)) {
            2    -> greeting = R.string.login_message2.toString()
            3    -> greeting = R.string.login_message3.toString()
            else -> greeting = R.string.login_message1.toString()
        }
        println("The greeting is $greeting")
        mainActivityIntent.putExtra("Login", greeting)
        println("Login intent argument is : ${mainActivityIntent.extras?.get("Login")}")
        startActivity(mainActivityIntent)
        this.finish()
    }

    private fun displayError() {
        Toast.makeText(this, "Failed to Authenticate", Toast.LENGTH_SHORT).show()
    }
}