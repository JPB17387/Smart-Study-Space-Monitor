# The architecture of the project

## Future Design for the project enclosure
```
+-----------------------------+
| Smart Study Space Assistant |
|                             |
| OLED                        |
| PIR Sensor                  |
| Light Sensor                |
| Push Button                 |
| Status LEDs                 |
|                             |
+-----------------------------+
```
It is okay to use either this 3 (Perfboards, Custom PCB, Foam Board, Acrylic, Cardboard).

### Tools I will use in 3D Designing for the enclosure

> - Onshape
> - Tinkercad

### Dimensions of the Enclosure

> - Width - 120mm
> - Height - 85mm
> - Depth - 40mm

### Internal Layout
```
Top Cover
────────────────────────────

 OLED Window

 PIR Hole

 LDR Hole

 Button Hole

 Buzzer Holes

────────────────────────────


Inside

+-------------------------------------+

 OLED

 Arduino UNO Q

 Breadboard

 Wires

+-------------------------------------+

Bottom Cover
```

### Mounting Layout
```
Top Cover
────────────────────────────

 OLED Window

 PIR Hole

 LDR Hole

 Button Hole

 Buzzer Holes

────────────────────────────


Inside

+-------------------------------------+

 OLED

 Arduino UNO Q

 Breadboard

 Wires

+-------------------------------------+

Bottom Cover
```

### Ventilation 
<p>Include several small ventilation slots on the sides to help dissipate heat and improve airflow.</p>

### USB Access
<p>We'll include a cutout for the USB connector so you can upload new firmware without opening the enclosure.</p>

### Sensor Openings 
<p>We'll design:
<br>
    - a rectangular cutout for the OLED, <br>
    - a circular opening for the PIR Fresnel lens, <br>
    - a small hole for the LDR,<br>
    - several small holes for the buzzer,<br>
    - a round hole for the push button.<br>
</p>