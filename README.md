# KL25Z-Automatic-Irrigation-System

This project is a automatic plant irrigation system. There are two modes in the system. Firts mode uses two hygrometer to measure moisture of the soil so that the system can decide whether it should water the plants or not. Second mode is a timer-interrupt based system. When the time comes irrigation system begins to work. The second mode has its flaws so it is not very efficient in my opinion. 

In main file, both main code and timers' code are available. I used PIT and SYSTICK. SYSTICK is only for generating the delay. PIT is for the second mode. TPM0 is used to work servo motor (SG90). ADCs are for two hygrometer and one water level sensor. One gpio is used for the dc motor (pump) with a motor driver (L298). There is also one 2*16 LCD screen. Some codes are from Mazidi but I changed them to use in the project.
