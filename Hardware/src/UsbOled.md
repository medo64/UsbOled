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
|  3 | C 1uF 16V X7R (0805)                      | C2-C4   | 1276-6471-1-ND      |
|  1 | DS LED (0805)                             | DS1     | 475-1415-1-ND       |
|  1 | J USB 2.0 (C)                             | J1      | USB4085-GF-A        |
|  1 | J Header Female (4w)                      | J2      | S7002-ND            |
|  1 | L Ferrite 1A 40Ohm (0805)                 | L1      | 445-2201-1-ND       |
|  1 | R 1K 0.125W (0805)                        | R1      | RMCF0805FT1K00CT-ND |
|  5 | R 5.1K 0.125W (0805)                      | R2-R6   | RMCF0805FT5K10CT-ND |
|  1 | U PIC16F1454-I/ST (SSOP-14)               | U1      | PIC16F1454-I/ST-ND  |


#### Optional I²C Pull-Up ####

Resistors with PU mark next to them (`R5` and `R6`) should be left unpopulated
if additional I²C pull-up is not needed. Assuming OLED board already has the
`4.7K` pull-ups (commonly true), adding `5.1K` pull-ups will result in `2.45K`
pull-up value. Such strong pull up is not really necessary but it also won't
hurt.

Pull-ups are taken from internal PIC 3.3V line and thus must be omitted if
OLED board doesn't use 3.3V logic.


#### Optional Capacitors ####

Capacitors `C3` and `C4` are not needed unless a really long OLED cable is
used. They can be omitted for usage with cables of `2 m` and shorter.


### Revisions ###

| Revision | A5    | Description     |
|----------|-------|-----------------|
| A        | Low   | First revision. |


---

*You can check my blog and other projects at [www.medo64.com](https://www.medo64.com/electronics/)*
