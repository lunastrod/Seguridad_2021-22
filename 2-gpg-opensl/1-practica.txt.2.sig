-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

PRACTICA DE SEGURIDAD EN REDES DE ORDENADORES.

USO DE GPG Y COMANDOS DE OPENSSL

Esta práctica consiste en aprender a utilizar GPG y los comandos de
OpenSSL para cifrar y descifrar ficheros, y gestionar las claves
públicas del resto de usuarios.

ENTREGA: el usuario debe responder a las preguntas en este mismo
fichero, en el hueco indicado. Las respuestas deben estar escritas en
texto plano UTF-8 **CORRECTAMENTE FORMATEADO** , respondiendo a las
preguntas, resumiendo los pasos que ha tenido que seguir para realizar
cada apartado de la práctica y especificando los comandos que ha
ejecutado. SE DEBEN ESPECIFICAR TODOS LOS COMANDOS PARA REALIZAR CADA
PARTE DE CADA APARTADO DEL ENUNCIADO, COPIANDO EL TEXTO DEL TERMINAL
(TANTO COMANDO COMO SALIDA, SIEMPRE QUE NO SEA MUY LARGA LA SALIDA).

Entregue la memoria como indica el último apartado del enunciado.


1. Cree su par de claves GPG, eligiendo como algoritmo RSA de 4096
bits (tanto firma como cifrado) y que caduque en 5 años.
 Elija una passphrase segura.

COMANDO:
====================================================






====================================================

2. Descargue la clave pública del profesor y guárdela en
su anillo de claves. Puede descargar la clave del profesor Enrique Soriano
de:

	http://gsyc.urjc.es/~esoriano

También se puede conseguir de los servidor de claves GPG, como
https://keyserver.ubuntu.com (puedes buscar otros servidores en google).

