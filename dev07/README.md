# dev07 - WROOM clock with NTP client and DS1307

Ignore WARNING about library Timezone class

## Implementation notes

**Wifi** libraries are stored in 
```
~/.arduino15/packages/esp32/hardware/esp32/1.0.4/libraries/WiFi/src
```

`WiFiType.h` defines `wl_status_t`, which is returned by `WiFi.status()`. It takes one of the values   `WL_NO_SHIELD, WL_IDLE_STATUS, WL_NO_SSID_AVAIL, WL_SCAN_COMPLETED, WL_CONNECTED,`
`WL_CONNECT_FAILED, WL_CONNECTION_LOST, WL_DISCONNECTED`

