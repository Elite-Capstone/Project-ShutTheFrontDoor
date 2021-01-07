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
    RecyclerView.Adapter<SystemsStatusAdapter.ViewHolder>() {

    private lateinit var view: View
    class ViewHolder(view: View) : RecyclerView.ViewHolder(view) {

        init {
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
         view = LayoutInflater.from(parent.context)
            .inflate(R.layout.card_system_status, parent, false)
        return ViewHolder(view)
    }

    // TODO: Create an instance for each system in the systemSet and attach an adapter
    //  to the recycler view within the instance
    override fun onBindViewHolder(holder: ViewHolder, position: Int) {

    }

    // Used to attach the adapter to the recycler found within each instance
    private fun setUpRecyclerView(index: Int) {
        val systemRecyclerView = view.findViewById<RecyclerView>(R.id.systemStatusRecyclerView)
        systemRecyclerView.layoutManager =
            LinearLayoutManager(view.context, RecyclerView.VERTICAL, false)

        systemRecyclerView.adapter = SingleSysStatusAdapter(systemSet[index].getSysDevices())
    }

    override fun getItemCount(): Int {
        return 2 + systemSet.size
    }
}
