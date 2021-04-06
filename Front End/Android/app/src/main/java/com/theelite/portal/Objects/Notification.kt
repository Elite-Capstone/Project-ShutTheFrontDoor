package com.theelite.portal.Objects

import io.realm.RealmObject
import java.util.*

open class Notification() : RealmObject() {
    lateinit var notification: String
    lateinit var date: Date
    lateinit var accountId: String
    lateinit var doorId: String

    constructor(notification: String, date: Date, accountId: String, doorId: String) : this()
}
