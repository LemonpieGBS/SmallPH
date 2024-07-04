#include <iostream> // cin y cout
#include <fstream> // Ficheros
#include <string.h> // Por el getline

#include "SmallPH/smallPH.h"

using namespace std;

#define FILE_PATH "passes.txt"
int main() {
    // Este ejemplo demostrará como usar PasswordLock para validar
    fstream dataFile(FILE_PATH, ios::in);

    // Casi todo el tiempo comprueba si el archivo existe
    if(dataFile.is_open()) {
        // Aqui se inicia sesión

        cout << "=== INICIO DE SESION ===" << endl;
        string usuario, input;

        // Pedimos los inputs del usuario
        cout << "Usuario: ";
        getline(cin,usuario);

        cout << "Contrasena: ";
        getline(cin, input);

        // Obtengamos los datos de la cuenta
        string comp_usuario;
        PASSWORD comp_pass;

        getline(dataFile, comp_usuario);
        getline(dataFile, comp_pass.hash);
        getline(dataFile, comp_pass.salt);

        bool validado = false;
        
        do {

            validado = true;

            // Ahora podemos validar, para validar una contraseña, pasamos el input y el objeto
            if(!validatePassword(input,comp_pass)) validado = false;
            if(usuario != comp_usuario) validado = false;

            if(!validado) {

                cout << "=== LOS DATOS SON INCORRECTOS ===" << endl;
                cout << "Intente nuevamente: " << endl;

                cout << "Usuario: ";
                getline(cin,usuario);

                cout << "Contrasena: ";
                getline(cin,input);
            }

        } while(!validado);

        cout << "Acceso Ganado!" << endl;
        system("pause");
        
    } else {
        // Aqui se crea una cuenta en el archivo
        cout << "=== CREACION DE CUENTA ===" << endl;

        string usuario, contrasena_temporal;

        cout << "Nuevo Usuario: ";
        getline(cin, usuario);

        cout << "Nueva Contrasena: ";
        getline(cin, contrasena_temporal);

        // Aqui creamos un objeto tipo PASSWORD, que es la verdadera contraseña
        PASSWORD pass = createPassword(contrasena_temporal);

        // pass tiene dos componentes que son esenciales para poder validar:
        // hash: el propio hash de la contraseña
        // salt: string aleatorio para ofuscarla

        // AMBOS SE TIENEN QUE GUARDAR!!! AMBOS!!!

        // Guardemos
        dataFile.open(FILE_PATH, ios::out);

        dataFile << usuario << endl;
        dataFile << pass.hash << endl;
        dataFile << pass.salt << endl;

        dataFile.close();
    }

    return 0;
}