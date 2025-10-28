// UserDaten.h Dummydaten
#pragma once

// WLAN-Zugangspunkt 1
#define STASSID   "ABCDEF"      ///< SSID des primären WLANs
#define STAPSK    "12345678"    ///< Passwort des primären WLANs

// WLAN-Zugangspunkt 2 (Fallback)
#define STASSID2  "GHIJKLM"     ///< SSID des alternativen WLANs
#define STAPSK2   "12345678"    ///< Passwort des alternativen WLANs

// Access Point (Fallback bei fehlender Verbindung)
#define APNAME    "DM-Spieluhr" ///< Name des Access Points
#define APPSK     "nopqrstuv"   ///< Passwort für den Access Point

// OTA-Konfiguration
#define HOSTNAME  "spieluhr-1"  ///< Hostname für OTA-Updates
#define otaPASSWD "wyxz0123"    ///< Passwort für OTA-Zugriff

// I²C-Adresse des PCF8574-Portexpanders
#define I2C_ADDR_PCF8574 0x20   ///< Standardadresse für den Expander

// Ports für DNS und Webserver
#define PORT_AP_DNS 53          ///< DNS-Port für Captive Portal
