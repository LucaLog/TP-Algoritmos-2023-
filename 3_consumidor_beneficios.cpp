#include <iostream>
#include <string>
#include <cstring>
using namespace std;

struct Alumno{
    char mail[100];
    char contraseña[100];
    int creditos = 1000;
};

struct Beneficio{
    char nombre[50];
    int costo;
};

Alumno pedirAlumno();
void validarUsuario(Alumno alumnoIngresado);
void mostrarBeneficios(Alumno alumnoIngresado);
void canjearBeneficio(Alumno alumnoIngresado);
FILE *archivoBeneficios;
FILE *registrosAlumnos;

int main(){
    Alumno alumnoIngresado;
    alumnoIngresado = pedirAlumno();

    return 0;
}

//---------------------FUNCIÓN 1-----------------------
/*Esta función recibe el mail del alumno que quiere ingresar al sistema. En la funcion validar usuario se verifica si el mail 
existe y si su contraseña*/
Alumno pedirAlumno(){
    Alumno alumnoIngresado;
    Alumno alumno;
    cout << "Bienvenido al Gestor de la UTN. Por favor inicie sesion" << endl;
    cout << "Ingrese su mail: " << endl;
    cin >> alumnoIngresado.mail;

    validarUsuario(alumnoIngresado);
    return alumnoIngresado;
}

//----------------------FUNCIÓN 2------------------------
/*Abre los 2 archivos. Si ambos no son vacios, empieza a leerlos (primero el de alumnos). Si el mail ingresado
esta registrado en el sistema, le muestra la cantidad de creditos que tiene y los beneficios que puede canjear con estos.
Para esto verifica, leyendo el archivo de beneficios, que la cantidad de creditos sea menor o igual al costo de los beneficios.
Caso contrario, devuelve que al menos un beneficio no lo puede canjear, para eso sirve el flag repetido*/
void mostrarBeneficios(Alumno alumnoIngresado){
    FILE *registrosAlumnos = fopen("registrosAlumnos.dat", "rb+");
    FILE *archivoBeneficios = fopen("archivoBeneficios.dat", "rb");
    Beneficio beneficio;
    Alumno alumno;
    int flagRepetido = 0;
    if (archivoBeneficios != NULL && registrosAlumnos != NULL){
      
        while (fread(&alumno, sizeof(Alumno), 1, registrosAlumnos) == 1){
            if (strcmp(alumno.mail, alumnoIngresado.mail) == 0){
                cout << "Tienes " << alumno.creditos << " creditos" << endl;
                cout << "Puedes obtener los siguiente beneficios con tu cantidad de creditos: " << endl;

                while (fread(&beneficio, sizeof(Beneficio), 1, archivoBeneficios) == 1){
                    if (alumno.creditos >= beneficio.costo){
                        cout << "Beneficio: " << beneficio.nombre << endl;
                        cout << "Costo: " << beneficio.costo << endl;
                    }
                    else {
                        flagRepetido = 1;
                    }
                }
            }
        }
        if(flagRepetido==1){
            cout << "Al menos un beneficio no puedes canjearlo." << endl;
        }
        fclose (archivoBeneficios);
        fclose (registrosAlumnos);
    }
}

//------------------FUNCIÓN 3--------------------
void canjearBeneficio(Alumno alumnoIngresado){
    FILE *archivoBeneficios = fopen("archivoBeneficios.dat", "rb");
    FILE *registrosAlumnos = fopen("registrosAlumnos.dat", "rb+");
    Alumno alumno;

    if (archivoBeneficios && registrosAlumnos != NULL){

        Beneficio beneficio;
        char beneficio_deseado[50];
        int nuevacantidad = 0;
        char beneficio_deseado_incorrecto[50];
        bool encontrado = false;
       
        cout << "Seleccione el beneficio que desea canjear: " << endl;
        cin >> beneficio_deseado;
        
        while ((fread(&beneficio, sizeof(Beneficio), 1, archivoBeneficios) == 1)){
            if (strcmp(beneficio.nombre, beneficio_deseado) == 0) {
                while (fread(&alumno, sizeof(Alumno), 1, registrosAlumnos) == 1){
                    if (strcmp(alumno.mail, alumnoIngresado.mail)==0){                              
                        encontrado = true; 
                    
                        nuevacantidad = (alumno.creditos - beneficio.costo);
                                                        
                        alumno.creditos = nuevacantidad;
                            
                        fseek(registrosAlumnos, -sizeof(Alumno), SEEK_CUR);
                        fwrite(&alumno, sizeof(Alumno), 1, registrosAlumnos); 
                        cout << "Canje exitoso! Tu actual cantidad de credito es: " << alumno.creditos << endl;
                    }
                }
            }
        }
        fclose (registrosAlumnos);  
        fclose (archivoBeneficios); 
        
        if (!encontrado){
            cout << "Beneficio no encontrado, debe volver a iniciar el programa." << endl;
        }
    }
} 

//--------------------------------FUNCIÓN 4----------------------------------
void validarUsuario (Alumno alumnoIngresado){
    FILE *registrosAlumnos = fopen("registrosAlumnos.dat", "rb");
    if (registrosAlumnos != NULL){
       
        Alumno alumno;
        fseek(registrosAlumnos, 0, SEEK_SET);
        bool alumnoExiste = false;
        char contraseñaIncorrecta[100];
        
        while (fread(&alumno, sizeof(Alumno), 1, registrosAlumnos) == 1){
            if (strcmp(alumno.mail, alumnoIngresado.mail) == 0){
                alumnoExiste = true;
                
                cout << "Ingrese su clave: " << endl;
                cin >> alumnoIngresado.contraseña;
                if (strcmp(alumno.contraseña, alumnoIngresado.contraseña) == 0){                   
                       
                    mostrarBeneficios(alumnoIngresado); 
                    canjearBeneficio (alumnoIngresado);
                    break;
                   
                } else{
                    for (int i = 0; i < 3; i++){
                        cout << "Contraseña incorrecta, intente otra vez. Te quedan " << 3 - i  << " intentos." << endl;
                        cin >> alumnoIngresado.contraseña;
                        
                        if (strcmp(alumno.contraseña, alumnoIngresado.contraseña) == 0){
                        
                            mostrarBeneficios(alumnoIngresado);
                            canjearBeneficio (alumnoIngresado);

                        break;
                        }
                    }
                }
            }   
        }
        if (!alumnoExiste){
            cout << "No existe el mail ingresado." << endl;
        }
        fclose(registrosAlumnos);
    } else{
        cout << "No se pudo abrir el archivo." << endl;
    }
}
