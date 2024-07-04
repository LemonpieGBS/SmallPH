<h1 style="text-align: center">SmallPH</h1>

![SmallPH](https://socialify.git.ci/LemonpieGBS/SmallPH/image?font=KoHo&language=1&name=1&owner=1&pattern=Diagonal%20Stripes&theme=Light)

He creado una librer�a sencilla y f�cil de implementar para C++ que permite hashear y validar contrase�as de manera f�cil, **sin necesidad de almacenar el propio texto de la contrase�a**, efectivamente encriptandola.

SmallPH (abreviatura de Small Password Hasher) es una peque�a librer�a que he desarrollado para facilitar su uso en **programas peque�os con sistemas de inicio de sesi�n**. Guardar una contrase�a en texto plano es irresponsable, e incluso cifrarla (un proceso que con muchas librer�as puede ser complicado) siempre conlleva el riesgo de que se descubra un m�todo para descifrarla. Por ello, se recomienda 'hashear' una contrase�a en lugar de cifrarla, ya que **el hashing es un proceso unidireccional y no permite revertir la contrase�a a su forma original**.

<h2>Tabla de Contenidos</h2>
<ul>
    <li>
    <a href="#Funcionamiento"> Funcionamiento (parte t�cnica y aburrida)
    </a>
    <ul>
        <li>
        <a href="#Hashing"> Hashing
        </a>
        </li>
        <li>
        <a href="#Salt"> Salt
        </a>
        </li>
        <li>
        <a href="#CSPRNG"> CSPRNG
        </a>
        </li>
    </ul>
    </li>
    <li>
    <a href="#Uso"> Uso (Parte Interesante)</a>
    </li>
    <li>
    <a href="#Compat"> Compatibilidad</a>
    </li>
    <li>
    <a href="#LibsUsadas"> Liber�as Usadas</a>
    </li>
</ul>
<br>

<h1 id="Funcionamiento">
Funcionamiento</h1>

<h2 id="Hashing">Hashing</h2>

Hashing es un proceso que **convierte una cadena de caracteres** en otra **aparentemente** aleatoria. En el caso de SHA256, esta cadena tiene una longitud de **64 caracteres**.

Lo �til del hashing es que "aparentemente" es una palabra clave, ya que de hecho un **mismo input SIEMPRE retorna un mismo hash**.

Ejemplo: <br>
```python
# 1234 convertido a Hash
1234 = 03ac674216f3e15c761ee1a5e255f067953623c8b388b4459e13f978d7c846f4
```

La cadena "1234" **siempre va a retornar el mismo hash**, lo que es �til para validar contrase�as, ya que **simplemente comprobamos que los hashes sean iguales**. As�, no tenemos que guardar la contrase�a en texto plano, sino en forma de hash.

Sin embargo, existe un problema con esto: si, por ejemplo, 1000 usuarios usan la misma contrase�a, **el hash ser� el mismo**. Alguien que vea la base de datos podr�a detectar el patr�n y, **si logra descubrir a qu� contrase�a corresponde** el hash, **podr� acceder a todas esas cuentas**.

Adem�s, **los hashes de contrase�as comunes ya estan en Internet**. Si quieres hacer el experimento, busca el hash anterior. Esto no es ideal, y para solucionar este problema se utiliza el 'salting'.<br><br>

<h2 id="Salt">Salt</h2>

El salting es **agregar una cadena aleatoria** a la contrase�a que se esta hasheando, de esa manera, **creamos una segunda contrase�a** que logra hacer que incluso usuarios usando la misma cuenta **tengan hashes completamente distintos**, y adicionalmente, estos hashes al ser de cadenas totalmente distintas, **no se encuentran en Internet.**

Por ejemplo, para dos usuarios distintos se generan salts aleatorias y se agregan al principio, en este caso, de 5 caracteres:

<hr>

Contase�a: 1234<br>
Salt: P271A (generado aleatoriamente)

```python
# 1234 con salt convertido a Hash
P271A1234 = c429d503c12b96fdc0cba4e325d19d14d7b88f7dec0ed19ca66f818c8e5bf5e5
```

<hr>

Contase�a: 1234<br>
Salt: H6y71 (generado aleatoriamente)

```python
# 1234 con salt convertido a Hash (resultado distinto)
H6y711234 = 6304fc6110aa9d9eb856c4df872983b7ed9872ad948331c20cbbf00f72a55d7a
```

<hr>

Gracias a esto, cada vez que querramos validar la contrase�a tan solo tenemos que hashear `(salt + contrase�a)` y si coincide con el hash original, estamos ante la misma contrase�a y de esta manera reforzamos la seguridad de nuestro sistema.<br><br>

<h2 id="CSPRNG">CSPRNG</h2>

Este es tan solo un peque�o a�adido, la propia librer�a **genera 32 caracteres aleatorios** usando **CSPRNG** (Cryptographically Secure Pseudo-Random Number Generator).

Un CSPRNG es un tipo de generador de n�meros pseudoaleatorios dise�ado para **cumplir con los requisitos de seguridad criptogr�fica**. A diferencia de los generadores de n�meros pseudoaleatorios comunes, un CSPRNG **produce secuencias de n�meros que son extremadamente dif�ciles de predecir**, lo que los hace ideales para aplicaciones de criptograf�a, donde la seguridad y la imprevisibilidad son cruciales.

En este caso, se usa para que el salt sea lo menos predecible posible y as� que la seguridad del hash aumente. El m�todo tradicional de establecer una semilla usando `time(0)` es bueno pero no criptogr�ficamente seguro.
<br><br>

<h1 id="Uso">Uso</h1>

Esta librer�a es f�cil de implementar, tan solo copia la carpeta `SmallPH` del source code y ponla adjunto a tu proyecto.

```
Folder de tu proyecto:
- SmallPH (Folder del repositorio)
- main.cpp (Tu archivo main)
- ... (Resto de cosas)
```

Seguidamente implementamos el header file:

```cpp
#include "SmallPH/smallPH.h"
```

Y ya, podremos usar SmallPH.<br><br>

<h2 id="Crear">Crear un PASSWORD</h2>

En el caso de que no hayas le�do el funcionamiento, explicado de manera r�pida, necesitar�s dos cosas para poder crear y validar tu contrase�a: un **Hash** y un **Salt**.

Usa la estructura incluida **PASSWORD** y la funci�n `createPassword()` para hacerlo, como parametro pasamos el string que queremos que sea la contrase�a:
```cpp
PASSWORD pass; // Creamos una contrase�a
pass = createPassword("Hola Mundo"); // Esta contrase�a es "Hola Mundo"
```
`createPassword()` generar� el hash y el salt de manera autom�tica y ser�n cargados a la estructura creada, **el string de la contrase�a NO SE NECESITA ALMACENAR** (y mejor si no lo almacenas del todo), **la liber�a no necesita el string de la contrase�a asignada para poder validarse**.

Es muy posible que quieras almacenar el hash y el salt en un fichero o en una base de datos, para accesarlos, se accesan como un struct normal:

```cpp
PASSWORD pass;
string hashGenerado, saltGenerado;

hashGenerado = pass.hash; // Consigue el hash de pass
saltGenerado = pass.salt; // Consigue el salt de pass
```

<h2 id="Validar">Validar un PASSWORD</h2>

Quisiera reiterar, **NO TIENES, Y POR BUENA PRACTICA, NO DEBES ALMACENAR LAS CONTRASE�AS EN TEXTO**, la liber�a permite validar las contrase�as sin necesidad de almacenar el texto original, del todo.

Para validar una contrase�a se necesita el objeto de contrase�a y el valor que se busca validar:

```cpp
PASSWORD pass;
pass = createPassword("Hola Mundo"); // Esta contrase�a es "Hola Mundo"

cout << validatePassword("Hola Mundo",pass); // Devolver� verdadero ya que el valor de la contrase�a es Hola Mundo

cout << validatePassword("1234",pass); // Devolver� falso porque el valor de la contrase�a NO es 1234
```

<h2 id="Almacenar">Almacenar un PASSWORD</h2>

DE NUEVO, **NO ALMACENAR EL TEXTO PLANO DE LA CONTRASE�A**, EL HASH Y EL SALT SE ENCARGAN DE HACER LA VALIDACI�N POSIBLE SIN NECESIDAD DEL TEXTO ORIGINAL. Sabiendo esto, para almacenar una contrase�a simplemente almacenamos el hash y el salt.

```cpp
PASSWORD pass;
pass = createPassword("Pepito123"); // Esta contrase�a es "Pepito123"

// Abrimos un archivo con fstream para output
ofstream::pass_file;

// Imprimamos al archivo:
pass_file << pass.hash; // Accesamos el hash
pass_file << pass.salt; // Accesamos el salt

pass_file.close(); // Cerramos el archivo
```

Para ahora accesar al password, lo hacemos como datos normales:

```cpp
PASSWORD input_pass;

// Abrimos un archivo para lectura
ifstream::pass_file;

// Sacamos l�nea por l�nea del archivo
getline(input_pass, pass.hash); // Accesamos el hash
getline(input_pass, pass.salt); // Accesamos el salt

pass_file.close();
```

En el caso de tener alguna duda, pueden ver `demo.cpp` que pone en pr�ctica lo mencionado en este c�pitulo.<br><br>

<h1 id="Compat">Compatibilidad</h1>

Esta liber�a **EN PRINCIPIO** deber�a funcionar en todos los sistemas operativos, porfavor notificar por medio de Issues en el caso de que esto no sea cierto.<br><br>

<h1 id="LibsUsadas">Librerias Usadas</h1>

Un abrazo a **System-Glitch** por la liber�a de hashing SHA256 para C++.

Link a la liber�a original: https://github.com/System-Glitch/SHA256/tree/master

Link al perfil de **System-Glitch**: https://github.com/System-Glitch
<br><br><br>
Otro abrazo a **Duthomhas** por la liber�a de CSPRNG para C++ y C: 

Link a la liber�a original: https://github.com/Duthomhas/CSPRNG

Link al perfil de **Duthomhas**: https://github.com/Duthomhas