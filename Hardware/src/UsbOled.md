## USB OLED ##


### Properties ###

|            |                |
|------------|---------------:|
| Dimensions | 27.4 x 12.0 mm |
| Voltage    | 5 V ±10%       |
| Current    | 100 mA         |


### Parts ###

|  # | Part                                      | RefDes  | DigiKey Part Number |
|---:|-------------------------------------------|---------|---------------------|
|  1 | C 100nF X7R 16V (0805)                    | C1      | 478-5311-1-ND       |
|  2 | C 1uF 16V X7R (0805)                      | C2-C3   | 1276-6471-1-ND      |
|  1 | DS LED (0805)                             | DS1     | 475-1415-1-ND       |
|  1 | J USB 2.0 (C)                             | J1      | USB4085-GF-A        |
|  1 | J Header Female (4w)                      | J2      | S7002-ND            |
|  1 | L Ferrite 1A 40Ohm (0805)                 | L1      | 445-2201-1-ND       |
|  3 | R 5.1K 0.125W (0805)                      | R1-R3   | RMCF0805FT5K10CT-ND |
|  1 | U PIC16F1454-I/ST (SSOP-14)               | U1      | PIC16F1454-I/ST-ND  |


#### Optional I²C Pull-Up ####

OLED display is expected to have a pull-up resistors for I²C line (commonly
`4.7K`). No additional I²C resistors are usually required.

If additional I²C pull-ups are needed there are 2 resistor footprints available
on the OLED side that can be populated. E.g., adding `5.1K` pull-ups if `4.7K`
pull up is already available will result in `2.45K` pull-up value. Such strong
pull up is not really necessary but it also shouldn't hurt.


### Revisions ###

| Revision | RA5   | RC5   | Description      |
|----------|-------|-------|------------------|
| A        | Low   | N/A   | First revision.  |
| B        | High  | Low   | Second revision. |


---

*You can check my blog and other projects at [www.medo64.com](https://www.medo64.com/electronics/)*
