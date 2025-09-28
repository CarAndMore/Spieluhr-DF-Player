# Spieluhr-DF-Player

##Struktur & Hinweise
SpieluhrDFPlayer.h kapselt alles rund um DFPlayer, EEPROM, Zeitsteuerung und Web-Endpunkte.

spieluhr.begin() übernimmt Initialisierung von DFPlayer, EEPROM und LED.

spieluhr.loop() ersetzt dein waitMilliseconds() und checkAndPlayMp3() – alles sauber gekapselt.

setupWebEndpoints() registriert alle HTTP-Endpunkte (/play, /status, /config, etc.).

playStartupMelody() spielt Track 32 beim Start.
