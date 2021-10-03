instrucciones para compilar y ejecutar el programa:

es un programa en python3: ./breakcaesar
si el intérprete está en otra ruta, cambiar "#!/usr/bin/env python3" en la primera linea o llamarlo con "python3 breakcaesar"

caesar también está en python3, pero tiene un argumento, que es la clave: "./caesar 23"

echo -e 'Encryption with Caesar code is based on an alphabet shift' | ./caesar 12 | ./breakcaesar
    key = 12