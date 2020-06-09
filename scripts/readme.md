/* - simple program to show Dashboard
/* - simple scripts to controll blindx, hvac and led by mqtt
/* by Kuba Bartoszewicz

#showDashboard
- at folder showDashboard type 'make' to compile the programm
- type '$watch -d -n 1 ./show_dashboard' to run dashboard refreshed every 1 sec.

#control scripts
$./blinds <blind_id> <value>    /* where blind_no = 1 refer to blind id = 71 etc.
$./led <led_id> <hue>           /* where led_id = 1 refer to led id = 21 etc. Saturation and Value by default eq. 100
$./hvac <room_id> <status>      /* force HVAC to run. <status>: h -heating, c - cooling  
$./hvac                         /* force HVAC to stop at all rooms.