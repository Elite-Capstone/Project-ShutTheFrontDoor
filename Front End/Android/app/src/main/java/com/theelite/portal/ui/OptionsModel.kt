package com.theelite.portal.ui

import android.content.Context
import android.content.res.Resources

data class OptionsModel(val name: String, val imageId: Int)

class OptionsList {
    companion object {
        fun getListOfSettingsOptions(
            context: Context?,
            options: Array<String>,
            icons: Array<String>, resources: Resources
        ): ArrayList<OptionsModel> {
            val settingsOptions = ArrayList<OptionsModel>()

            for (i in options.indices) {
                val logo = resources.getIdentifier(icons[i], "drawable", context?.packageName)
                settingsOptions.add(OptionsModel(options[i], logo))
            }

            return settingsOptions
        }
    }

}