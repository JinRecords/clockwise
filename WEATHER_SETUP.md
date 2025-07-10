# Weather Setup Guide

This guide explains how to set up weather functionality for the Clockwise clockface.

## Prerequisites

1. **OpenWeather API Key**: You need to get a free API key from [OpenWeatherMap](https://openweathermap.org/api)
2. **City ID**: You need to find your city's ID from OpenWeatherMap

## Setup Steps

### 1. Get OpenWeather API Key

1. Go to [OpenWeatherMap](https://openweathermap.org/api)
2. Sign up for a free account
3. Navigate to "My API Keys" section
4. Copy your API key

### 2. Find Your City ID

1. Go to [OpenWeatherMap City List](https://bulk.openweathermap.org/sample/)
2. Download the city list or search for your city
3. Find your city's ID (it's a number)

Alternatively, you can use this URL to find your city ID:
```
https://api.openweathermap.org/data/2.5/weather?q=YOUR_CITY_NAME&appid=YOUR_API_KEY
```

### 3. Configure Weather Settings

1. Access the Clockwise settings webpage
2. Navigate to the "Weather" card
3. Enter your OpenWeather API key
4. Enter your city ID
5. Save the settings

## Weather Conditions

The system displays the following weather conditions:

- **clear**: Sunny/clear sky
- **cloudy**: Cloudy conditions
- **partly**: Partly cloudy
- **overcast**: Overcast conditions
- **rain**: Rain
- **drizzle**: Light rain/drizzle
- **thunder**: Thunderstorm
- **snow**: Snow
- **fog**: Fog/mist

## Display

The weather information is displayed:
- **Icon**: 8x8 pixel weather icon next to the WiFi icon
- **Text**: Weather condition name (truncated to 6 characters)

## Update Frequency

Weather data is updated every 5 minutes to avoid hitting API rate limits.

## Troubleshooting

### Weather not showing
1. Check that your API key is correct
2. Verify your city ID is valid
3. Ensure you have an internet connection
4. Check the serial monitor for error messages

### API Errors
- **401**: Invalid API key
- **404**: City not found
- **429**: Rate limit exceeded (free tier allows 60 calls/minute)

## API Limits

The free OpenWeather API has the following limits:
- 60 calls per minute
- 1,000,000 calls per month

With 5-minute update intervals, you'll use approximately 288 calls per day, well within the free tier limits. 