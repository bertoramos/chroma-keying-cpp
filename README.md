# Aplicación de croma

![Chroma keying result](output.png)
*Resultado*

En este repositorio se incluye un programa en C++ que aplica la técnica de croma para sustituir el fondo de una imagen.

## Instalación

Para utilizar este programa es necesario compilarlo con *cmake* (se incluye el fichero *CMakeLists.txt*).

Además, este programa tiene las siguientes dependencias:

- [Boost](https://github.com/boostorg/boost) >=1.71.
- [Boost Generic Image Library](https://github.com/boostorg/gil).
- PNG library. ([libpng-dev](https://packages.ubuntu.com/bionic/libpng-dev)).

## Comando

Los parámetros de este programa son:
- ***--t*** (por defecto 1) : valor de *threshold*.
- ***--bg*** : imagen para el fondo (solo ficheros PNG).
- ***--fg*** : imagen a eliminar el fondo (solo ficheros PNG).
- ***--o*** (por defecto ../output.png).
- ***key-color*** : valores R G y B del color clave.


Un ejemplo de comando es:
```
./chroma --fg ../../fotos_de_prueba/con_croma/image2.png --bg ../../fotos_de_prueba/con_croma/fondos/fox.png --key-color 0 254 0 --t 1.5
```

Que nos devolvería la imagen resultado de la portada en un fichero *output.png*.
