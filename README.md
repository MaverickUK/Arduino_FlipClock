# Arduino: FlipClock
Pacemaker code for Arduino to drive flipclock

![Groundhog day style flipboard](https://64.media.tumblr.com/2b3c8e8da6c6dd852bf7537342be8be5/2d33f5e3fe1e74f2-9b/s540x810/01073d6185cc428b6fb5bc6b189f59dc740de767.jpg)

The clock in our dinning room has a really nice flip date feature. This means it's possible to see the current month and day through a nice physical flip mechanism. Unfortunately after several years of faithful service the signalling mechisum without the clock portion stopped sending the signal every 12 hours to change the date portion.

This solution uses an Arduino Pro Mini as a low powered pacemaker, which sends a signal to a relay every 12 hours. An external RTC module to used to ensure accurate time is kept.

!(Circuit)[https://64.media.tumblr.com/ee987bf2ba70cee2b889b7019c345558/2d33f5e3fe1e74f2-ca/s540x810/d689aae4c26dbf4dc3ee5608076acbf4efde1a66.jpg]

## Parts list
- **Arduino Pro Mini 3.3v:** Available direct or through eBay clones
- **LCD I2C 16x2 display:** Again widely available on eBay
- **Relay:** Another cheap eBay item
- **Adafruit DS1307 Real Time Clock:** £6.50 from the [PiHut](https://href.li/?https://thepihut.com/products/adafruit-ds1307-real-time-clock-assembled-breakout-board)
- **Adafruit Micro Lipo charger:** £6.60 from [Pimoroni](https://href.li/?https://thepihut.com/products/adafruit-ds1307-real-time-clock-assembled-breakout-board)
- **LiPo Battery Pack – 2000mAh:** £13.50 from [Pimoroni](https://href.li/?https://shop.pimoroni.com/products/lipo-battery-pack?variant=20429082247)
