#include <iostream>
#include <string>
#include <cstring>
using namespace std;

struct Alumno{
    char mail [100];
    char contraseña [100];
    int creditos = 1000;
};

//DECLARAMOS FUNCIONES
Alumno pedirAlumno();
void AgregarAlumno(int cantidad, Alumno alumnoA []);
void mostrarArchivo();


int main(){
    int cantidad;
    cout<< "Ingrese la cantidad de alumnos: "<< endl;
    cin >> cantidad;
    Alumno alumnoA[cantidad];
     

    for(int i=0; i < cantidad; i++){
        alumnoA[i] = pedirAlumno();
    }

    //Vemos que mails se guardaron
    cout<< "Los mails cargados son: " <<endl;
    
    for (int i = 0; i < cantidad; i++) {
        cout<< alumnoA[i].mail <<endl;
    }    

    //Comparamos si se repite algún mail ingresado (previo a guardar en el achivo)
    
    for(int i = 0; i < cantidad; i++) {
        for(int j = 0; j < cantidad; j++){
            if((strcmp(alumnoA[i].mail, alumnoA[j].mail)==0) && i!=j){
                cout << "Se repite un mail: " << alumnoA[i].mail << endl;
                cout <<" Se deberán cargar los mails nuevamente."<< endl;
            return 0;
            }
        }      
    }
    cout<<"No se cargaron mails repetidos."<< endl;
    
    //Escribimos el archivo.
    AgregarAlumno (cantidad, alumnoA);
    //Mostramos los alumnos del archivo.
    mostrarArchivo();

}


//DEFINIMOS FUNCIONES

//FUNCIÓN 1
Alumno pedirAlumno(){
    Alumno alumno;
    cout<< "Ingrese su mail: "<< endl;
    cin >> alumno.mail; 
    cout<<"Ingrese su clave: " <<endl;
    cin >> alumno.contraseña;
    return alumno;
}

//FUNCIÓN 2
void AgregarAlumno(int cantidad, Alumno alumnoA []){ 
    FILE* registrosAlumnos = fopen ("registrosAlumnos.dat", "ab+");
    Alumno alumno;
    fseek(registrosAlumnos, 0, SEEK_SET);
    bool alumnoRepetido = false;
    while (fread (&alumno, sizeof (Alumno),1 , registrosAlumnos) == 1){
        for (int i = 0; i < cantidad; i ++){
            if(strcmp (alumno.mail, alumnoA[i].mail)==0){
                cout << "Error. El alumno ya existe. No se pudo agregar " << alumnoA[i].mail << " al archivo." << endl;
                alumnoRepetido = true;
                fclose(registrosAlumnos);
                break;
            }
        }
    } 
    if(!alumnoRepetido){
       
        for (int i = 0; i < cantidad; i ++){
            fseek(registrosAlumnos, 0, SEEK_END);
            fwrite(&alumnoA[i], sizeof (Alumno),1, registrosAlumnos); 
        }
        cout<< "El alumno se cargó correctamente!"<<endl;
        fclose(registrosAlumnos);   
    }
    
}

//FUNCIÓN 3
void mostrarArchivo(){
    FILE * registrosAlumnos = fopen("registrosAlumnos.dat", "rb");
    Alumno alumno;
    cout << "Los alumnos disponibles son: "<< endl;
    if (registrosAlumnos != NULL){
        fseek(registrosAlumnos, 0, SEEK_SET);
        while (fread(&alumno, sizeof(Alumno), 1, registrosAlumnos) == 1){
            
            cout << "Mail del alumno: " <<alumno.mail << endl;
            cout << "Creditos: " << alumno.creditos << endl;
        }
        fclose (registrosAlumnos);
    }
} 