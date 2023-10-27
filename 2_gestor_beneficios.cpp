#include <iostream>
#include <string>
#include <cstring>
using namespace std;

struct Beneficio{
    char nombre [50];
    int costo;
};

FILE * archivoBeneficios;
void ListarBeneficiosDisponibles();
void EditarBeneficio(char beneficioBuscado [50]);
void AgregarBeneficio();
int EliminarBeneficios(char beneficioAEliminar [50]);

int main(){
    int opcion;
    do {
        cout << "Elige una opcion del menu:" << endl;
        cout << "1. Listar beneficios disponibles" << endl;
        cout << "2. Modificar un beneficio" << endl;
        cout << "3. Agregar un nuevo beneficio" << endl;
        cout << "4. Eliminar un beneficio" << endl;
        cout << "5. Salir del sistema" << endl;
    
        cin >> opcion;

        switch (opcion) {
            case 1: //Funcion 1: Enlistar los beneficios
                ListarBeneficiosDisponibles(); 
                break;
            case 2: //Funcion 2: Editar un beneficio
                char beneficioBuscado [50];
                int nuevoCosto;
                cout << "Ingrese el nombre del beneficio que desea modificar: "<< endl;
                cin >> beneficioBuscado;
                EditarBeneficio(beneficioBuscado);
                break;
            case 3: //Funcion 3: Agregar un beneficio
                AgregarBeneficio ();
                break;
            case 4: //Funcion 4: Elimina un beneficio
                char beneficioAEliminar [50];
                cout << "Ingrese el nombre del beneficio que desee eliminar: " << endl;
                cin >> beneficioAEliminar;
                EliminarBeneficios(beneficioAEliminar);
                break;            
            case 5:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                cout << "Opción no válida. Elija otra opcion del menú, por favor." << endl;
                break;
        }
    } while (opcion != 5);

    return 0;
}    


//--------------------PRIMER FUNCIÓN-------------------------//
void ListarBeneficiosDisponibles(){
    FILE* archivoBeneficios = fopen ("archivoBeneficios.dat", "rb");
    if (archivoBeneficios != NULL){
        Beneficio beneficio;
        cout << "Los beneficios disponibles son: "<< endl;
        while (fread (&beneficio, sizeof (Beneficio), 1, archivoBeneficios) == 1){
            cout << "Nombre del beneficio: " << beneficio.nombre << endl;
            cout << "Costo del beneficio: " << beneficio.costo << endl;
            cout << " " << endl;
        }
        fclose (archivoBeneficios);
    } 
    else{
        cout << "No se pudo abrir el archivo." << endl;
    }
}


//--------------------SEGUNDA FUNCIÓN-------------------------//
void EditarBeneficio(char beneficioBuscado [50]){
    FILE* archivoBeneficios = fopen ("archivoBeneficios.dat", "rb+");
    if (archivoBeneficios != NULL){
        Beneficio beneficio;
        bool encontrado = false;
        int nuevoCosto = 0;
        while (!encontrado && fread(&beneficio, sizeof(Beneficio), 1, archivoBeneficios) == 1){
            if (strcmp(beneficio.nombre, beneficioBuscado)==0){
                encontrado = true;
                cout << "Ingrese el nuevo costo: ";
                cin >> nuevoCosto;
                beneficio.costo = nuevoCosto;
                fseek(archivoBeneficios, -sizeof(Beneficio), SEEK_CUR);
                fwrite(&beneficio, sizeof(Beneficio), 1, archivoBeneficios);

                cout << "Costo actualizado exitosamente." << endl; 
            }
        }
        if (!encontrado){
            cout << "No se encontró un beneficio con el nombre ingresado." << endl;
        }
        fclose(archivoBeneficios);
    } 
    else{
        cout << "No se pudo abrir el archivo para lectura y escritura." << endl;
    }
}


//--------------------TERCERA FUNCIÓN-------------------------//
void AgregarBeneficio(){ 
    FILE* archivoBeneficios = fopen ("archivoBeneficios.dat", "ab+");
    
    Beneficio beneficio;
    Beneficio beneficiosAgregado;   
    
    if (archivoBeneficios != NULL){
        cout << "Ingrese el beneficio: " << endl;
        cin >> beneficiosAgregado.nombre;
        cout << "Ingrese el costo del beneficio: " << endl;
        cin >> beneficiosAgregado.costo;
    
        fseek(archivoBeneficios, 0, SEEK_SET);
        bool beneficioRepetido = false;

        while (fread (&beneficio, sizeof (Beneficio),1 , archivoBeneficios) == 1){
            if(strcmp (beneficiosAgregado.nombre, beneficio.nombre)==0){
                cout << "Error. El beneficio ya existe. No se pudo agregar." << endl;
                beneficioRepetido = true;
                break;
                fclose(archivoBeneficios);
            }
        } 
        if(!beneficioRepetido){
            fseek(archivoBeneficios, 0, SEEK_END);
            fwrite(&beneficiosAgregado, sizeof (Beneficio),1, archivoBeneficios); 
            cout<< "El beneficio se cargo correctamente!"<<endl;   
        }
        fclose(archivoBeneficios);
    } 
    else {
        cout << "No se pudo abrir el archivo." << endl;
    }
}


//--------------------CUARTA FUNCIÓN-------------------------//
int EliminarBeneficios(char beneficioAEliminar [50]){
    FILE *archivoBeneficios = fopen("archivoBeneficios.dat", "rb+"); 

    if (archivoBeneficios != NULL){
        FILE *archivoBeneficiosTemporal = fopen("archivoBeneficiosTemporal.dat", "wb");

        if (archivoBeneficiosTemporal != NULL){
            Beneficio beneficio;
            
            bool encontrado = false;
            while (!encontrado && fread(&beneficio, sizeof(Beneficio), 1, archivoBeneficios) == 1){
                     
                if (strcmp (beneficio.nombre, beneficioAEliminar)!=0){
                    fwrite(&beneficio, sizeof(Beneficio), 1, archivoBeneficiosTemporal);
                }  
            }
            if (!encontrado){
                cout << "No se encontró un beneficio con el nombre ingresado." << endl;
                return 0;
            }

            fclose(archivoBeneficios);
            fclose(archivoBeneficiosTemporal);

            FILE *archivoBeneficiosOriginal = fopen("archivoBeneficios.dat", "wb");
            FILE *archivoBeneficiosTemp = fopen("archivoBeneficiosTemporal.dat", "rb");

            if (archivoBeneficiosOriginal != NULL && archivoBeneficiosTemp != NULL){
                
                while (fread(&beneficio, sizeof(Beneficio), 1, archivoBeneficiosTemp) == 1){
                    fwrite(&beneficio, sizeof(Beneficio), 1, archivoBeneficiosOriginal);
                }

                fclose(archivoBeneficiosOriginal);
                fclose(archivoBeneficiosTemp);

                cout << "Beneficio eliminado exitosamente." << endl;
            } 
            else {
                cout << "Error al abrir los archivos para copiar." << endl;
            }
        } 
        else {
            cout << "No se pudo crear el nuevo archivo." << endl;
        }
    } 
    else {
        cout << "No se pudo abrir el archivo de beneficios." << endl;
    }
}