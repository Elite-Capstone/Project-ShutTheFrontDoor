package com.theelite.portal.Database

//import android.content.ContentValues
//import android.content.Context
//import android.database.sqlite.SQLiteDatabase
//import android.database.sqlite.SQLiteOpenHelper
//import android.os.Build
//import androidx.annotation.RequiresApi
//import com.theelite.portal.Objects.Notification
//import java.time.LocalDate
//import java.time.ZonedDateTime
//import java.time.format.DateTimeFormatter
//import java.util.*
//import kotlin.collections.ArrayList

//class DBHelper (context: Context):SQLiteOpenHelper(context,DATABASE_NAME,null,DATABASE_VER) {

//    companion object{
//        private val DATABASE_VER = 1
//        private  val DATABASE_NAME = "doorhub.db"
//
//        private val TABLE_NAME = "Notifications"
//        private val COL_ID = "ID"
//        private val COL_NOTIFICATION = "Notification"
//        private val COL_DATE = "Date"
//        private val COL_DoorID = "DoorID"
//    }
//
//    override fun onCreate(db: SQLiteDatabase?) {
//        val CREATE_TABLE_QUERY = ("CREATE TABLE $TABLE_NAME(" +
//                "$COL_ID INTEGER PRIMARY KEY AUTOINCREMENT," +
//                "$COL_DATE TEXT," +
//                "$COL_DoorID TEXT," +
//                "$COL_NOTIFICATION TEXT)")
//        db!!.execSQL(CREATE_TABLE_QUERY)
//    }
//
//    override fun onUpgrade(db: SQLiteDatabase?, oldVersion: Int, newVersion: Int) {
//        db!!.execSQL("DROP TABLE IF EXISTS $TABLE_NAME")
//        onCreate(db!!)
//    }
//
//        val allNotifs:List<Notification>
//            @RequiresApi(Build.VERSION_CODES.O)
//            get() {
//                val lstNotifs = ArrayList<Notification>()
//                val selectQuery = "SELECT * FROM $TABLE_NAME"
//                val db = this.writableDatabase
//                val cursor = db.rawQuery(selectQuery,null)
//                if(cursor.moveToFirst()){
//                    do {
//
//                        val stringDate = cursor.getString(cursor.getColumnIndex(COL_DATE))
//                        val formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd'T'HH:mm:ss.SSS")
//                        val dt = ZonedDateTime.parse(stringDate, formatter)
//                        var date = Date.from(dt.toInstant())
//                        var doorId = UUID.fromString(cursor.getString(cursor.getColumnIndex(COL_DoorID)))
//                        var accountId = UUID.fromString("00000000-0000-0000-0000-000000000000")
//                        var notification = cursor.getString(cursor.getColumnIndex(COL_NOTIFICATION))
//                        var notif = Notification(notification,date,accountId,doorId)
//                        lstNotifs.add(notif)
//                    }while (cursor.moveToNext())
//                }
//                db.close()
//                return lstNotifs
//            }
//
//    fun addNotif(notif:Notification){
//        val db = this.writableDatabase
//        val values = ContentValues()
//        values.put(COL_NOTIFICATION,notif.notification)
//        values.put(COL_DoorID,notif.doorId.toString())
//        values.put(COL_DATE,notif.date.toString())
//        db.insert(TABLE_NAME,null,values)
//        db.close()
//    }
//
//    fun deleteAll(){
//        val db = this.writableDatabase
//        db!!.execSQL("DELETE FROM $TABLE_NAME;")
//        db!!.execSQL("VACUUM;")
//    }
//}