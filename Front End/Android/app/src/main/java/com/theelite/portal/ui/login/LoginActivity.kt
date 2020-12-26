package com.theelite.portal.ui.login

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import com.android.volley.Request
import com.android.volley.Response
import com.android.volley.toolbox.JsonObjectRequest
import com.android.volley.toolbox.Volley
import com.google.android.material.textfield.TextInputEditText
import com.google.android.material.textfield.TextInputLayout
import com.theelite.portal.MainActivity
import com.theelite.portal.R
import com.theelite.portal.request.BooleanRequest
import com.theelite.portal.ui.register.RegisterActivity
import org.json.JSONObject

class LoginActivity : AppCompatActivity() {
    lateinit var toRegisterButton: Button
    lateinit var loginButton: Button
    lateinit var loginEmailTextField: TextInputEditText
    lateinit var loginPasswordTextField: TextInputEditText


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
//        val mainActivityIntent = Intent(this, MainActivity::class.java)
//        startActivity(mainActivityIntent)
//        this.finish()
//        if (::loginEmailTextField.isInitialized) print("They null"); return
        println("Email is ${loginEmailTextField.text}")
        println("Password is ${loginPasswordTextField.text}")
        val queue = Volley.newRequestQueue(this)
        val url = "http://3.131.85.201/api/profile/authenticate"
        val params = JSONObject()
        params.put("email", loginEmailTextField.text.toString())
        params.put("password", loginPasswordTextField.text.toString())
        params.put("username", loginEmailTextField.text.toString())


        println("params in strings is ${params.toString()}")
        val jSonObjectRequest = BooleanRequest(
            Request.Method.POST,
            url, params.toString(),
            Response.Listener<Boolean> { res -> println("We got ${res}") },
            Response.ErrorListener { error -> println("error happend during get ${error.message}") }
        )
        queue.add(jSonObjectRequest)

    }
}