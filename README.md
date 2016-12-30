# NodeControl

Libreria para controlar los pines de un microcontrolador NodeMCU desde 
la web.

### Funcionalidaes
  - Se conecta al Wifai
  - Enciende y apaga leds
  - Muestra informacion de si mismo y sus elementos

### To do:
  - Falta hacer una app para controlarlo xD

### USO
Para utilizarlo, clonar este repositorio y abrir el archivo .ino y antes 
de enviar el codigo al NodeMCU cambiar las configuraciones como los 
datos del WiFi, los pines y los nombres de los pines.
##### Endpoints
 - /info
  -- Retorna informcación del dispositivo como su nombre, los elementos 
(pines controlables) y su dirección IP.
- /action?pin=0&level=1
-- pin = id del pin a manipular(id obtenido en /info).
-- level: 0 = apagado, 1 encendido.
- /all/on 
-- Enciende todos los pines .
- /all/off
-- Apaga todos los pines.

### Autor
Andrés Alonzo

### Colaboradores
Nadie :'(  
