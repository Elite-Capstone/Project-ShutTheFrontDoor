package com.theelite.portal.ui.adapters

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.theelite.portal.Objects.Notification
import com.theelite.portal.R
import com.theelite.portal.ui.ClickListener
import java.util.*
import java.util.concurrent.TimeUnit


class RecentNotificationsAdapter(
    private val dataSet: MutableList<Notification>,
    var context: Context,
    var onClickContext: ClickListener, var buttonAction: String
) :
    RecyclerView.Adapter<RecentNotificationsAdapter.ViewHolder>() {

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
                recentNotificationsWActionsPeekButton =
                    view.findViewById(R.id.recentNotificationsWActionsPeekButton)
                recentNotificationsWActionsLockButton =
                    view.findViewById(R.id.recentNotificationsWActionsDoorButton)

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
        } else {
            view = LayoutInflater.from(parent.context)
                .inflate(R.layout.recent_notifications_w_actions, parent, false)
            view.setTag(R.layout.recent_notifications_w_actions, "recent")
        }
        return ViewHolder(view)
    }


    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        holder.recentNotificationsWActionsLockButton?.text = buttonAction
        holder.titleTextView.text = dataSet[position].notification
        holder.descriptionTextView.text = dataSet[position].doorId
        holder.dateTextView.text = dataSet[position].date.toString()
        holder.recentNotificationsWActionsPeekButton?.setOnClickListener {
            onClickContext.onItemClicked("StreamActivity")
        }
        holder.recentNotificationsWActionsLockButton?.setOnClickListener() {
            onClickContext.onItemClicked(buttonAction)
        }
    }

    override fun getItemCount(): Int {
        return dataSet.size
    }
}