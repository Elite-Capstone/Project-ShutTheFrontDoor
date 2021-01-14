package com.theelite.portal.ui.adapters

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.theelite.portal.Objects.SystemDescription
import com.theelite.portal.R

class SystemsStatusAdapter(private val systemSet: ArrayList<SystemDescription>) :
    RecyclerView.Adapter<SystemsStatusAdapter.SystemViewHolder>() {

    private lateinit var view: View

    class SystemViewHolder(_view: View) : RecyclerView.ViewHolder(_view) {
        val view = _view
        private lateinit var systemsRecyclerView: RecyclerView

        // Used to attach the adapter to the recycler found within each instance
        fun setUpRecyclerView(system: SystemDescription) {
            systemsRecyclerView = view.findViewById(R.id.systemStatusRecyclerView)
            systemsRecyclerView.layoutManager =
                LinearLayoutManager(view.context, RecyclerView.VERTICAL, false)

            systemsRecyclerView.adapter = SingleSysStatusAdapter(system)
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): SystemViewHolder {
         view = LayoutInflater.from(parent.context)
            .inflate(R.layout.card_system_status, parent, false)
        return SystemViewHolder(view)
    }

    override fun onBindViewHolder(holder: SystemViewHolder, position: Int) {
        holder.setUpRecyclerView(systemSet[position])
    }

    override fun getItemCount(): Int {
        return systemSet.size
    }
}
