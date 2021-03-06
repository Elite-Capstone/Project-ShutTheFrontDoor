package com.theelite.portal.Objects

data class Device(var deviceId: String, var deviceName: String, var status: Status?)

data class Status(var statusList: StatusList, var timeOfPublish: TimeOfPublish?)

data class StatusList(
    var gotWifiIP: Boolean,
    var camInit: Boolean,
    var sdcardInit: Boolean,
    var camStreamInit: Boolean,
    var doorLocked: Boolean,
    var doorClosed: Boolean,
    var batteryLevel: Int
)