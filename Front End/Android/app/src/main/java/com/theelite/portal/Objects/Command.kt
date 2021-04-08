package com.theelite.portal.Objects

data class Command(
    var timeOfPublish: TimeOfPublish?,
    var targetDevice: String,
    var commandRequest: String,
    var requestFlag: Int,
    var commandDelayMS: Int
) {


}