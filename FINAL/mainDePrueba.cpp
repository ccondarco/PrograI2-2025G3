#include <iostream>
#include <string>
#include "libreriaDePrueba.h"
using namespace std;

int main() {
    system("chcp 65001");
    system("cls");
    
    // Nombres de los archivos .bin ^w^)r
    string nombreArchivoClientes = "";
    string nombreArchivoEmpleados = "";
    string nombreArchivoProductos = "Productos_02.bin";
    string nombreArchivoVentas = "";
    string nombreArchivoFacturas = "";
    string nombreArchivoReparaciones = "Reparaciones_02.bin";

    vector<string> metodos_pago = {"Efectivo", "Tarjeta", "QR"};
    int opcion;
    do {
        system("cls");
        cout << "=== MENÚ PRINCIPAL ===\n";
        cout << "\t1. CLIENTES\n";
        cout << "\t2. EMPLEADOS\n";
        cout << "\t3. PRODUCTOS\n";
        cout << "\t4. VENTAS Y FACTURAS\n";
        cout << "\t5. REPARACIONES\n";
        cout << "\t0. SALIR\n";
        cout << "-->";
        cin >> opcion;
        if (opcion==1) {
            menuClientes();
        } else if (opcion==2){
            menuEmpleados();
        } else if(opcion==3) {
            menuABM_Productos(nombreArchivoClientes);
        } else if(opcion==4) {
            menu_principal(nombreArchivoVentas, nombreArchivoFacturas, nombreArchivoProductos, metodos_pago);
        } else if(opcion==5) {
            menuABM_Reparacion(nombreArchivoReparaciones, nombreArchivoFacturas, nombreArchivoClientes, nombreArchivoProductos);
        } else if (opcion==0) {
            cout << "Saliendo...";
        } else {
            cout << "Opción no válida\n";
            system("pause");
        }
    } while (opcion!=0);
    
    return 0;

}
