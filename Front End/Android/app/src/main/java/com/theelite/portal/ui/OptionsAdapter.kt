package com.theelite.portal.ui

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.theelite.portal.R

class OptionsAdapter(private val dataSet: ArrayList<OptionsModel>, var context: ClickListener) :
    RecyclerView.Adapter<OptionsAdapter.ViewHolder>() {

    /**
     * Provide a reference to the type of views that you are using
     * (custom ViewHolder).
     */
    class ViewHolder(view: View) : RecyclerView.ViewHolder(view) {
        val textView: TextView
        val imageView: ImageView

        init {
            // Define click listener for the ViewHolder's View.
            textView = view.findViewById(R.id.settingsCustomCellTextView)
            imageView = view.findViewById(R.id.settingsCustomCellImageView)
        }

    }

    // Create new views (invoked by the layout manager)
    override fun onCreateViewHolder(viewGroup: ViewGroup, viewType: Int): ViewHolder {
        // Create a new view, which defines the UI of the list item
        val view = LayoutInflater.from(viewGroup.context)
            .inflate(R.layout.options_recycler_cell, viewGroup, false)

        return ViewHolder(view)
    }

    // Replace the contents of a view (invoked by the layout manager)
    override fun onBindViewHolder(viewHolder: ViewHolder, position: Int) {

        // Get element from your dataSet at this position and replace the
        // contents of the view with that element
        viewHolder.textView.text = dataSet[position].name
        viewHolder.imageView.setImageResource(dataSet[position].imageId)
        viewHolder.itemView.setOnClickListener {
            context.onItemClicked(dataSet[position].name)
        }
    }


    // Return the size of your dataset (invoked by the layout manager)
    override fun getItemCount() = dataSet.size
}
