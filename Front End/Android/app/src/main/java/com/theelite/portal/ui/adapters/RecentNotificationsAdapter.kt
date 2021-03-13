package com.theelite.portal.ui.adapters

import android.content.Intent
import android.net.Uri
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import androidx.core.content.ContextCompat.startActivity
import androidx.recyclerview.widget.RecyclerView
import com.theelite.portal.Objects.Notification
import com.theelite.portal.R
import com.theelite.portal.ui.ClickListener
import java.util.*
import java.util.concurrent.TimeUnit


class RecentNotificationsAdapter(private val dataSet: MutableList<Notification>, var context: ClickListener) :
    RecyclerView.Adapter<RecentNotificationsAdapter.ViewHolder>() {


    class ViewHolder(view: View) : RecyclerView.ViewHolder(view) {

        val titleTextView: TextView
        val descriptionTextView: TextView
        val dateTextView: TextView
        val recentNotificationsWActionsPeekButton: Button?

        init {
            if (view.getTag(R.layout.recent_notifications_w_actions) == "recent") {
                titleTextView = view.findViewById(R.id.recentNotificationsWActionsTitleLabel)
                descriptionTextView =
                    view.findViewById(R.id.recentNotificationsWActionsDescriptionLabel)
                dateTextView = view.findViewById(R.id.recentNotificationsWActionsDateLabel)
                recentNotificationsWActionsPeekButton = view.findViewById(R.id.recentNotificationsWActionsPeekButton)
            } else {
                titleTextView = view.findViewById(R.id.recentNotificationsNoActionsTitleLabel)
                descriptionTextView =
                    view.findViewById(R.id.recentNotificationsNoActionsDescriptionLabel)
                dateTextView = view.findViewById(R.id.recentNotificationsNoActionsDateLabel)
                recentNotificationsWActionsPeekButton = null
            }
        }

    }

    override fun getItemViewType(position: Int): Int {
        return position
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        var view: View

        println("View type $viewType is odd")
        view = LayoutInflater.from(parent.context)
            .inflate(R.layout.recent_notifications_w_actions, parent, false)
        view.setTag(R.layout.recent_notifications_w_actions, "recent")
        val MAX_DURATION: Long = TimeUnit.MILLISECONDS.convert(20, TimeUnit.MINUTES)
        val now = Date()
        val duration: Long = now.getTime() - dataSet[viewType].date.getTime()

//        = Minutes.minutesBetween(new DateTime(previous), new DateTime())
//            .isGreaterThan(Minutes.minutes(20));
        if (duration >= MAX_DURATION) {
            view = LayoutInflater.from(parent.context)
                .inflate(R.layout.recent_notifications_no_actions, parent, false)
            view.setTag(R.layout.recent_notifications_no_actions, "old")
        }
        /*if (viewType % 2 == 0) {
            println("View type $viewType is even")


        } */
        else {
            println("View type $viewType is odd")
            view = LayoutInflater.from(parent.context)
                .inflate(R.layout.recent_notifications_w_actions, parent, false)
            view.setTag(R.layout.recent_notifications_w_actions, "recent")
        }
        return ViewHolder(view)
    }

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        holder.titleTextView.text = dataSet[position].notification
        holder.descriptionTextView.text = dataSet[position].notification
        holder.dateTextView.text = dataSet[position].date.toString()
        holder.recentNotificationsWActionsPeekButton?.setOnClickListener {
            context.onItemClicked("http://192.168.1.19/stream")
        }
    }

    override fun getItemCount(): Int {
        return dataSet.size
    }
}