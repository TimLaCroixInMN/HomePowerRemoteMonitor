# HomePowerRemoteMonitor
This repository holds the firmware for my Photon board along with a web page that displays battery status remotely.
The web page also allows remote control of a load on the Particle Photon/Power Shield system. 

This project is intended to explore a prototype home power generation and monitoring system.  An example system would generate power from several optional sources (solar, wind, micro-hydro).  It would store excess power in a battery bank or as fuel for a fuel cell or direct excess energy back to the power grid for sale to power utility companies.  Ideally, the system operator would be able to monitor and control it remotely and be notified of any important system events.

My photon board is connected to a Particle Power Shield which is in turn connected to a battery and a solar panel.  The power shield can draw current to charge the battery from the solar panel or from a mini-USB connection.  The photon board is also connected to a load that is modeled by the small motor that was in the Spark kits that we bought (SIK Circuit #12).

The hardware/firmware integrates with both Particle and Ubidots dashboards for publishing data and remote control. Data is acquired from the battery manager and published to both dashboards. The data can be viewed at the respective web sites for Particle and Ubidots. The firmware includes a power management strategy that puts the board to sleep but this is left commented-out since it makes reflashing the firmware from the Particle WEB IDE nearly impossible.

The Ubidots dashboard will present published data graphically along a time axis. My Ubidots system is configured to send an SMS message to my phone if the system battery voltage levels fall below a defined threshold.

The web page, "ComboBatteryMeter.html" makes http get and put requests for fetching data and sending control commands.  These requests are directed to Particle which then transforms the requests to communication to my Photon Board.  Note that the access code embedded in the javascript of this file will expire and needs to be periodically refreshed with a new value from the Particle IDE.

With Ubidots, this same prototype system could be built without Particle using an Arduino baseboard, power shield and WIFI shield.

As an aside, note that a paid Ubidots account allows for integration with the Ubidots math engine.  The Particle battery manager does not support current measurements but if it did, current values could be acquired and sent to Ubidots.  The math engine could then calculate power drawn from the system over time (Power = Voltage * Current) and display this trend graphically as a "derived variable. In other words, the Ubidots math engine could be used to minimize the logic implemented in the embedded endpoint.

Compared to Particle, Ubidots was more:
 - simple (mildly)
 - easily configured
 - feature rich (graphs, controls, events, etc.)
 - money (an individual account costs $13/month)
