# Weather Setup for Clockwise

This document explains how to set up weather functionality in your Clockwise device.

## Overview

The weather feature displays current weather conditions next to the WiFi icon on the clockface. It fetches data from OpenWeather API and shows weather conditions like "sunny", "cloudy", "rain", etc.

## Setup Instructions

### 1. Get OpenWeather API Key

1. Go to [OpenWeather](https://openweathermap.org/)
2. Sign up for a free account
3. Navigate to "My API Keys" section
4. Copy your API key

### 2. Find Your City ID

1. Go to [OpenWeather City List](https://bulk.openweathermap.org/sample/)
2. Download the city list file
3. Search for your city and note the `id` field
4. Alternatively, use the [OpenWeather City Finder](https://openweathermap.org/find)

### 3. Configure in Clockwise

1. Connect to your Clockwise device's WiFi access point
2. Open the web interface (usually at 192.168.4.1)
3. Find the "Weather" card in the settings
4. Enter your OpenWeather API key
5. Enter your city ID
6. Save the settings

## Features

- **Weather Conditions**: Displays current weather (clear, cloudy, rain, thunder, snow, fog, etc.)
- **Status Indicators**: Shows "..." when connecting, "error" when API is unreachable
- **Scrolling Text**: Long weather descriptions scroll horizontally for better readability
- **Startup Retry Logic**: Attempts to fetch weather data 5 times at startup with 10-second intervals if the initial fetch fails
- **Automatic Updates**: Refreshes weather data every 5 minutes
- **Error Handling**: Graceful handling of network issues and API errors

## Weather Icons

The system includes weather icons for different conditions:
- ☀️ Clear/Sunny
- ☁️ Cloudy/Partly Cloudy/Overcast
- 🌧️ Rain/Drizzle
- ⚡ Thunder
- ❄️ Snow
- 🌫️ Fog

## Troubleshooting

### "error" Display
- Check your API key is correct
- Verify your city ID is valid
- Ensure internet connectivity
- The system will retry automatically at startup

### No Weather Display
- Verify both API key and city ID are configured
- Check WiFi connection
- Restart the device if issues persist

### Weather Not Updating
- Weather updates every 5 minutes
- Check internet connectivity
- Verify API key has sufficient quota

## Technical Details

- **API Endpoint**: OpenWeather Current Weather API
- **Update Interval**: 5 minutes
- **Startup Retries**: 5 attempts with 10-second intervals
- **Display**: 8x8 pixel icons with scrolling text
- **Cache**: Weather data is cached between updates 