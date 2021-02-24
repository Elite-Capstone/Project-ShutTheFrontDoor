package com.theelite.portal.ui.register

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
import retrofit2.Call
import retrofit2.Callback
import retrofit2.Response

class RegisterActivity : AppCompatActivity() {
    private lateinit var registerButton: Button
    private lateinit var firstNameTextField: TextInputEditText
    private lateinit var lastNameTextField: TextInputEditText
    private lateinit var emailTextField: TextInputEditText
    private lateinit var passwordTextField: TextInputEditText
    private lateinit var confirmPasswordTextField: TextInputEditText


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_register)
        setUpRegisterView()

    }

    fun setUpRegisterView() {
        registerButton = findViewById(R.id.registerButton)
        firstNameTextField = findViewById(R.id.registerFirstNameTextField)
        lastNameTextField = findViewById(R.id.registerLastNameTextField)
        emailTextField = findViewById(R.id.registerEmailTextField)
        passwordTextField = findViewById(R.id.registerPasswordTextField)
        confirmPasswordTextField = findViewById(R.id.registerConfirmPasswordTextField)

        registerButton.setOnClickListener {
            addNewUser()
        }
    }


    fun addNewUser() {
        val retrofit = RetroFit.get(getString(R.string.url))
        val userService = retrofit.create(UserService::class.java)

        val call = userService.newUser(getUserInfo())
        call.enqueue(object : Callback<Boolean> {
            override fun onResponse(call: Call<Boolean>, response: Response<Boolean>) {
                if (response.isSuccessful && response.body()!!) moveToHomeActivity() else couldNotCreateUser()
            }


            override fun onFailure(call: Call<Boolean>, t: Throwable) {
                Toast.makeText(this@RegisterActivity, "${t.message}", Toast.LENGTH_LONG).show()
            }
        })
    }

    fun moveToHomeActivity() {
        val mainActivity = Intent(this, MainActivity::class.java)
        mainActivity.putExtra("Home_Greeting", getString(R.string.registration_message))
        startActivity(mainActivity)
        this.finish()
    }

    fun couldNotCreateUser() {
        Toast.makeText(
            this@RegisterActivity,
            "Could not create user, email might already be used",
            Toast.LENGTH_LONG
        ).show()
    }

    fun checkContentFields(): Boolean {
        return true
    }

    fun getUserInfo(): User {
        return User(
            emailTextField.text.toString(),
            firstNameTextField.text.toString(),
            lastNameTextField.text.toString(),
            passwordTextField.text.toString(),
            null,
            null
        )
    }
}