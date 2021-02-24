package com.theelite.portal.ui.login

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.google.android.material.textfield.TextInputEditText
import com.theelite.portal.MainActivity
import com.theelite.portal.Objects.User
import com.theelite.portal.R
import com.theelite.portal.request.RetroFit
import com.theelite.portal.request.UserService
import com.theelite.portal.ui.register.RegisterActivity
import retrofit2.Call
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

        val retrofit = RetroFit.get(getString(R.string.url))
        val userService = retrofit.create(UserService::class.java)
        val call = userService.authenthicateUser(getUserInfo())

        call.enqueue(object: retrofit2.Callback<Boolean>{
            override fun onResponse(call: Call<Boolean>, response: retrofit2.Response<Boolean>) {
                if (response.isSuccessful && response.body()!!) moveToHomePage() else displayError()
            }

            override fun onFailure(call: Call<Boolean>, t: Throwable) {
                Toast.makeText(this@LoginActivity, "${t.message}", Toast.LENGTH_LONG).show()
            }

        })
    }

    private fun moveToHomePage() {
        val mainActivityIntent = Intent(this, MainActivity::class.java)
        val greeting: String = when (Random(System.currentTimeMillis()).nextInt(3) + 1) {
            2    -> getString(R.string.login_message2)
            3    -> getString(R.string.login_message3)
            else -> getString(R.string.login_message1)
        }
        mainActivityIntent.putExtra("Home_Greeting", greeting)
        println("Login intent argument is : ${mainActivityIntent.extras?.get("Home_Greeting")}")
        startActivity(mainActivityIntent)
        this.finish()
    }

    private fun getUserInfo():User {
        val user = User(loginEmailTextField.text.toString(), null, null, loginPasswordTextField.text.toString(), null, null)
        return user
    }

    private fun displayError() {
        Toast.makeText(this, "Failed to Authenticate", Toast.LENGTH_SHORT).show()
    }
}