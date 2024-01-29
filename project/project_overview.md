# Daisyworld design

## Objective

```bash
💮💮🌸🌸🌸🌸🌸💮💮  
🌸💮💮🌸🌸🌸🌸🌸🌸🌸
🌸💮💮💮🌸🌸🌸🌸🌸🌸
💮🌸💮🌸🌸🌸🌸🌸🌸🌸
🌸💮🌸💮💮🌸🌸💮💮🌸
🌸🌸🌸💮💮🌸🌸🌸💮🌸
  🌸🌸💮💮🌸💮🌸💮💮
  🌸🌸🌸🌸🌸🌸💮💮🌸
    🌸🌸🌸🌸🌸🌸🌸🌸
            🌸🌸🌸🌸

Global temperature: 29.1791
Black daisies: 63
White daisies: 26
Barren land: 11
Solar Luminosity: 0.9
```

- Simulate the evolution of a population of two different type of flowers in a
  2D world, based on the Daisyworld model.

- Output the values of population and global temperature based on the solar
  intensity.

- Show the simulation tick by tick for small grids.

## Design

- The world is represented by a matrix of patches (cells of the grid) that can
  either be barren or populated by one of two types of daisy (black or white).

- For every iteration the solar luminosity can increase, decrease or remain
  constant, based on the behaviour we want to study.

- The temperature of the patches of land are determined by the albedo of their
  surface, different for black and white daisy and barren land, and the solar
  luminosity.

- The temperature of a patch of land influences the temperature of the
  neighbouring terrain.

- The flowers age with the ticks of the simulation and die out when they reach a
  specific age, turning their patch of land barren.

- The live flowers spread to the neighbouring land based on seeding probability
  dependent on their local temperature.

## Simulation loop

```pseudocode
START
Set parameteres
Setup simulation
LOOP
  Set solar luminosity
  FOR every patch of land
    Compute temperature
    Change temperature of neighbours
    Age and kill daisies
    Spread live daisies to neighbours
  Compute population
  Compute global temperature
  Write to file/screen
END
```

## Parameters

- World size  
  `int` > 0. Number of rows/columns.
- Albedo (black daisy, white daisy and barren land)  
  `double` between 0 and 1. Percentage of light reflected.
- Solar luminosity  
  `double` where 1 is our Sun's.
- Max age of daisy  
  `int` > 0. Number of ticks before the plant dies.
- Diffusion rate  
  `double` between 0 and 1. Percentage of patch temperature 'spread' between the
  neighbours.
- Starting percentage population (black and white daisies)  
  `double` between 0 and 1. Percentage of black and white daisies in the initial
  population.
