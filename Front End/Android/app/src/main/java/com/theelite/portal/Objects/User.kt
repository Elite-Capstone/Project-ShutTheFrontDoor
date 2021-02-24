package com.theelite.portal.Objects

import android.provider.ContactsContract
import java.util.*

data class User(val email: String, val firstName : String, val lastName: String, val password: String?,val accountId : UUID?, val role: UserRole?)
