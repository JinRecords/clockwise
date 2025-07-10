#pragma once

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "CWPreferences.h"

struct WeatherData {
  String condition;
  String description;
  int temperature;
  int humidity;
  bool isValid;
  
  WeatherData() : temperature(0), humidity(0), isValid(false) {}
};

struct CWWeatherService {
  static CWWeatherService* getInstance() {
    static CWWeatherService instance;
    return &instance;
  }

  WeatherData getCurrentWeather();
  void updateWeather();
  String getWeatherCondition(const String& weatherId);
  
private:
  WiFiClientSecure client;
  WeatherData currentWeather;
  unsigned long lastUpdate = 0;
  const unsigned long UPDATE_INTERVAL = 300000; // 5 minutes
  
  String makeWeatherRequest();
  bool parseWeatherResponse(const String& response, WeatherData& weather);
}; 