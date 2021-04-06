package com.theelite.portal.ui.adapters

import android.content.Context
import android.content.Intent
import android.content.SharedPreferences
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.RecyclerView
import com.theelite.portal.Objects.Notification
import com.theelite.portal.R
import com.theelite.portal.ui.ClickListener
import com.theelite.portal.ui.login.LoginActivity
import java.util.*
import java.util.concurrent.TimeUnit


class RecentNotificationsAdapter(private val dataSet: MutableList<Notification>, var context:Context,var onClickContext:ClickListener) :
    RecyclerView.Adapter<RecentNotificationsAdapter.ViewHolder>() {

    private var lockState: Boolean = false
    private var email: String? = null
    private var token: String? = null



    class ViewHolder(view: View) : RecyclerView.ViewHolder(view) {
        val titleTextView: TextView
        val descriptionTextView: TextView
        val dateTextView: TextView
        val recentNotificationsWActionsPeekButton: Button?
        val recentNotificationsWActionsLockButton: Button?

        init {
            if (view.getTag(R.layout.recent_notifications_w_actions) == "recent") {
                titleTextView = view.findViewById(R.id.recentNotificationsWActionsTitleLabel)
                descriptionTextView =
                    view.findViewById(R.id.recentNotificationsWActionsDescriptionLabel)
                dateTextView = view.findViewById(R.id.recentNotificationsWActionsDateLabel)
                recentNotificationsWActionsPeekButton = view.findViewById(R.id.recentNotificationsWActionsPeekButton)
                recentNotificationsWActionsLockButton = view.findViewById(R.id.recentNotificationsWActionsDoorButton)

            } else {
                titleTextView = view.findViewById(R.id.recentNotificationsNoActionsTitleLabel)
                descriptionTextView =
                    view.findViewById(R.id.recentNotificationsNoActionsDescriptionLabel)
                dateTextView = view.findViewById(R.id.recentNotificationsNoActionsDateLabel)
                recentNotificationsWActionsPeekButton = null
                recentNotificationsWActionsLockButton = null
            }
        }

    }

    override fun getItemViewType(position: Int): Int {
        return position
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        var view: View

        view = LayoutInflater.from(parent.context)
            .inflate(R.layout.recent_notifications_w_actions, parent, false)
        view.setTag(R.layout.recent_notifications_w_actions, "recent")
        val MAX_DURATION: Long = TimeUnit.MILLISECONDS.convert(5, TimeUnit.MINUTES)
        val now = Date()
        val duration: Long = now.time - dataSet[viewType].date.time

        if (duration >= MAX_DURATION) {
            view = LayoutInflater.from(parent.context)
                .inflate(R.layout.recent_notifications_no_actions, parent, false)
            view.setTag(R.layout.recent_notifications_no_actions, "old")
        }
        else {
            view = LayoutInflater.from(parent.context)
                .inflate(R.layout.recent_notifications_w_actions, parent, false)
            view.setTag(R.layout.recent_notifications_w_actions, "recent")
        }
        return ViewHolder(view)
    }


    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        loadState()
        if (lockState){
            holder.recentNotificationsWActionsLockButton?.text = "Unlock"
        }
        else{
            holder.recentNotificationsWActionsLockButton?.text = "Lock"
        }
        holder.titleTextView.text = dataSet[position].notification
        holder.descriptionTextView.text = dataSet[position].doorId
        holder.dateTextView.text = dataSet[position].date.toString()
        holder.recentNotificationsWActionsPeekButton?.setOnClickListener {
            onClickContext.onItemClicked("StreamActivity")
        }
        holder.recentNotificationsWActionsLockButton?.setOnClickListener(){
            if (lockState){
                holder.recentNotificationsWActionsLockButton.text = "Lock"
                notifyDataSetChanged()
                lockState = false
                saveState()
                onClickContext.onItemClicked("unlock")

            }
            else{
                holder.recentNotificationsWActionsLockButton.text = "Unlock"
                notifyDataSetChanged()
                lockState = true
                saveState()
                onClickContext.onItemClicked("lock")
            }
        }
    }

    override fun getItemCount(): Int {
        return dataSet.size
    }

    private fun loadState() {
        val sharedPreferences: SharedPreferences = context.getSharedPreferences(LoginActivity.SHARED_PREFS, AppCompatActivity.MODE_PRIVATE)
        lockState = sharedPreferences.getBoolean(LoginActivity.DOORSTATE, false)
        email = sharedPreferences.getString(LoginActivity.EMAIL, null)
        token = sharedPreferences.getString(LoginActivity.TOKEN, null)
    }

    private fun saveState() {
        val sharedPreferences: SharedPreferences = context.getSharedPreferences(LoginActivity.SHARED_PREFS, AppCompatActivity.MODE_PRIVATE)
        val editor: SharedPreferences.Editor = sharedPreferences.edit()
        editor.putBoolean(LoginActivity.DOORSTATE, lockState)
        editor.apply()
    }


}