Compruebe que en ambos sitios (la página personal y un servidor de claves
GPG la clave pública de Enrique Soriano es la misma.

COMANDOS:
====================================================






====================================================

3. Después de descargarlas, ¿puede estar seguro de que esas claves descargadas
son auténticas y pertenecen a Enrique Soriano? ¿Por qué?

RESPUESTA:
====================================================






====================================================

4. Compruebe la autenticidad de la clave del profesor Enrique Soriano
comprobando su  fingerprint con el que ha dado el profesor en persona o por un
canal seguro:

COMANDOS:
====================================================






====================================================

4. ¿Ahora puede estar seguro de que la clave descargada es auténtica
y pertenece al profesor?

RESPUESTA:
====================================================






====================================================

5. Si es así, firme la clave del profesor y suba la firma al servidor
de claves que ha usado antes.

COMANDOS:
====================================================






====================================================

6. Comparta su clave pública con, al menos, otro compañero de clase.
¿Cómo exporta su clave pública? Resuma todos los pasos para compartir su clave
pública de forma segura:

COMANDO y RESPUESTA:
====================================================






====================================================

7. Añada la clave de un compañero (al menos) a su anillo de claves.
Asigne el nivel de confianza que desee.

COMANDOS:
====================================================






====================================================


8. Compruebe la autenticidad y la integridad de las tres versiones
del enunciado que están disponibles (1, 2, 3). ¿Puede asegurar que
alguna de las versiones fue publicada por el profesor? ¿Cuál o cuáles?
¿Por qué?

COMANDO y RESPUESTA:
====================================================






====================================================

9. Descargue del foro de la asignatura el fichero p1secret.gpg,
cifrado con una clave simétrica y descífrelo. El secreto para
descifrar el fichero se comunicará en clase. ¿Con qué algoritmo
estaba cifrado? ¿En qué modo? ¿Qué dice el mensaje?

COMANDOS Y RESPUESTA:
====================================================






====================================================

10. Descargue a firma del mensaje en claro, p1secret.sig.
¿Puede estar seguro de que el mensaje es integro (no ha sido
modificado por un atacante)? ¿Puede el profesor repudiar dicho
mensaje?

COMANDOS Y RESPUESTA:
====================================================






====================================================

11. ¿Oculta dicha imagen algún mensaje con técnicas de esteganografía
mediante la herramienta steghide? Pruebe con la misma contraseña
usada para descifrar.

COMANDOS Y RESPUESTA:
====================================================






====================================================

12. Cifre esa misma imagen con el algoritmo de clave simétrica AES, con
una clave de 256 bits, usando el comando gpg. Haga lo mismo usando el
comando openssl. Tiene que usar el modo CBC.

COMANDO:
====================================================






====================================================

13. Calcule el resumen hash SHA-1 para los dos cifrados anteriores.
¿Coinciden? ¿Deberían coincidir? ¿Por qué?

COMANDO Y RESPUESTA:
====================================================






====================================================

14. Descifre ambos ficheros y compruebe que los datos descifrados son
exactamente los mismos que los originales generando una hash o usando
un comando Unix:

COMANDOS:
====================================================






====================================================

15. Genere un certificado X.509 en texto plano para la compañía ACME,
con correo electrónico pepe@acme.jp. La clave RSA tiene que ser de
4096 bits y el certificado debe ser válido por un año desde su
creación. Indique su número de serie:

COMANDOS:
====================================================






====================================================

16. ¿Cómo puede enviar la clave privada del certificado anterior como
cuerpo de un correo electrónico, en texto plano y aplanado en PEM a un
compañero (de los del ejercicio 7)? ¿Puede el profesor descifrar dicho
mensaje si se hace con el correo electrónico enviado? ¿Y si le roba
sus anillos de claves de su directorio $HOME/.gnugp?

COMANDO Y RESPUESTAS:
====================================================






====================================================

17. ¿Cómo tendría que descifrar y verificar su compañero el mensaje
del punto anterior?

RESPUESTA:
====================================================






====================================================

18. ¿Cuál es el número de serie y el fingerprint del certificado
X509 que usa www.urjc.es? ¿Para qué dominios es válido? ¿Y si el
navegador no entiende la extensión Subject Alternative Names?

COMANDOS Y RESPUESTAS:
====================================================






====================================================


19. Verifique la cadena de confianza del certificado del punto anterior
con el comando openssl. En un sistema GNU/Linux, los certificados raíz
del sistema se encuentran en el directorio /etc/ssl/certs.

COMANDOS:
====================================================






====================================================


20. Entregue este fichero con las respuestas, firmado y cifrado
para que sólo Enrique Soriano pueda leerlo. El fichero se tiene
que llamar practica.gpg. También debe entregar un fichero con su
clave pública, llamado clave.gpg.

Especifique los comandos que va a ejecutar para crear el fichero
practica.gpg:

RESPUESTA:
====================================================






====================================================
-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEEx9w9VDmfw50pCmHJqNbZ9QBUu6YFAmAz6V8ACgkQqNbZ9QBU
u6Y/3Q/+JyITFSE8qySsyscHsgddlF7+GGpCI6kZ/e4bnkMgEdcMJQz8EK0K0ya0
N0oMEoLlmUmRkRWWsg73Lay/QlzCX/3A2Ru6WrrJ2reiDpu0O/IdTFX3drTypQYi
/JcKm4vgtjMms1ypDhF9Ys+oEzFT5xp/xOziMgtMvouNR2DFNuJp+Om6etRK7KF1
cRc/m8u4bIYH/RJQxDqhKE4qg7GNTeblHJoNYg0Qi+Dw32i8UN3TfC4+BwwQ/zvJ
4Eh6fnJtVs9NFvOS7tq0h+4Nz+1uM+RzZPFxx12gBLBIa/jpyW/F/zO+ByJw9OeH
G477BRnCF6QclF8EaDx3Cp8Kc9clQcm8bm0xxjgTJmAIFsITAI31RB5oG1HOLKT7
gYjMqAU+QZSq1QNxuS9G/k9Qes/MnMOLhoLDO0k8Uzgxi1OhDzlBICSX2J0jf4Xe
tICgbyoo0ZKxfvCXo4BUhKsZFZhbGVssotoyyySK4Kql/3Tpu8QGmhgXwctwaQIb
fG4u25Er3AWttudECA/dkzPVuWPyU9M1eyuAZGm+E4e46w5rQTI+RRUq2jRg3bKv
dgLCbc79mobmLgfq61rRZdCUaUHMP0qvt/rEdRCQN71Xc4+zdIq56TtV0Jo25Q0L
JHGf/Fm6zLbpdt8jBGYFwBMShiS74OyXQshrjBy+u4pXNHs9dLk=
=/R96
-----END PGP SIGNATURE-----
